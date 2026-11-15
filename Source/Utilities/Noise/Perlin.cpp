/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Perlin.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/20/2008 @ 21:35:59 - nwilliam
**  \brief   Implementation of the "improved noise"
*******************************************************************************/

#include "Perlin.h"
#include "Math/MiscMathFunctions.h"

//int ParameterFlags() const
//  {
//    return PF_FREQUENCY | PF_LACUNARITY | PF_OCTAVES | PF_PERSISTENCE;
//  }

namespace SF {
namespace Utilities {
namespace Noise
{
  using namespace PerlinPrivate;
  
  NoiseVal PerlinValue1D( const NoiseData& data, NoiseVal x )
  {
    NoiseVal value = 0.0f;
    NoiseVal persistence = 1.0f;
    
    x *= data.frequency;
    
    for( int i = 0; i < data.octaves; i++)
    {
      // TODO: Test with/without seed. Still not sure on it
      //seed = (int)( (uint)(m_seed + i) & 0xFFFFFFFF );
      value += ( PerlinNoise1D( x ) * persistence );
      
      x *= data.lacunarity;
      persistence *= data.persistence;
    }
    
    return value;
  }
  
  NoiseVal PerlinValue2D( const NoiseData& data, NoiseVal x, NoiseVal y )
  {
    NoiseVal value = 0.0f;
    NoiseVal persistence = 1.0f;
    
    x *= data.frequency;
    y *= data.frequency;
    
    for( int i = 0; i < data.octaves; i++ )
    {
      // TODO: Test with/without seed. Still not sure on it
      //seed = (int)( (uint)( m_seed + i ) & 0xFFFFFFFF );
      value += PerlinNoise2D( x, y ) * persistence;
      
      x *= data.lacunarity;
      y *= data.lacunarity;
      persistence *= data.persistence;
    }
    
    return value;
  }
  
  NoiseVal PerlinValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z )
  {
    NoiseVal value = 0.0f;
    NoiseVal persistence = 1.0f;
    
    x *= data.frequency;
    y *= data.frequency;
    z *= data.frequency;
    
    for( int i = 0; i < data.octaves; i++)
    {
      // TODO: Test with/without seed. Still not sure on it
      //seed = (int)( (uint)(m_seed + i) & 0xFFFFFFFF );
      value += ( PerlinNoise3D( x, y, z ) * persistence );
      
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
      persistence *= data.persistence;
    }
    
    return value;
  }
  
  NoiseVal PerlinValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w )
  {
    NoiseVal value = 0.0f;
    NoiseVal persistence = 1.0f;
    
    x *= data.frequency;
    y *= data.frequency;
    z *= data.frequency;
    w *= data.frequency;
    
    for( int i = 0; i < data.octaves; i++ )
    {
      // TODO: Test with/without seed. Still not sure on it
      //seed = (int)( (uint)( m_seed + i ) & 0xFFFFFFFF );
      value += ( PerlinNoise4D( x, y, z, w ) * persistence );
      
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
      w *= data.lacunarity;
      persistence *= data.persistence;
    }
    
    return value;
  }
  
  NoiseVal PerlinNoise1D( NoiseVal x )
  {
    int ix0, ix1;
    NoiseVal fx0, fx1;
    NoiseVal s, n0, n1;
    
    ix0 = FastFloor( x ); // Integer part of x
    fx0 = x - ix0;        // Fractional part of x
    fx1 = fx0 - 1.0f;
    ix1 = ( ix0 + 1 ) & 0xFF;
    ix0 = ix0 & 0xff;    // Wrap to 0..255
    
    s = SCurveQuintic( fx0 );
    
    n0 = Gradient( perms[ix0], fx0 );
    n1 = Gradient( perms[ix1], fx1 );
    
    return Lerp( s, n0, n1 );
  }
  
