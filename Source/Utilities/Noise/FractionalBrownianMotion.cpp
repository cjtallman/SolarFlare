/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    FractionalBrownianMotion.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/20/2008 @ 21:38:57 - nwilliam
**  \brief   Implementation of fBM based generator.
*******************************************************************************/

#include "Utilities/Asserts.h"
#include "FractionalBrownianMotion.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  //int FractionalBrownianMotion::ParameterFlags() const
  //{
  //  return PF_HURST_INDEX | PF_LACUNARITY | PF_OCTAVES;
  //}
  
  NoiseVal FBMValue1D( const NoiseData& data, NoiseVal x )
  {
    ASSERT( data.Noise1 );
    
    NoiseVal value = 0.0f;
    
    for( int i = 0; i < data.octaves; ++i )
    {
      value += ( data.Noise1( x ) * data.weights[i] );
      x *= data.lacunarity;
    }
    
    return value;
  }
  
  NoiseVal FBMValue2D( const NoiseData& data, NoiseVal x, NoiseVal y )
  {
    ASSERT( data.Noise2 );
    
    NoiseVal value = 0.0f;
    
    for( int i = 0; i < data.octaves; ++i )
    {
      value += ( data.Noise2( x, y ) * data.weights[i] );
      x *= data.lacunarity;
      y *= data.lacunarity;
    }
    
    return value;
  }
  
  NoiseVal FBMValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z )
  {
    ASSERT( data.Noise3 );
    
    NoiseVal value = 0.0f;
    
    for( int i = 0; i < data.octaves; ++i )
    {
      value += ( data.Noise3( x, y, z ) * data.weights[i] );
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
    }
    
    return value;
  }
  
  NoiseVal FBMValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w )
  {
    ASSERT( data.Noise4 );
    
    NoiseVal value = 0.0f;
    
    for( int i = 0; i < data.octaves; ++i )
    {
      value += ( data.Noise4( x, y, z, w ) * data.weights[i] );
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
      w *= data.lacunarity;
    }
    
    return value;
  }
  
} // Noise namespace
} // Utilities namespace
} // SF namespace
