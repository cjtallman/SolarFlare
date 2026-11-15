/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RidgedMultiFractal.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/20/2008 @ 21:36:16 - nwilliam
**  \brief   Implementation of the ridged multi-fractal noise generator.
*******************************************************************************/

#include <cmath>
#include "Utilities/Misc.h"
#include "Utilities/Asserts.h"
#include "RidgedMultiFractal.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  //int RidgedMultiFractal::ParameterFlags() const
  //{
  //  return PF_GAIN | PF_HURST_INDEX | PF_LACUNARITY | PF_OCTAVES | PF_OFFSET;
  //}
  
  NoiseVal RMFValue1D( const NoiseData& data, NoiseVal x )
  {
    ASSERT( data.Noise1 );
    
    NoiseVal signal = data.offset - std::fabs( data.Noise1( x ) );
    signal *= signal;
    
    NoiseVal value = signal;
    NoiseVal weight = 1.0f;
    
    for( int i = 1; i < data.octaves; ++i )
    {
      x *= data.lacunarity;
      
      weight = Clamp( signal * data.gain, 0.0f, 1.0f );
      
      signal = data.offset - std::fabs( data.Noise1( x ) );
      signal *= signal;
      signal *= weight;
      
      value += ( signal * data.weights[i] );
    }
    
    return value;
  }
  
  NoiseVal RMFValue2D( const NoiseData& data, NoiseVal x, NoiseVal y )
  {
    ASSERT( data.Noise2 );
    
    NoiseVal signal = data.offset - std::fabs( data.Noise2( x, y ) );
    signal *= signal;
    
    NoiseVal value = signal;
    NoiseVal weight = 1.0f;
    
    for( int i = 1; i < data.octaves; ++i )
    {
      x *= data.lacunarity;
      y *= data.lacunarity;
      
      weight = Clamp( signal * data.gain, 0.0f, 1.0f );
      
      signal = data.offset - std::fabs( data.Noise2( x, y ) );
      signal *= signal;
      signal *= weight;
      
      value += ( signal * data.weights[i] );
    }
    
    return value;
  }
  
  NoiseVal RMFValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z )
  {
    ASSERT( data.Noise3 );
    
    NoiseVal signal = data.offset - std::fabs( data.Noise3( x, y, z ) );
    signal *= signal;
    
    NoiseVal value = signal;
    NoiseVal weight = 1.0f;
    
    for( int i = 1; i < data.octaves; ++i )
    {
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
      
      weight = Clamp( signal * data.gain, 0.0f, 1.0f );
      
      signal = data.offset - std::fabs( data.Noise3( x, y, z ) );
      signal *= signal;
      signal *= weight;
      
      value += ( signal * data.weights[i] );
    }
    
    return value;
  }
  
  
  NoiseVal RMFValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w )
  {
    ASSERT( data.Noise4 );
    
    NoiseVal signal = data.offset - std::fabs( data.Noise4( x, y, z, w ) );
    signal *= signal;
    
    NoiseVal value = signal;
    NoiseVal weight = 1.0f;
    
    for( int i = 1; i < data.octaves; ++i )
    {
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
      w *= data.lacunarity;
      
      weight = Clamp( signal * data.gain, 0.0f, 1.0f );
      
      signal = data.offset - std::fabs( data.Noise4( x, y, z, w ) );
      signal *= signal;
      signal *= weight;
      
      value += ( signal * data.weights[i] );
    }
    
    return value;
  }
  
} // Noise namespace
} // Utilities namespace
} // SF namespace
