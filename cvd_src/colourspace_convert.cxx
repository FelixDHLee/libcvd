#include <cvd/image_convert_fwd.h>
#include <cvd/colourspaces.h>
#include <cvd/colourspace.h>
#include <cvd/colourspace_convert.h>
#include <cvd/byte.h>
#include <cvd/rgb.h>

namespace CVD
{
	
  template<> void convert_image(const BasicImage<bayer_bggr>& from, BasicImage<Rgb<byte> >& to)
  {
    ColourSpace::bayer_to_rgb_bggr(reinterpret_cast<const unsigned char*>(from.data()), 
			      reinterpret_cast<unsigned char*>(to.data()), 
			      from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_gbrg>& from, BasicImage<Rgb<byte> >& to)
  {
    ColourSpace::bayer_to_rgb_gbrg(reinterpret_cast<const unsigned char*>(from.data()),
			      reinterpret_cast<unsigned char*>(to.data()), 
			      from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_grbg>& from, BasicImage<Rgb<byte> >& to)
  {
    ColourSpace::bayer_to_rgb_grbg(reinterpret_cast<const unsigned char*>(from.data()),
			      reinterpret_cast<unsigned char*>(to.data()), 
			      from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_rggb>& from, BasicImage<Rgb<byte> >& to)
  {
    ColourSpace::bayer_to_rgb_rggb(reinterpret_cast<const unsigned char*>(from.data()),
			      reinterpret_cast<unsigned char*>(to.data()), 
			      from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_bggr>& from, BasicImage<byte>& to)
  {
    ColourSpace::bayer_to_grey_bggr(reinterpret_cast<const unsigned char*>(from.data()), to.data(), from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_gbrg>& from, BasicImage<byte>& to)
  {
    ColourSpace::bayer_to_grey_gbrg(reinterpret_cast<const unsigned char*>(from.data()), to.data(), from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_grbg>& from, BasicImage<byte>& to)
  {
    ColourSpace::bayer_to_grey_grbg(reinterpret_cast<const unsigned char*>(from.data()), to.data(), from.size().x, from.size().y);
  }
	
  template<> void convert_image(const BasicImage<bayer_rggb>& from, BasicImage<byte>& to)
  {
    ColourSpace::bayer_to_grey_rggb(reinterpret_cast<const unsigned char*>(from.data()), to.data(), from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_bggr16>& from, BasicImage<Rgb<unsigned short> >& to)
  {
    ColourSpace::bayer_to_rgb_bggr(reinterpret_cast<const unsigned short*>(from.data()), 
			      reinterpret_cast<unsigned short*>(to.data()), 
			      from.size().x, from.size().y);
  }
  template<> void convert_image(const BasicImage<bayer_gbrg16>& from, BasicImage<Rgb<unsigned short> >& to)
  {
    ColourSpace::bayer_to_rgb_gbrg(reinterpret_cast<const unsigned short*>(from.data()),
			      reinterpret_cast<unsigned short*>(to.data()), 
			      from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_grbg16>& from, BasicImage<Rgb<unsigned short> >& to)
  {
    ColourSpace::bayer_to_rgb_grbg(reinterpret_cast<const unsigned short*>(from.data()),
			      reinterpret_cast<unsigned short*>(to.data()), 
			      from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_rggb16>& from, BasicImage<Rgb<unsigned short> >& to)
  {
    ColourSpace::bayer_to_rgb_rggb(reinterpret_cast<const unsigned short*>(from.data()),
			      reinterpret_cast<unsigned short*>(to.data()), 
			      from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_bggr16>& from, BasicImage<unsigned short>& to)
  {
    ColourSpace::bayer_to_grey_bggr(reinterpret_cast<const unsigned short*>(from.data()), to.data(), from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_gbrg16>& from, BasicImage<unsigned short>& to)
  {
    ColourSpace::bayer_to_grey_gbrg(reinterpret_cast<const unsigned short*>(from.data()), to.data(), from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_grbg16>& from, BasicImage<unsigned short>& to)
  {
    ColourSpace::bayer_to_grey_grbg(reinterpret_cast<const unsigned short*>(from.data()), to.data(), from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_rggb16>& from, BasicImage<unsigned short>& to)
  {
    ColourSpace::bayer_to_grey_rggb(reinterpret_cast<const unsigned short*>(from.data()), to.data(), from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_bggr16be>& from, BasicImage<Rgb<unsigned short> >& to)
  {
    ColourSpace::bayer_to_rgb_bggr_be(reinterpret_cast<const unsigned short*>(from.data()), 
			      reinterpret_cast<unsigned short*>(to.data()), 
			      from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_gbrg16be>& from, BasicImage<Rgb<unsigned short> >& to)
  {
    ColourSpace::bayer_to_rgb_gbrg_be(reinterpret_cast<const unsigned short*>(from.data()),
			      reinterpret_cast<unsigned short*>(to.data()), 
			      from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_grbg16be>& from, BasicImage<Rgb<unsigned short> >& to)
  {
    ColourSpace::bayer_to_rgb_grbg_be(reinterpret_cast<const unsigned short*>(from.data()),
			      reinterpret_cast<unsigned short*>(to.data()), 
			      from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_rggb16be>& from, BasicImage<Rgb<unsigned short> >& to)
  {
    ColourSpace::bayer_to_rgb_rggb_be(reinterpret_cast<const unsigned short*>(from.data()),
			      reinterpret_cast<unsigned short*>(to.data()), 
			      from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_bggr16be>& from, BasicImage<unsigned short>& to)
  {
    ColourSpace::bayer_to_grey_bggr_be(reinterpret_cast<const unsigned short*>(from.data()), to.data(), from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_gbrg16be>& from, BasicImage<unsigned short>& to)
  {
    ColourSpace::bayer_to_grey_gbrg_be(reinterpret_cast<const unsigned short*>(from.data()), to.data(), from.size().x, from.size().y);
  }
  
  template<> void convert_image(const BasicImage<bayer_grbg16be>& from, BasicImage<unsigned short>& to)
  {
    ColourSpace::bayer_to_grey_grbg_be(reinterpret_cast<const unsigned short*>(from.data()), to.data(), from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<bayer_rggb16be>& from, BasicImage<unsigned short>& to)
  {
    ColourSpace::bayer_to_grey_rggb_be(reinterpret_cast<const unsigned short*>(from.data()), to.data(), from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<yuv411>& from, BasicImage<Rgb<byte> >& to)
  {
    ColourSpace::yuv411_to_rgb(reinterpret_cast<const unsigned char*>(from.data()),
			       from.totalsize(),
			       reinterpret_cast<unsigned char*>(to.data()));
  }

  template<> void convert_image(const BasicImage<yuv411>& from, BasicImage<byte>& to)
  {
    ColourSpace::yuv411_to_y(reinterpret_cast<const unsigned char*>(from.data()),
			     from.totalsize(),
			     reinterpret_cast<unsigned char*>(to.data()));
  }

  template<> void convert_image(const BasicImage<yuv422>& from, BasicImage<Rgb<byte> >& to)
  {
    ColourSpace::yuv422_to_rgb(reinterpret_cast<const byte*>(from.data()), reinterpret_cast<byte*>(to.data()), from.size().x, from.size().y);
  }

  template<> void convert_image(const BasicImage<yuv422>& from, BasicImage<byte>& to)
  {
    ColourSpace::yuv422_to_grey(reinterpret_cast<const byte*>(from.data()), to.data(), from.size().x, from.size().y);
  }

  // Name changed from 'convert_image' to prevent conflict with previous convert_image
  // with same method signature.
  template<> std::pair<Image<byte>,Image<Rgb<byte> > > convert_image_pair(const BasicImage<yuv411>& from)
  {
    Image<byte> rety(from.size());
    Image<Rgb<byte> > retc(from.size());

    ColourSpace::yuv411_to_rgb_y(reinterpret_cast<const unsigned char*>(from.data()),
				 from.totalsize(),
				 reinterpret_cast<unsigned char*>(retc.data()),
				 reinterpret_cast<unsigned char*>(rety.data()));

    return std::pair<Image<byte>,Image<Rgb<byte> > >(rety, retc);

  }
	
	template<> void convert_image(const BasicImage<vuy422>& from, BasicImage<Rgb<byte> >& to)
	{
		ColourSpace::vuy422_to_rgb(reinterpret_cast<const byte*>(from.data()), reinterpret_cast<byte*>(to.data()), from.size().x, from.size().y);
	}

	template<> void convert_image(const BasicImage<vuy422>& from, BasicImage<byte>& to)
	{
		ColourSpace::vuy422_to_grey(reinterpret_cast<const byte*>(from.data()), to.data(), from.size().x, from.size().y);
	}	
}
