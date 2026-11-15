/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Simplex.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:47:07 - nwilliam
**  \brief   Implementation of Perlin's lesser known simplex noise algorithm.
*******************************************************************************/

#include "Perlin.h"
#include "Simplex.h"

namespace
{
  static unsigned char simplex[64][4] = 
  {
    {0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
    {0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
    {1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
    {2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}
  };
}


namespace SF {
namespace Utilities {
namespace Noise
{
  using namespace PerlinPrivate;
  
  //int Simplex::ParameterFlags() const
  //{
  //  return PF_FREQUENCY | PF_PERSISTENCE | PF_LACUNARITY;
  //}
  
  NoiseVal SimplexValue1D( const NoiseData& data, NoiseVal x )
  {
    NoiseVal value = 0.0f;
    NoiseVal persistence = 1.0f;
    
    x *= data.frequency;
    
    for( int i = 0; i < data.octaves; i++)
    {
      // TODO: Test with/without seed. Still not sure on it
      //seed = (int)( (uint)(m_seed + i) & 0xFFFFFFFF );
      value += ( SimplexNoise1D( x ) * persistence );
      
      x *= data.lacunarity;
      persistence *= data.persistence;
    }
    
    return value;
  }
  
  NoiseVal SimplexValue2D( const NoiseData& data, NoiseVal x, NoiseVal y )
  {
    NoiseVal value = 0.0f;
    NoiseVal persistence = 1.0f;
    
    x *= data.frequency;
    y *= data.frequency;
    
    for( int i = 0; i < data.octaves; i++ )
    {
      // TODO: Test with/without seed. Still not sure on it
      //seed = (int)( (uint)( m_seed + i ) & 0xFFFFFFFF );
      value += ( SimplexNoise2D( x, y ) * persistence );
      
      x *= data.lacunarity;
      y *= data.lacunarity;
      persistence *= data.persistence;
    }
    
    return value;
  }
  
  NoiseVal SimplexValue3D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z )
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
      value += ( SimplexNoise3D( x, y, z ) * persistence );
      
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
      persistence *= data.persistence;
    }
    
    // Move output range to very near [-1,1]
    return value;
  }
  
  NoiseVal SimplexValue4D( const NoiseData& data, NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w )
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
      value += ( SimplexNoise4D( x, y, z, w ) * persistence );
      
      x *= data.lacunarity;
      y *= data.lacunarity;
      z *= data.lacunarity;
      w *= data.lacunarity;
      persistence *= data.persistence;
    }
    
    return value;
  }
  
  NoiseVal SimplexNoise1D( NoiseVal x )
  {
    int i0 = FastFloor(x);
    int i1 = i0 + 1;
    NoiseVal x0 = x - i0;
    NoiseVal x1 = x0 - 1.0f;
    
    NoiseVal n0, n1;
    
    NoiseVal t0 = 1.0f - x0*x0;
    t0 *= t0;
    n0 = t0 * t0 * Gradient( perms[i0 & 0xff], x0 );
    
    NoiseVal t1 = 1.0f - x1*x1;
    t1 *= t1;
    n1 = t1 * t1 * Gradient( perms[i1 & 0xff], x1 );
    
    return 0.395f * (n0 + n1);
  }
  
