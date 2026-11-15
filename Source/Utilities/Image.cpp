/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Image.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/24/2009 @ 16:23:55 - nwilliam
**  \brief   Implementation of common image functionality.
*******************************************************************************/

#include <cstring>      // memset

#include "Image.h"
#include "Asserts.h"
#include "Utilities/Misc.h"
#include "External/SOIL/include/SOIL.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Utilities
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  Image::Image() : m_bpp( 0 ),
                   m_width( 0 ),
                   m_height( 0 ),
                   m_dataSize( 0 ),
                   m_format( Image::INVALID ),
                   m_pixelData( 0 )
  {
    *this = Image( 16, 16, RGB );
  }

  Image::Image( const std::string& filename ) : m_bpp( 0 ),
                                                m_width( 0 ),
                                                m_height( 0 ),
                                                m_dataSize( 0 ),
                                                m_format( Image::INVALID ),
                                                m_pixelData( 0 )
  {
    LoadFromFile( filename );
  }

  Image::Image( int width, int height, Image::PixelFormat format ) : m_bpp( 0 ),
                                                                     m_width( width ),
                                                                     m_height( height ),
                                                                     m_format( format )
  {
    m_bpp = CalculateBPP( format );
    m_dataSize = m_width * m_height * m_format;
    m_pixelData = new unsigned char[m_dataSize];
    
    std::memset( m_pixelData.Get(), 0, m_dataSize );
  }

  Image::~Image()
  {
  }

  int Image::BPP() const
  {
    return m_bpp;
  }

  int Image::Width() const
  {
    return m_width;
  }

  int Image::Height() const
  {
    return m_height;
  }

  int Image::PixelDataSize() const
  {
    return m_dataSize;
  }

  Image::PixelFormat Image::Format() const
  {
    return m_format;
  }

  const unsigned char* Image::PixelData() const
  {
    return m_pixelData.Get();
  }

  void Image::WriteToFile( const std::string& filename )
  {
    SOIL_save_image( filename.c_str(),
                     SOIL_SAVE_TYPE_BMP,
                     m_width,
                     m_height,
                     m_format,
                     m_pixelData.Get()
                   );
  }

  void Image::LoadFromFile( const std::string& filename )
  {
    int channels = 0;
    m_pixelData = SOIL_load_image( filename.c_str(), &m_width, &m_height, &channels, SOIL_LOAD_AUTO );

    m_format = static_cast<PixelFormat>( channels );

    m_bpp = CalculateBPP( m_format );
    m_dataSize = m_width * m_height * channels;
  }

  void Image::Swap( Image& rhs )
  {
    Utilities::Swap( m_bpp,      rhs.m_bpp );
    Utilities::Swap( m_width,    rhs.m_width );
    Utilities::Swap( m_height,   rhs.m_height );
    Utilities::Swap( m_dataSize, rhs.m_dataSize );
    Utilities::Swap( m_format,   rhs.m_format );

    m_pixelData.Swap( rhs.m_pixelData );
  }

  void Image::SetData( unsigned char* data, int width, int height, PixelFormat format )
  {
    m_width = width;
    m_height = height;
    m_format = format;
    m_pixelData = data;
    m_bpp = CalculateBPP( format );
    
    m_dataSize = format * m_width * m_height;
  }
  
  void Image::SetTexel( float u, float v, SF::Color color )
  {
    return SetTexel( static_cast<int>(m_height*v), static_cast<int>(m_width*u), color );
  }

  void Image::SetTexel( int row, int col, Color color )
  {
    ASSERT_MSG( m_pixelData, "Texture Contains No Data" );
    
    
    
    int index = row*m_width + col;
    
    char bytes = static_cast<char>( m_bpp / 8 );
    
    switch( bytes )
    {
      case 4:
        m_pixelData[bytes*(index) + 3] = color.a;
      default:
      case 3:
        m_pixelData[bytes*(index) + 2] = color.b;
      case 2:
        m_pixelData[bytes*(index) + 1] = color.g;
      case 1:
        m_pixelData[bytes*(index) + 0] = color.r;
      break;
    }
  }

  int Round( float x )
  {
    return static_cast<int>(x + .5f);
  }

  Color Image::GetTexel( float u, float v ) const
  {
    ASSERT_MSG( m_pixelData, "Texture Contains No Data" );

    return GetTexel( static_cast<int>(m_height*v), static_cast<int>(m_width*u) );
    
  }

  Color Image::GetTexel( int row, int col ) const
  {
    ASSERT_MSG( m_pixelData, "Texture Contains No Data" );

   

    int index = row * m_width + col;
    
    Color color;
    char bytes = static_cast<char>( m_bpp / 8 );
    
    switch( bytes )
    {
      case 4:
        color.a = m_pixelData[bytes*index + 3];
      default:
      case 3:
        color.b = m_pixelData[bytes*index + 2];
      case 2:
        color.g = m_pixelData[bytes*index + 1];
      case 1:
        color.r = m_pixelData[bytes*index + 0];
      break;
    }

    return color;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  int Image::CalculateBPP( PixelFormat format ) const
  {
    int bpp = -1;

    switch( format )
    {
      case GRAY:        bpp = 8;  break;
      case GRAY_ALPHA:  bpp = 16; break;
      case RGB:         bpp = 24; break;
      case RGBA:        bpp = 32; break;
    }

    return bpp;
  }

} // Utilities namespace
} // SF namespace