  NoiseVal PerlinNoise2D( NoiseVal x, NoiseVal y )
  {
    int ix0, iy0, ix1, iy1;
    NoiseVal fx0, fy0, fx1, fy1;
    NoiseVal s, t, nx0, nx1, n0, n1;
    
    ix0 = FastFloor( x ); // Integer part of x
    iy0 = FastFloor( y ); // Integer part of y
    fx0 = x - ix0;        // Fractional part of x
    fy0 = y - iy0;        // Fractional part of y
    fx1 = fx0 - 1.0f;
    fy1 = fy0 - 1.0f;
    ix1 = ( ix0 + 1 ) & 0xFF;  // Wrap to 0..255
    iy1 = ( iy0 + 1 ) & 0xFF;
    ix0 = ix0 & 0xFF;
    iy0 = iy0 & 0xFF;
    
    t = SCurveQuintic( fy0 );
    s = SCurveQuintic( fx0 );
    
    nx0 = Gradient( perms[ix0 + perms[iy0]], fx0, fy0 );
    nx1 = Gradient( perms[ix0 + perms[iy1]], fx0, fy1 );
    n0 = Lerp( t, nx0, nx1 );
    
    nx0 = Gradient( perms[ix1 + perms[iy0]], fx1, fy0 );
    nx1 = Gradient( perms[ix1 + perms[iy1]], fx1, fy1 );
    n1 = Lerp( t, nx0, nx1 );
    
    return Lerp( s, n0, n1 );
  }
  
  NoiseVal PerlinNoise3D( NoiseVal x, NoiseVal y, NoiseVal z )
  {
    int ix0, iy0, ix1, iy1, iz0, iz1;
    NoiseVal fx0, fy0, fz0, fx1, fy1, fz1;
    NoiseVal s, t, r;
    NoiseVal nxy0, nxy1, nx0, nx1, n0, n1;
    
    ix0 = FastFloor( x ); // Integer part of x
    iy0 = FastFloor( y ); // Integer part of y
    iz0 = FastFloor( z ); // Integer part of z
    fx0 = x - ix0;        // Fractional part of x
    fy0 = y - iy0;        // Fractional part of y
    fz0 = z - iz0;        // Fractional part of z
    fx1 = fx0 - 1.0f;
    fy1 = fy0 - 1.0f;
    fz1 = fz0 - 1.0f;
    ix1 = ( ix0 + 1 ) & 0xFF; // Wrap to 0..255
    iy1 = ( iy0 + 1 ) & 0xFF;
    iz1 = ( iz0 + 1 ) & 0xFF;
    ix0 = ix0 & 0xFF;
    iy0 = iy0 & 0xFF;
    iz0 = iz0 & 0xFF;
    
    r = SCurveQuintic( fz0 );
    t = SCurveQuintic( fy0 );
    s = SCurveQuintic( fx0 );
    
    nxy0 = Gradient( perms[ix0 + perms[iy0 + perms[iz0]]], fx0, fy0, fz0 );
    nxy1 = Gradient( perms[ix0 + perms[iy0 + perms[iz1]]], fx0, fy0, fz1 );
    nx0 = Lerp( r, nxy0, nxy1 );
    
    nxy0 = Gradient( perms[ix0 + perms[iy1 + perms[iz0]]], fx0, fy1, fz0 );
    nxy1 = Gradient( perms[ix0 + perms[iy1 + perms[iz1]]], fx0, fy1, fz1 );
    nx1 = Lerp( r, nxy0, nxy1 );
    
    n0 = Lerp( t, nx0, nx1 );
    
    nxy0 = Gradient( perms[ix1 + perms[iy0 + perms[iz0]]], fx1, fy0, fz0 );
    nxy1 = Gradient( perms[ix1 + perms[iy0 + perms[iz1]]], fx1, fy0, fz1 );
    nx0 = Lerp( r, nxy0, nxy1 );
    
    nxy0 = Gradient( perms[ix1 + perms[iy1 + perms[iz0]]], fx1, fy1, fz0 );
    nxy1 = Gradient( perms[ix1 + perms[iy1 + perms[iz1]]], fx1, fy1, fz1 );
    nx1 = Lerp( r, nxy0, nxy1 );
    
    n1 = Lerp( t, nx0, nx1 );
    
    return Lerp( s, n0, n1 );
  }
  
