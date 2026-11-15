/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TTVector4.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 12/23/2008 @ 21:39:08 - nwilliam
**  \brief   A 4D vector class
*******************************************************************************/

#ifndef SF_VECTOR_4_H
#define SF_VECTOR_4_H

namespace SF {
namespace Math
{
  template< typename T >
  struct TVector4
  {
    typedef T Type;
    
    // Constructors
    TVector4();
    TVector4( T _x, T _y, T _z, T _w = 0 );
    
    // Default copy, assignment ok
    
    // Unary negation
    TVector4 operator-() const;
    
    // Scalar operations
    TVector4& operator/=( T rhs );
    TVector4& operator*=( T rhs );
    TVector4 operator/( T rhs ) const;
    TVector4 operator*( T rhs ) const;
    
    // Vector operations
    T operator*( const TVector4& rhs) const;
    TVector4& operator-=( const TVector4& rhs );
    TVector4& operator+=( const TVector4& rhs );
    bool operator==( const TVector4& rhs ) const;
    bool operator!=( const TVector4& rhs ) const;
    TVector4 operator-( const TVector4& rhs ) const;
    TVector4 operator+( const TVector4& rhs ) const;
    
    // Normalization
    TVector4& Normalize();
    TVector4 Normalized() const;
    
    // Magnitude
    T Magnitude() const;
    T MagnitudeSquared() const;
    
    // Helpers / General
    
    // Data members
    T x;
    T y;
    T z;
    T w;
  };
  
  
  // "Default" TVector4
  typedef TVector4<float> Vector4;
  
} // Math namespace
} // SF namespace


#include "MiscMathFunctions.h" // fastSqrt_Q3

namespace SF {
namespace Math
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector4<T>::TVector4() : x( 0 ),
                            y( 0 ),
                            z( 0 ),
                            w( 0 )
  {
  }
  
  template< typename T >
  TVector4<T>::TVector4( T _x, T _y, T _z, T _w ) : x( _x ),
                                                    y( _y ),
                                                    z( _z ),
                                                    w( _w )
  {
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Unary negation
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector4<T> TVector4<T>::operator-() const
  {
    return TVector4<T>( -x, -y, -z, -w );
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Scalar operations
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector4<T>& TVector4<T>::operator/=( T rhs )
  {
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    
    return *this;
  }
  
  template< typename T >
  TVector4<T>& TVector4<T>::operator*=( T rhs )
  {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    
    return *this;
  }
  
  template< typename T >
  TVector4<T> TVector4<T>::operator/( T rhs ) const
  {
    return TVector4<T>( x / rhs, y / rhs, z / rhs, w / rhs );
  }
  
  template< typename T >
  TVector4<T> TVector4<T>::operator*( T rhs ) const
  {
    return TVector4<T>( x * rhs, y * rhs, z * rhs, w * rhs );
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Vector operations
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  T TVector4<T>::operator*( const TVector4<T>& rhs ) const
  {
    return ( x * rhs.x) + ( y * rhs.y) + ( z * rhs.z) + ( w * rhs.w );
  }
  
  template< typename T >
  TVector4<T>& TVector4<T>::operator-=( const TVector4<T>& rhs )
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    
    return *this;
  }
  
  template< typename T >
  TVector4<T>& TVector4<T>::operator+=( const TVector4<T>& rhs )
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    
    return *this;
  }
  
  template< typename T >
  bool TVector4<T>::operator==( const TVector4<T>& rhs ) const
  {
    return ( x == rhs.x ) && ( y == rhs.y ) && ( z == rhs.z ) && ( w == rhs.w );
  }
  
  template< typename T >
  bool TVector4<T>::operator!=( const TVector4<T>& rhs ) const
  {
    return !operator==( rhs );
  }
  
  template< typename T >
  TVector4<T> TVector4<T>::operator-( const TVector4<T>& rhs ) const
  {
    return TVector4<T>( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w );
  }
  
  template< typename T >
  TVector4<T> TVector4<T>::operator+( const TVector4<T>& rhs ) const
  {
    return TVector4<T>( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Normalization
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector4<T>& TVector4<T>::Normalize()
  {
    T m = Magnitude();
    
    x /= m;
    y /= m;
    z /= m;
    w /= m;
    
    return *this;
  }
  
  template< typename T >
  TVector4<T> TVector4<T>::Normalized() const
  {
    TVector4<T> v( *this );
    v.Normalize();
    
    return v;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Magnitude
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  T TVector4<T>::Magnitude() const
  {
    return fastSqrt_Q3( MagnitudeSquared() );
  }
  
  template< typename T >
  T TVector4<T>::MagnitudeSquared() const
  {
    return ( x * x ) + ( y * y ) + ( z * z ) + ( w * w );
  }
  
} // Math namespace
} // SF namespace


#endif // SF_T_VECTOR_4_H