  NoiseVal SimplexNoise2D( NoiseVal x, NoiseVal y )
  {
    const NoiseVal F2 = 0.366025403f; // F2 = 0.5*(sqrt(3.0)-1.0)
    const NoiseVal G2 = 0.211324865f; // G2 = (3.0-Math.sqrt(3.0))/6.0
    
    NoiseVal n0, n1, n2; // Noise contributions from the three corners
    
    // Skew the input space to determine which simplex cell we're in
    NoiseVal s = (x+y)*F2; // Hairy factor for 2D
    NoiseVal xs = x + s;
    NoiseVal ys = y + s;
    int i = FastFloor(xs);
    int j = FastFloor(ys);
    
    NoiseVal t = (NoiseVal)(i+j)*G2;
    NoiseVal X0 = i-t; // Unskew the cell origin back to (x,y) space
    NoiseVal Y0 = j-t;
    NoiseVal x0 = x-X0; // The x,y distances from the cell origin
    NoiseVal y0 = y-Y0;
    
    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if( x0 > y0 )
    {
      i1 = 1; // lower triangle, XY order: (0,0)->(1,0)->(1,1)
      j1 = 0;
    }
    else
    {
      i1=0; // upper triangle, YX order: (0,0)->(0,1)->(1,1)
      j1=1;
    }
    
    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6
    
    NoiseVal x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    NoiseVal y1 = y0 - j1 + G2;
    NoiseVal x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
    NoiseVal y2 = y0 - 1.0f + 2.0f * G2;
    
    // Wrap the integer indices at 256, to avoid indexing perms[] out of bounds
    int ii = i & 0xff;
    int jj = j & 0xff;
    
    // Calculate the contribution from the three corners
    NoiseVal t0 = 0.5f - x0*x0-y0*y0;
    if( t0 < 0.0f )
    {
      n0 = 0.0f;
    }
    else
    {
      t0 *= t0;
      n0 = t0 * t0 * Gradient( perms[ ii + perms[jj] ], x0, y0 );
    }
    
    NoiseVal t1 = 0.5f - x1*x1-y1*y1;
    if( t1 < 0.0f )
    {
      n1 = 0.0f;
    }
    else
    {
      t1 *= t1;
      n1 = t1 * t1 * Gradient( perms[ ii + i1 + perms[jj+j1] ], x1, y1 );
    }
    
    NoiseVal t2 = 0.5f - x2*x2-y2*y2;
    
    if(t2 < 0.0f)
    {
      n2 = 0.0f;
    }
    else
    {
      t2 *= t2;
      n2 = t2 * t2 * Gradient( perms[ ii + 1 + perms[jj+1] ], x2, y2 );
    }
    
    return 40.0f * (n0 + n1 + n2);
  }
  
  NoiseVal SimplexNoise3D( NoiseVal x, NoiseVal y, NoiseVal z )
  {
    // Simple skewing factors for the 3D case
    const NoiseVal F3 = 0.333333333f;
    const NoiseVal G3 = 0.166666667f;
    
    NoiseVal n0, n1, n2, n3; // Noise contributions from the four corners
    
    // Skew the input space to determine which simplex cell we're in
    NoiseVal s = (x + y + z) * F3; // Very nice and simple skew factor for 3D
    NoiseVal xs = x + s;
    NoiseVal ys = y + s;
    NoiseVal zs = z + s;
    int i = FastFloor( xs );
    int j = FastFloor( ys );
    int k = FastFloor( zs );
    
    NoiseVal t = static_cast<NoiseVal>( i + j + k ) * G3;
    NoiseVal X0 = i - t; // Unskew the cell origin back to (x,y,z) space
    NoiseVal Y0 = j - t;
    NoiseVal Z0 = k - t;
    NoiseVal x0 = x - X0; // The x,y,z distances from the cell origin
    NoiseVal y0 = y - Y0;
    NoiseVal z0 = z - Z0;
    
    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // Determine which simplex we are in.
    int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
    int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords
    
    if( x0 >= y0 )
    {
      if( y0 >= z0 ) // X Y Z order
      {
        i1=1;
        j1=0;
        k1=0;
        i2=1;
        j2=1;
        k2=0;
      }
      else if( x0 >= z0 ) // X Z Y order
      {
        i1=1;
        j1=0;
        k1=0;
        i2=1;
        j2=0;
        k2=1;
      }
      else // Z X Y order
      {
        i1=0;
        j1=0;
        k1=1;
        i2=1;
        j2=0;
        k2=1;
      }
    }
    else // x0<y0
    {
      if( y0 < z0 ) // Z Y X order
      {
        i1=0;
        j1=0;
        k1=1;
        i2=0;
        j2=1;
        k2=1;
      }
      else if( x0 < z0 ) // Y Z X order
      {
        i1=0;
        j1=1;
        k1=0;
        i2=0;
        j2=1;
        k2=1;
      }
      else // Y X Z order
      {
        i1=0;
        j1=1;
        k1=0;
        i2=1;
        j2=1;
        k2=0;
      }
    }
    
    // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
    // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
    // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
    // c = 1/6.
    
    NoiseVal x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
    NoiseVal y1 = y0 - j1 + G3;
    NoiseVal z1 = z0 - k1 + G3;
    NoiseVal x2 = x0 - i2 + 2.0f*G3; // Offsets for third corner in (x,y,z) coords
    NoiseVal y2 = y0 - j2 + 2.0f*G3;
    NoiseVal z2 = z0 - k2 + 2.0f*G3;
    NoiseVal x3 = x0 - 1.0f + 3.0f*G3; // Offsets for last corner in (x,y,z) coords
    NoiseVal y3 = y0 - 1.0f + 3.0f*G3;
    NoiseVal z3 = z0 - 1.0f + 3.0f*G3;
    
    // Wrap the integer indices at 256, to avoid indexing perms[] out of bounds
    int ii = i & 0xff;
    int jj = j & 0xff;
    int kk = k & 0xff;
    
    // Calculate the contribution from the four corners
    NoiseVal t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
    
    if( t0 < 0.0f )
    {
      n0 = 0.0f;
    }
    else
    {
      t0 *= t0;
      n0 = t0 * t0 * Gradient( perms[ ii + perms[ jj + perms[kk] ] ], x0, y0, z0 );
    }
    
    NoiseVal t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
    
    if( t1 < 0.0f )
    {
      n1 = 0.0f;
    }
    else
    {
      t1 *= t1;
      n1 = t1 * t1 * Gradient( perms[ ii + i1 + perms[ jj + j1 + perms[ kk + k1 ] ] ], x1, y1, z1);
    }
    
    NoiseVal t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
    
    if( t2 < 0.0f )
    {
      n2 = 0.0f;
    }
    else
    {
      t2 *= t2;
      n2 = t2 * t2 * Gradient( perms[ ii + i2 + perms[ jj + j2 + perms[ kk + k2 ] ] ], x2, y2, z2);
    }
    
    NoiseVal t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
    
    if( t3 < 0.0f )
    {
      n3 = 0.0f;
    }
    else
    {
      t3 *= t3;
      n3 = t3 * t3 * Gradient( perms[ ii + 1 + perms[ jj + 1 + perms[ kk + 1 ] ] ], x3, y3, z3);
    }
    
    return 32.0f * (n0 + n1 + n2 + n3);
  }
  