  NoiseVal PerlinNoise4D( NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w )
  {
    int ix0, iy0, iz0, iw0, ix1, iy1, iz1, iw1;
    NoiseVal fx0, fy0, fz0, fw0, fx1, fy1, fz1, fw1;
    NoiseVal s, t, r, q;
    NoiseVal nxyz0, nxyz1, nxy0, nxy1, nx0, nx1, n0, n1;
    
    ix0 = FastFloor( x ); // Integer part of x
    iy0 = FastFloor( y ); // Integer part of y
    iz0 = FastFloor( z ); // Integer part of y
    iw0 = FastFloor( w ); // Integer part of w
    fx0 = x - ix0;        // Fractional part of x
    fy0 = y - iy0;        // Fractional part of y
    fz0 = z - iz0;        // Fractional part of z
    fw0 = w - iw0;        // Fractional part of w
    fx1 = fx0 - 1.0f;
    fy1 = fy0 - 1.0f;
    fz1 = fz0 - 1.0f;
    fw1 = fw0 - 1.0f;
    ix1 = ( ix0 + 1 ) & 0xFF;  // Wrap to 0..255
    iy1 = ( iy0 + 1 ) & 0xFF;
    iz1 = ( iz0 + 1 ) & 0xFF;
    iw1 = ( iw0 + 1 ) & 0xFF;
    ix0 = ix0 & 0xFF;
    iy0 = iy0 & 0xFF;
    iz0 = iz0 & 0xFF;
    iw0 = iw0 & 0xFF;
    
    q = SCurveQuintic( fw0 );
    r = SCurveQuintic( fz0 );
    t = SCurveQuintic( fy0 );
    s = SCurveQuintic( fx0 );
    
    nxyz0 = Gradient( perms[ix0 + perms[iy0 + perms[iz0 + perms[iw0]]]], fx0, fy0, fz0, fw0 );
    nxyz1 = Gradient( perms[ix0 + perms[iy0 + perms[iz0 + perms[iw1]]]], fx0, fy0, fz0, fw1 );
    nxy0 = Lerp( q, nxyz0, nxyz1 );
    
    nxyz0 = Gradient( perms[ix0 + perms[iy0 + perms[iz1 + perms[iw0]]]], fx0, fy0, fz1, fw0 );
    nxyz1 = Gradient( perms[ix0 + perms[iy0 + perms[iz1 + perms[iw1]]]], fx0, fy0, fz1, fw1 );
    nxy1 = Lerp( q, nxyz0, nxyz1 );
    
    nx0 = Lerp( r, nxy0, nxy1 );
    
    nxyz0 = Gradient( perms[ix0 + perms[iy1 + perms[iz0 + perms[iw0]]]], fx0, fy1, fz0, fw0 );
    nxyz1 = Gradient( perms[ix0 + perms[iy1 + perms[iz0 + perms[iw1]]]], fx0, fy1, fz0, fw1 );
    nxy0 = Lerp( q, nxyz0, nxyz1 );
    
    nxyz0 = Gradient( perms[ix0 + perms[iy1 + perms[iz1 + perms[iw0]]]], fx0, fy1, fz1, fw0 );
    nxyz1 = Gradient( perms[ix0 + perms[iy1 + perms[iz1 + perms[iw1]]]], fx0, fy1, fz1, fw1 );
    nxy1 = Lerp( q, nxyz0, nxyz1 );
    
    nx1 = Lerp( r, nxy0, nxy1 );
    
    n0 = Lerp( t, nx0, nx1 );
    
    nxyz0 = Gradient( perms[ix1 + perms[iy0 + perms[iz0 + perms[iw0]]]], fx1, fy0, fz0, fw0 );
    nxyz1 = Gradient( perms[ix1 + perms[iy0 + perms[iz0 + perms[iw1]]]], fx1, fy0, fz0, fw1 );
    nxy0 = Lerp( q, nxyz0, nxyz1 );
    
    nxyz0 = Gradient( perms[ix1 + perms[iy0 + perms[iz1 + perms[iw0]]]], fx1, fy0, fz1, fw0 );
    nxyz1 = Gradient( perms[ix1 + perms[iy0 + perms[iz1 + perms[iw1]]]], fx1, fy0, fz1, fw1 );
    nxy1 = Lerp( q, nxyz0, nxyz1 );
    
    nx0 = Lerp( r, nxy0, nxy1 );
    
    nxyz0 = Gradient( perms[ix1 + perms[iy1 + perms[iz0 + perms[iw0]]]], fx1, fy1, fz0, fw0 );
    nxyz1 = Gradient( perms[ix1 + perms[iy1 + perms[iz0 + perms[iw1]]]], fx1, fy1, fz0, fw1 );
    nxy0 = Lerp( q, nxyz0, nxyz1 );
    
    nxyz0 = Gradient( perms[ix1 + perms[iy1 + perms[iz1 + perms[iw0]]]], fx1, fy1, fz1, fw0 );
    nxyz1 = Gradient( perms[ix1 + perms[iy1 + perms[iz1 + perms[iw1]]]], fx1, fy1, fz1, fw1 );
    nxy1 = Lerp( q, nxyz0, nxyz1 );
    
    nx1 = Lerp( r, nxy0, nxy1 );
    
    n1 = Lerp( t, nx0, nx1 );
    
    return Lerp( s, n0, n1 );
  }
  
  
  namespace PerlinPrivate
  {
    int FastFloor( NoiseVal x )
    {
      return ( x > 0 ) ? (int)x : (int)x-1;
    }
    
