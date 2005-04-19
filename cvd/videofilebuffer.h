
#ifndef CVD_VIDEOFILEBUFFER_H
#define CVD_VIDEOFILEBUFFER_H

#include <vector>
#include <string>
#include <fstream>
#include <errno.h>

#include <cvd/localvideobuffer.h>
#include <cvd/videobufferflags.h>
#include <cvd/videofilebuffer_frame.h>
#include <cvd/image_io.h>

#include <cvd/byte.h>
#include <cvd/rgb.h>

#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>

struct AVFormatContext;
struct AVFrame; 

namespace CVD
{
	namespace Exceptions
	{
		/// %Exceptions specific to VideoFileBuffer
		/// @ingroup gException
		namespace VideoFileBuffer
		{
			/// Base class for all VideoFileBuffer exceptions
			struct All: public CVD::Exceptions::VideoBuffer::All { };
			/// Unable to open the file as a video stream, for various reasons
			struct FileOpen: public All { FileOpen(const std::string& file, const std::string& error); ///< Construt from filename and error message
			};
			/// Unable to open allocate a video frame
			struct BadFrameAlloc: public All { BadFrameAlloc(); };
			/// Unable to decode the video frame
			struct BadDecode: public All { BadDecode(double t); ///< Construt from frame timestamp
			};
			/// get_frame() was called when at the end of the buffer
			struct EndOfFile: public All { EndOfFile(); };
			/// seek_to() was called for an invalid timestamp
			struct BadSeek: public All { BadSeek(double t); ///< Construt from timestamp
			};
		}
	}

	namespace VFB
	{

	template<class C> struct rgb
	{
		static const bool p=Error__type_not_valid___Use_byte_or_rgb_of_byte;
	};

	template<> struct rgb<CVD::byte>
	{	
		static const bool p=false;
	};

	template<> struct rgb<CVD::Rgb<CVD::byte> >
	{	
		static const bool p=true;
	};

	class RawVideoFileBuffer 
	{
		public:
			RawVideoFileBuffer(const std::string& file, bool is_rgb);
			~RawVideoFileBuffer();
		
			// Base class interface
 			ImageRef size()
			{
				return my_size;
			}

			VideoFileFrame<byte>* get_frame();
			void put_frame(VideoFrame<CVD::byte>* f);

			bool frame_pending()
			{
				return frame_ready;
			}

			void seek_to(double t);
			
			// This class additions	
			void on_end_of_buffer(VideoBufferFlags::OnEndOfBuffer behaviour) 
			{
				end_of_buffer_behaviour = behaviour;
			}
		
			double frames_per_second() 
			{
				return pCodecContext->frame_rate / static_cast<double>(pCodecContext->frame_rate_base);
			};
			
			std::string file_name() 
			{
				return pFormatContext->filename;
			}
			
			std::string codec_name() 
			{
				return pCodecContext->codec_name;
			}
		
		private:
			bool read_next_frame();
				
		private:
			ImageRef my_size;
			VideoBufferFlags::OnEndOfBuffer end_of_buffer_behaviour;
			double start_time;
			bool frame_ready;

			AVFormatContext* pFormatContext;
			int video_stream;
			AVCodecContext* pCodecContext;
		    AVFrame* pFrame; 
    		AVFrame* pFrameRGB;
			CVD::Image<CVD::byte> next_frame;
			double frame_time;
			bool is_rgb;
	};
	}

	///	A video buffer to play frames from a video file.
	/// This uses the ffmpeg library (http://ffmpeg.sourceforge.net/) to play
	/// a wide range of video formats, including MPEG (1, 2 and 4) and AVI (including
	/// DivX and DV) files. 
	/// Provides frames of type CVD::VideoFileFrame and throws exceptions of type
	///  CVD::Exceptions::VideoFileBuffer
	/// @param T The pixel type of the video frames. Currently only <code>CVD::Rgb<CVD::byte> ></code> and 
	/// <code>CVD::byte></code> are supported.
	/// @ingroup gVideoBuffer
	template<typename T> 
	class VideoFileBuffer : public CVD::LocalVideoBuffer<T>
	{
		private:
			VFB::RawVideoFileBuffer vf;
			
	
		public:
			/// Construct a VideoFileBuffer to play this file
			/// @param file The path to the video file
			VideoFileBuffer(const std::string& file)
			:vf(file, VFB::rgb<T>::p)
			{
			}

			~VideoFileBuffer()
			{
			}
		
			// Base class interface
			virtual ImageRef size()
			{
				return vf.size();
			}

			virtual bool frame_pending()
			{ 
				return vf.frame_pending();
			}

			/// What should the buffer do when it reaches the end of the list of files?
			/// @param behaviour The desired behaviour
			virtual void on_end_of_buffer(VideoBufferFlags::OnEndOfBuffer behaviour) 
			{
				vf.on_end_of_buffer(behaviour);
			}

			virtual void seek_to(double t)
			{
				vf.seek_to(t);
			}

			virtual VideoFileFrame<T> * get_frame()
			{
				return reinterpret_cast<VideoFileFrame<T>*>(vf.get_frame());
			}

			virtual void put_frame(VideoFrame<T>* f)
			{
				vf.put_frame(reinterpret_cast<VideoFrame<byte>*>(f));
			}
			
			// This class additions	
		
			double frame_rate() 
			{
				return vf.frames_per_second();
			}

			/// What is the path to the video file?
			std::string file_name() 
			{
				return vf.file_name();
			}

			/// What codec is being used to decode this video?
			std::string codec_name() 
			{
				return vf.codec_name();
			}
		
		private:
	};
}

#endif