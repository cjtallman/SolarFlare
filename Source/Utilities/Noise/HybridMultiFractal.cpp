/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    HybridMultiFractal.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/20/2008 @ 21:37:04 - nwilliam
**  \brief   Implementation of the hybrid multi-fractal noise generator
*******************************************************************************/

#include "Utilities/Asserts.h"
#include "HybridMultiFractal.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  //int HybridMultiFractal::ParameterFlags() const
  //{
  //  return PF_HURST_INDEX | PF_LACUNARITY | PF_OCTAVES | PF_OFFSET;
  //}
  
  NoiseVal HMFValue1D( const NoiseData& data, NoiseVal x )
  {
    ASSERT( data.Noise1 );
    
    NoiseVal value = ( data.Noise1( x ) + data.offset );
    NoiseVal weight = data.gain * value;
    
    for( int i = 1; i < data.octaves; ++i )
    {
      if( weight > 1.0f )
      {
        weight = 1.0f;
      }
      
      x *= data.lacunarity;
      
      NoiseVal signal = ( data.Noise1( x ) + data.offset ) * data.weights[i];
      value += ( weight * signal );
      weight *= ( data.gain * signal );
    }
    
    return value;
  }
  
  NoiseVal HMFValue2D( const NoiseData& data, NoiseVal x, NoiseVal y )
  {
    ASSERT( data.Noise2 );
    
    NoiseVal value = ( data.Noise2( x, y ) + data.offset );
    NoiseVal weight = data.gain * value;
    
    for( int i = 1; i < data.octaves; ++i )
    {
      if( weight > 1.0f )
      {
        weight = 1.0f;
      }
      
      x *= data.lacunarity;
      y *= data.lacunarity;
      
      NoiseVal signal = ( data.Noise2( x, y ) + data.offset ) * data.weights[i];
      value += ( weight * signal );
      weight *= ( data.gain * signal );
    }
    
    return value;
  }
  
  NoiseVal HMFValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z )
  {
    ASSERT( data.Noise3 );
    
    NoiseVal value = ( data.Noise3( x, y, z ) + data.offset );
    NoiseVal weight = data.gain * value;
    
    for( int i = 1; i < data.octaves; ++i )
    {
      if( weight > 1.0f )
      {
        weight = 1.0f;
      }
      
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
      
      NoiseVal signal = ( data.Noise3( x, y, z ) + data.offset ) * data.weights[i];
      value += ( weight * signal );
      weight *= ( data.gain * signal );
    }
    
    return value;
  }
  
  NoiseVal HMFValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w )
  {
    ASSERT( data.Noise4 );
    
    NoiseVal value = ( data.Noise4( x, y, z, w ) + data.offset );
    NoiseVal weight = data.gain * value;
    
    for( int i = 1; i < data.octaves; ++i )
    {
      if( weight > 1.0f )
      {
        weight = 1.0f;
      }
      
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
      w *= data.lacunarity;
      
      NoiseVal signal = ( data.Noise4( x, y, z, w ) + data.offset ) * data.weights[i];
      value += ( weight * signal );
      weight *= ( data.gain * signal );
    }
    
    return value;
  }
  
} // Noise namespace
} // Utilities namespace
} // SF namespace
