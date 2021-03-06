#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include <errno.h>
#include <string.h>

#include "cvd/config.h"
#include "cvd/Linux/v4l2buffer.h"
#include "cvd/timer.h"

#include <sys/types.h>   // included for open()
#include <sys/stat.h>    // included for open()
#include <fcntl.h>       // included for open()
#include <unistd.h>      // included for close();
#include <sys/ioctl.h>   
#include <unistd.h>      // included for mmap
#include <sys/mman.h>    // included for mmap

using namespace std;

namespace CVD {

Exceptions::V4L2Buffer::DeviceOpen::DeviceOpen(string device)
{
	what = "V4L2Buffer: failed to open \""+device+ "\": " + strerror(errno);
}

Exceptions::V4L2Buffer::DeviceSetup::DeviceSetup(string device, string action)
{
	what = "V4L2Buffer: \""+action + "\" ioctl failed on " + device + ": " +strerror(errno);
}

Exceptions::V4L2Buffer::PutFrame::PutFrame(string device)
{
	what = "V4L2Buffer: Requeuing buffer in put_frame failed (putting back a frame twice?) on " + device;
}

Exceptions::V4L2Buffer::GetFrame::GetFrame(string device)
{
	what = "V4L2Buffer: Dequeueing buffer in get frame failed on " + device;
}

void ErrorInfo(int e)
{
	cerr << "Error: " << strerror(e) << endl;
}

V4L2Buffer_Base::V4L2Buffer_Base(const char *devname, bool fields, V4L2BufferBlockMethod block, int input, int bufs, unsigned long int pixtype)
{
	device = devname;
	my_frame_rate=0;
	my_block_method=block;
	i_am_using_fields=fields;
	num_buffers = bufs;

	// Open the device.
	m_nVideoFileDesc=open(devname,O_RDWR|O_NONBLOCK);

	if(m_nVideoFileDesc == -1)
		throw Exceptions::V4L2Buffer::DeviceOpen(devname);

	// Get device capabilites::
	struct v4l2_capability sv4l2Capability;
	if(0!=ioctl(m_nVideoFileDesc, VIDIOC_QUERYCAP, &sv4l2Capability))
		throw Exceptions::V4L2Buffer::DeviceSetup(devname, "Query capabilities");


	cout << "  V4L2Buffer_Base: Device name:"<< sv4l2Capability.card <<endl;
	cout << "  V4L2Buffer_Base: (If that was garbled then you've not go the right modules loaded.) "<<endl;
  
	// Select video input
	struct v4l2_input sv4l2Input;
	sv4l2Input.index=input;  // This is the composite input
	if(ioctl(m_nVideoFileDesc, VIDIOC_S_INPUT, &sv4l2Input))
		throw Exceptions::V4L2Buffer::DeviceSetup(devname, "Select composite input");  

	// and PAL
	v4l2_std_id stdId=V4L2_STD_PAL;
	if(ioctl(m_nVideoFileDesc, VIDIOC_S_STD, &stdId ))
		throw Exceptions::V4L2Buffer::DeviceSetup(devname, "Set PAL");

	// Get / Set capture format.
	struct v4l2_format sv4l2Format;
	sv4l2Format.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(ioctl(m_nVideoFileDesc, VIDIOC_G_FMT, &sv4l2Format))
		throw Exceptions::V4L2Buffer::DeviceSetup(devname, "Get capture format");

	// ******************************************* CAPTURE FORMAT******
	sv4l2Format.fmt.pix.width=768;
	sv4l2Format.fmt.pix.pixelformat=pixtype;
	sv4l2Format.fmt.pix.height=576;
	if(i_am_using_fields)
	{
		sv4l2Format.fmt.pix.field=  V4L2_FIELD_ALTERNATE;
	};                                                            
	if(!i_am_using_fields)
	{
		sv4l2Format.fmt.pix.field=  V4L2_FIELD_INTERLACED;
	};

	// ****************************************************************
	if(ioctl(m_nVideoFileDesc, VIDIOC_S_FMT, &sv4l2Format))
		throw Exceptions::V4L2Buffer::DeviceSetup(devname, "Set capture format");

	cout<<"  V4L2Buffer_Base: Using capture format: "<<sv4l2Format.fmt.pix.width<<
		"x"<<sv4l2Format.fmt.pix.height<<
		", image size:"<<sv4l2Format.fmt.pix.sizeimage<<endl;
	my_image_size.x=sv4l2Format.fmt.pix.width;
	my_image_size.y=sv4l2Format.fmt.pix.height;


	// Set up the streaming buffer request
	struct v4l2_requestbuffers sv4l2RequestBuffers;
	sv4l2RequestBuffers.count=num_buffers;
	sv4l2RequestBuffers.type=V4L2_BUF_TYPE_VIDEO_CAPTURE; //|V4L2_BUF_ATTR_DEVICEMEM;
	sv4l2RequestBuffers.memory=V4L2_MEMORY_MMAP;
	
	//bttv now returns num_buffers
	if(ioctl(m_nVideoFileDesc,VIDIOC_REQBUFS,&sv4l2RequestBuffers)==-1)
		throw Exceptions::V4L2Buffer::DeviceSetup(devname, "Request capture buffers");

	num_buffers = sv4l2RequestBuffers.count;
  
	m_sv4l2Buffer = new struct v4l2_buffer [num_buffers];
	m_pvVideoBuffer = new void*[num_buffers];


  // Set up the streaming buffers, and mmap them
	for(int ii=0;ii<num_buffers;ii++) 
    {
		m_sv4l2Buffer[ii].index=ii;
		m_sv4l2Buffer[ii].type=sv4l2RequestBuffers.type;  

		if(ioctl(m_nVideoFileDesc,VIDIOC_QUERYBUF,&m_sv4l2Buffer[ii]))
			throw Exceptions::V4L2Buffer::DeviceSetup(devname, "Query streaming buffer");
	

		m_pvVideoBuffer[ii]=mmap(0,m_sv4l2Buffer[ii].length,PROT_READ|PROT_WRITE,MAP_SHARED,m_nVideoFileDesc, m_sv4l2Buffer[ii].m.offset);

		if(m_pvVideoBuffer[ii] == MAP_FAILED)
			throw Exceptions::V4L2Buffer::DeviceSetup(devname, "MMap buffers");
		
		if(0!= ioctl(m_nVideoFileDesc,VIDIOC_QBUF,&m_sv4l2Buffer[ii]))
			throw Exceptions::V4L2Buffer::DeviceSetup(devname, "Queue streaming buffer");
		  
    }

	if(ioctl(m_nVideoFileDesc,VIDIOC_STREAMON, &m_sv4l2Buffer[0].type))
		throw Exceptions::V4L2Buffer::DeviceSetup(devname, "Begin streaming (is device already in use?)");

}


V4L2Buffer_Base::~V4L2Buffer_Base()
{
	if(ioctl(m_nVideoFileDesc,VIDIOC_STREAMOFF, &m_sv4l2Buffer[0].type))
		throw Exceptions::V4L2Buffer::DeviceSetup(device, "End streaming");

	for(int ii=0;ii<V4L2BUFFERS;ii++)
		munmap(m_pvVideoBuffer[ii], m_sv4l2Buffer[ii].length);

	delete[] m_sv4l2Buffer;
	delete[] m_pvVideoBuffer;

	close(m_nVideoFileDesc);
}  

void V4L2Buffer_Base::put_frame(V4L2FrameT<unsigned char> *f)
{
  // Requeue
  if(ioctl(m_nVideoFileDesc,VIDIOC_QBUF,f->m_buf))
		throw Exceptions::V4L2Buffer::PutFrame(device);

  delete f->m_buf;
  // Delete the frame
  delete f;
}

void V4L2Buffer_Base::put_frame(VideoFrame<unsigned char> *f)
{
  put_frame((V4L2FrameT<unsigned char> *)f);
}

bool V4L2Buffer_Base::frame_pending(){
  fd_set fdsetRead;
  fd_set fdsetOther;
  struct timeval tv;
 
  FD_ZERO(&fdsetRead);
  FD_SET(m_nVideoFileDesc,&fdsetRead);
  FD_ZERO(&fdsetOther);
  tv.tv_sec=0;
  tv.tv_usec=0;
  if (select(m_nVideoFileDesc+1,&fdsetRead,&fdsetOther,&fdsetOther,&tv)>0)  
    return true;
  return false;
}

V4L2FrameT<unsigned char>* V4L2Buffer_Base::get_frame(){

  fd_set fdsetRead;
  fd_set fdsetOther;
  struct v4l2_buffer buffer;
  struct v4l2_buffer buffer2;
  V4L2FrameT<unsigned char> *frame;
 
  buffer.type=m_sv4l2Buffer[0].type;
  buffer2.type=m_sv4l2Buffer[0].type;
 
  // Block until one is ready and dequeue
  switch (my_block_method) {
  case V4L2BBMselect:
    FD_SET(m_nVideoFileDesc,&fdsetRead);
    FD_ZERO(&fdsetOther);
    select(m_nVideoFileDesc+1,&fdsetRead,&fdsetOther,&fdsetOther,NULL);  

    if(ioctl(m_nVideoFileDesc,VIDIOC_DQBUF,&buffer))
		throw Exceptions::V4L2Buffer::GetFrame(device);

    break;
  case V4L2BBMsleep:
    while(ioctl(m_nVideoFileDesc,VIDIOC_DQBUF,&buffer)) {
      usleep(10);
    }
    break;
  case V4L2BBMchew:
    // wait for a new one
    while(ioctl(m_nVideoFileDesc,VIDIOC_DQBUF,&buffer)) {      
    }
    // check for a newer one 
    while(!ioctl(m_nVideoFileDesc,VIDIOC_DQBUF,&buffer2)) {
      ioctl(m_nVideoFileDesc,VIDIOC_QBUF,&buffer);
      buffer=buffer2;
    }
    break;
  }

  // Create a new frame with the data
  VideoFrameFlags::FieldType field;

  switch(buffer.flags) {
      case V4L2_FIELD_NONE:
        field=VideoFrameFlags::Progressive;
        break;
      case V4L2_FIELD_TOP:
        field=VideoFrameFlags::Top;
	break;
      case V4L2_FIELD_BOTTOM:
        field=VideoFrameFlags::Bottom;
	break;
      case V4L2_FIELD_INTERLACED:
        field=VideoFrameFlags::Both;
	break;
      default:
        field=VideoFrameFlags::Unknown;
	break;
    }
  
  frame=new V4L2FrameT<unsigned char>(timer.conv_ntime(buffer.timestamp),my_image_size,buffer.index,(unsigned char *)m_pvVideoBuffer[buffer.index], field);

  frame->m_buf = new struct v4l2_buffer;
  *(frame->m_buf) = buffer;

  return frame;
}

}


