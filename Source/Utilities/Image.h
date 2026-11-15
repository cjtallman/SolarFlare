/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Image.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 10/27/2008 @ 17:36:58 - nwilliam
**  \brief   Base class for all image types.
*******************************************************************************/

#ifndef SF_IMAGE_H
#define SF_IMAGE_H

#include <string>
#include "Color.h"
#include "Utilities/SharedArray.h"

namespace SF {
namespace Utilities
{
  class Image
  {
    public:
      enum PixelFormat { INVALID, GRAY, GRAY_ALPHA, RGB, RGBA };
      
      Image();
      Image( const std::string& filename );
      Image( int width, int height, PixelFormat format );
      ~Image();
      
      // Default copy, assignment ok
      
      int BPP() const;
      int Width() const;
      int Height() const;
      int PixelDataSize() const;
      PixelFormat Format() const;
      const unsigned char* PixelData() const;
      
      void SetTexel( float u, float v, Color color );
      void SetTexel( int row, int col, Color color );
      Color GetTexel( float u, float v ) const;
      Color GetTexel( int row, int col ) const;
      
      void WriteToFile( const std::string& filename );
      void LoadFromFile( const std::string& filename );
      void SetData( unsigned char* data, int width, int height, PixelFormat format );

      void Swap( Image& rhs );

    protected:
      int CalculateBPP( PixelFormat format ) const;
      
      int m_bpp;
      int m_width;
      int m_height;
      int m_dataSize;
      PixelFormat m_format;
      SharedArray<unsigned char> m_pixelData;
  };
  
} // Utilities namespace
} // SF namespace

#endif // SF_IMAGE_H