  NoiseVal SimplexNoise4D(NoiseVal x, NoiseVal y, NoiseVal z, NoiseVal w )
  {
    const float F4 = 0.309016994f; // F4 = (Math.sqrt(5.0)-1.0)/4.0
    const float G4 = 0.138196601f; // G4 = (5.0-Math.sqrt(5.0))/20.0
    
    NoiseVal n0, n1, n2, n3, n4; // Noise contributions from the five corners
    
    // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
    NoiseVal s = (x + y + z + w) * F4; // Factor for 4D skewing
    NoiseVal xs = x + s;
    NoiseVal ys = y + s;
    NoiseVal zs = z + s;
    NoiseVal ws = w + s;
    int i = FastFloor( xs );
    int j = FastFloor( ys );
    int k = FastFloor( zs );
    int l = FastFloor( ws );
    
    NoiseVal t = (i + j + k + l) * G4; // Factor for 4D unskewing
    NoiseVal X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
    NoiseVal Y0 = j - t;
    NoiseVal Z0 = k - t;
    NoiseVal W0 = l - t;
    
    NoiseVal x0 = x - X0;  // The x,y,z,w distances from the cell origin
    NoiseVal y0 = y - Y0;
    NoiseVal z0 = z - Z0;
    NoiseVal w0 = w - W0;
    
    // To find out which of the 24 possible simplices we're in, we need to
    // determine the magnitude ordering of x0, y0, z0 and w0.
    // The method below is a good way of finding the ordering of x,y,z,w and
    // then find the correct traversal order for the simplex we're in.
    // First, six pair-wise comparisons are performed between each possible pair
    // of the four coordinates, and the results are used to add up binary bits
    // for an integer index.
    int c1 = (x0 > y0) ? 32 : 0;
    int c2 = (x0 > z0) ? 16 : 0;
    int c3 = (y0 > z0) ? 8 : 0;
    int c4 = (x0 > w0) ? 4 : 0;
    int c5 = (y0 > w0) ? 2 : 0;
    int c6 = (z0 > w0) ? 1 : 0;
    int c = c1 + c2 + c3 + c4 + c5 + c6;
    
    int i1, j1, k1, l1; // The integer offsets for the second simplex corner
    int i2, j2, k2, l2; // The integer offsets for the third simplex corner
    int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner
    
    // simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
    // Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
    // impossible. Only the 24 indices which have non-zero entries make any sense.
    // We use a thresholding to set the coordinates in turn from the largest magnitude.
    // The number 3 in the "simplex" array is at the position of the largest coordinate.
    i1 = simplex[c][0] >= 3 ? 1 : 0;
    j1 = simplex[c][1] >= 3 ? 1 : 0;
    k1 = simplex[c][2] >= 3 ? 1 : 0;
    l1 = simplex[c][3] >= 3 ? 1 : 0;
    
    // The number 2 in the "simplex" array is at the second largest coordinate.
    i2 = simplex[c][0] >= 2 ? 1 : 0;
    j2 = simplex[c][1] >= 2 ? 1 : 0;
    k2 = simplex[c][2] >= 2 ? 1 : 0;
    l2 = simplex[c][3] >= 2 ? 1 : 0;
    
    // The number 1 in the "simplex" array is at the second smallest coordinate.
    i3 = simplex[c][0] >= 1 ? 1 : 0;
    j3 = simplex[c][1] >= 1 ? 1 : 0;
    k3 = simplex[c][2] >= 1 ? 1 : 0;
    l3 = simplex[c][3] >= 1 ? 1 : 0;
    
    // The fifth corner has all coordinate offsets = 1, so no need to look that up.
    
    NoiseVal x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
    NoiseVal y1 = y0 - j1 + G4;
    NoiseVal z1 = z0 - k1 + G4;
    NoiseVal w1 = w0 - l1 + G4;
    NoiseVal x2 = x0 - i2 + 2.0f*G4; // Offsets for third corner in (x,y,z,w) coords
    NoiseVal y2 = y0 - j2 + 2.0f*G4;
    NoiseVal z2 = z0 - k2 + 2.0f*G4;
    NoiseVal w2 = w0 - l2 + 2.0f*G4;
    NoiseVal x3 = x0 - i3 + 3.0f*G4; // Offsets for fourth corner in (x,y,z,w) coords
    NoiseVal y3 = y0 - j3 + 3.0f*G4;
    NoiseVal z3 = z0 - k3 + 3.0f*G4;
    NoiseVal w3 = w0 - l3 + 3.0f*G4;
    NoiseVal x4 = x0 - 1.0f + 4.0f*G4; // Offsets for last corner in (x,y,z,w) coords
    NoiseVal y4 = y0 - 1.0f + 4.0f*G4;
    NoiseVal z4 = z0 - 1.0f + 4.0f*G4;
    NoiseVal w4 = w0 - 1.0f + 4.0f*G4;
    
    // Wrap the integer indices at 256, to avoid indexing perms[] out of bounds
    int ii = i & 0xff;
    int jj = j & 0xff;
    int kk = k & 0xff;
    int ll = l & 0xff;
    
    // Calculate the contribution from the five corners
    NoiseVal t0 = 0.6f - x0*x0 - y0*y0 - z0*z0 - w0*w0;
    
    if( t0 < 0.0f )
    {
      n0 = 0.0f;
    }
    else
    {
      t0 *= t0;
      n0 = t0 * t0 * Gradient( perms[ ii + perms[ jj + perms[ kk + perms[ll] ] ] ], x0, y0, z0, w0);
    }
    
    NoiseVal t1 = 0.6f - x1*x1 - y1*y1 - z1*z1 - w1*w1;
    
    if( t1 < 0.0f )
    {
      n1 = 0.0f;
    }
    else
    {
      t1 *= t1;
      n1 = t1 * t1 * Gradient( perms[ ii + i1 + perms[ jj + j1 + perms[ kk + k1 + perms[ ll + l1 ] ] ] ], x1, y1, z1, w1);
    }
    
    NoiseVal t2 = 0.6f - x2*x2 - y2*y2 - z2*z2 - w2*w2;
    
    if( t2 < 0.0f )
    {
      n2 = 0.0f;
    }
    else
    {
      t2 *= t2;
      n2 = t2 * t2 * Gradient( perms[ii+i2+perms[jj+j2+perms[kk+k2+perms[ll+l2]]]], x2, y2, z2, w2);
    }
    
    NoiseVal t3 = 0.6f - x3*x3 - y3*y3 - z3*z3 - w3*w3;
    
    if( t3 < 0.0f )
    {
      n3 = 0.0f;
    }
    else
    {
      t3 *= t3;
      n3 = t3 * t3 * Gradient( perms[ ii + i3 + perms[ jj + j3 + perms[ kk + k3 + perms[ ll + l3 ] ] ] ], x3, y3, z3, w3 );
    }
    
    NoiseVal t4 = 0.6f - x4*x4 - y4*y4 - z4*z4 - w4*w4;
    
    if( t4 < 0.0f )
    {
      n4 = 0.0f;
    }
    else
    {
      t4 *= t4;
      n4 = t4 * t4 * Gradient( perms[ ii + 1 + perms[ jj + 1 + perms[ kk + 1 + perms[ ll + 1 ] ] ] ], x4, y4, z4, w4);
    }
    
    return 27.0f * (n0 + n1 + n2 + n3 + n4);
  }
  
} // Noise namespace
} // Utilities namespace
} // SF namespace
