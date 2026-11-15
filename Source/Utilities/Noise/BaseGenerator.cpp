/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseGenerator.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/09/2009 @ 18:42:12 - nwilliam
**  \brief   Implementation of base generator class
*******************************************************************************/

#include "BaseGenerator.h"
#include "Utilities/Misc.h"
#include "Math/MiscMathFunctions.h"

#include "Utilities/LeakCheck.h"

namespace SF { 
namespace Utilities {
namespace Noise
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  BaseGenerator::BaseGenerator()
  {
  }

  BaseGenerator::~BaseGenerator()
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  unsigned char* BaseGenerator::BuildTexture( bool buildNoise )
  {
    if( buildNoise )
      Build();

    int width = m_array.Width();
    int height = m_array.Height();

    const NoiseVal* data = m_array.Data();

    unsigned char* image = new unsigned char[ width * height * 3 ];
    unsigned char* pixel = image;

    for( int y = 0; y < height; ++y )
    {
      for( int x = 0; x < width; ++x )
      {
        // Assume input range is [-1,1] (this is desired)
        // Could take (data.scaleMin/scaleMax) and rescale to [0,1], but don't
        // necessarily want [0,1] so let it be handled before we get here
        float t = ( *data + 1.0f ) / 2.0f;
        unsigned char color = static_cast<unsigned char>( 255 * t );

        pixel[0] = pixel[1] = pixel[2] = color;
        pixel += 3;
        data++;
      }
    }

    // Hey ctallman, uncomment this and look at the north pole
    /*pixel = image;
    for( int i = 0; i < (width*height)/2; ++i )
    {
      pixel[0] = pixel[1] = pixel[2] = 0;
      pixel += 3;
    }*/

    return image;
  }

  NoiseData& BaseGenerator::GetNoiseData()
  {
    return m_data;
  }

  void BaseGenerator::SetNoiseData( const NoiseData& data )
  {
    m_data = data;
    m_array.Resize( m_data.width, m_data.height );
  }

  int BaseGenerator::Width() const
  {
    return m_data.width;
  }

  int BaseGenerator::Height() const
  {
    return m_data.height;
  }

  NoiseArray& BaseGenerator::Array()
  {
    return m_array;
  }

  void BaseGenerator::Width( int width )
  {
    m_data.width = width;
    m_array.Width( width );
  }

  void BaseGenerator::Height( int height )
  {
    m_data.height = height;
    m_array.Height( height );
  }

  void BaseGenerator::Array( NoiseArray& noiseArray )
  {
    m_array = noiseArray;
  }

  void BaseGenerator::SetSize( int width, int height )
  {
    m_data.width = width;
    m_data.height = height;
    m_array.Resize( width, height );
  }

  void BaseGenerator::SetNoise( GeneratorType noiseType )
  {
    m_data.noiseType = noiseType;
  }

} // Noise namespace
} // Utilities namespace
} // SF namespace