    NoiseVal Fade( NoiseVal t )
    {
      return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
    }
    
    NoiseVal Lerp( NoiseVal t, NoiseVal a, NoiseVal b )
    {
      return a + t * ( b - a );
    }
    
    NoiseVal Gradient( int hash, NoiseVal x )
    {
      int h = hash & 15;
      NoiseVal grad = 1.0f + ( h & 7 );  // Gradient value 1.0, 2.0, ..., 8.0
      
      if( h & 8 )
      {
        // Random sign for the gradient
        grad = -grad;
      }
      
      // Multiply the gradient with the distance
      return grad * x;
    }
    
    NoiseVal SCurveQuintic( NoiseVal t )
    {
      const NoiseVal t3 = t * t * t;
      const NoiseVal t4 = t3 * t;
      const NoiseVal t5 = t4 * t;
      
      return ( 6.0f * t5 ) - ( 15.0f * t4 ) + ( 10.0f * t3 );
    }
    
    NoiseVal Gradient( int hash, NoiseVal x, NoiseVal y )
    {
      int h = hash & 7;            // Convert low 3 bits of hash code
      NoiseVal u = h < 4 ? x : y;  // into 8 simple gradient directions,
      NoiseVal v = h < 4 ? y : x;  // and compute the dot product with (x,y).
      
      return ( ( h & 1 ) ? -u : u ) + ( ( h & 2 ) ? -2.0f * v : 2.0f * v );
    }
    
    NoiseVal Gradient( int hash, NoiseVal x, NoiseVal y , NoiseVal z )
    {
      int h = hash & 15;                                    // Convert low 4 bits of hash code into 12 simple
      NoiseVal u = h < 8 ? x : y;                           // gradient directions, and compute dot product.
      NoiseVal v = h < 4 ? y : h == 12 || h == 14 ? x : z;  // Fix repeats at h = 12 to 15
      
      return ( ( h & 1 ) ? -u : u) + ( ( h & 2 ) ? -v : v );
    }
    
    NoiseVal Gradient( int hash, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal t )
    {
      int h = hash & 31;            // Convert low 5 bits of hash code into 32 simple
      NoiseVal u = h < 24 ? x : y;  // gradient directions, and compute dot product.
      NoiseVal v = h < 16 ? y : z;
      NoiseVal w = h < 8 ? z : t;
      
      return ( ( h & 1 ) ? -u : u) + ( ( h & 2 ) ? -v : v ) + ( ( h & 4 ) ? -w : w );
    }
    
    unsigned char perms[512] =
    {
      151,160,137,91,90,15,
      131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
      190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
      88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
      77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
      102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
      135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
      5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
      223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
      129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
      251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
      49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
      138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
      151,160,137,91,90,15,
      131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
      190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
      88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
      77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
      102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
      135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
      5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
      223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
      129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
      251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
      49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
      138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };
    
  } // PerlinPrivate namespace
  
} // Noise namespace
} // Utilities namespace
} // SF namespace
