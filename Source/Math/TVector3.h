/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TTVector3.h
**  \author  Patrick Mera, Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 12/23/2008 @ 21:39:18 - nwilliam
**  \brief   A 3D vector class.
*******************************************************************************/

#ifndef SF_T_VECTOR_3_H
#define SF_T_VECTOR_3_H

#include "Networking/ByteStream.h"

namespace SF {
namespace Math
{
  // Forward declaration
  struct Quaternion;

  template <typename T>
  struct TPoint3;

  template< typename T >
  struct TVector3
  {
    typedef T Type;

    // Constructors
    TVector3();
    TVector3( T _x, T _y, T _z = 0 );

    // Default copy, assignment ok

    // Unary negation
    TVector3 operator-() const;

    // Scalar operations
    TVector3& operator/=( T rhs );
    TVector3& operator*=( T rhs );
    TVector3 operator/( T rhs ) const;
    TVector3 operator*( T rhs ) const;

    // Vector operations
    T operator*( const TVector3& rhs) const;
    TVector3& operator-=( const TVector3& rhs );
    TVector3& operator+=( const TVector3& rhs );
    bool operator==( const TVector3& rhs ) const;
    bool operator!=( const TVector3& rhs ) const;
    TVector3 operator-( const TVector3& rhs ) const;
    TVector3 operator+( const TVector3& rhs ) const;

    // Normalization
    TVector3& Normalize();
    TVector3 Normalized() const;

    // Magnitude
    T Magnitude() const;
    T MagnitudeSquared() const;

    // Helpers / General
    void Rotate( const Quaternion& Q );
    void RotateInverse( const Quaternion& Q );
    TVector3 Cross( const TVector3& rhs )  const;

    // Friend operations
    template< typename U >
    friend TVector3<U> operator*( float lhs, const TVector3<U>& rhs );

    // Data members
    T x;
    T y;
    T z;
  };


  // "Default" TVector3
  typedef TVector3<float> Vector3;

  template< typename T >
  Networking::ByteStream& operator>>( Networking::ByteStream& bs, TVector3<T>& v );

  template< typename T >
  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const TVector3<T>& v );


} // Math namespace
} // SF namespace



#include "Quaternion.h"
#include "MiscMathFunctions.h" // fastSqrt_Q3

namespace SF {
namespace Math
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector3<T>::TVector3() : x( 0 ),
                            y( 0 ),
                            z( 0 )
  {
  }

  template< typename T >
  TVector3<T>::TVector3( T _x, T _y, T _z ) : x( _x ),
                                              y( _y ),
                                              z( _z )
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // Unary negation
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector3<T> TVector3<T>::operator-() const
  {
    return TVector3<T>( -x, -y, -z );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Scalar operations
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector3<T>& TVector3<T>::operator/=( T rhs )
  {
    x /= rhs;
    y /= rhs;
    z /= rhs;

    return *this;
  }

  template< typename T >
  TVector3<T>& TVector3<T>::operator*=( T rhs )
  {
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
  }

  template< typename T >
  TVector3<T> TVector3<T>::operator/( T rhs ) const
  {
    return TVector3<T>( x / rhs, y / rhs, z / rhs );
  }

  template< typename T >
  TVector3<T> TVector3<T>::operator*( T rhs ) const
  {
    return TVector3<T>( x * rhs, y * rhs, z * rhs );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Vector operations
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  T TVector3<T>::operator*( const TVector3<T>& rhs ) const
  {
    return ( x * rhs.x) + ( y * rhs.y) + ( z * rhs.z);
  }

  template< typename T >
  TVector3<T>& TVector3<T>::operator-=( const TVector3<T>& rhs )
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
  }

  template< typename T >
  TVector3<T>& TVector3<T>::operator+=( const TVector3<T>& rhs )
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
  }

  template< typename T >
  bool TVector3<T>::operator==( const TVector3<T>& rhs ) const
  {
    return ( x == rhs.x ) && ( y == rhs.y ) && ( z == rhs.z );
  }

  template< typename T >
  bool TVector3<T>::operator!=( const TVector3<T>& rhs ) const
  {
    return !operator==( rhs );
  }

  template< typename T >
  TVector3<T> TVector3<T>::operator-( const TVector3<T>& rhs ) const
  {
    return TVector3<T>( x - rhs.x, y - rhs.y, z - rhs.z );
  }

  template< typename T >
  TVector3<T> TVector3<T>::operator+( const TVector3<T>& rhs ) const
  {
    return TVector3<T>( x + rhs.x, y + rhs.y, z + rhs.z );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Normalization
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector3<T>& TVector3<T>::Normalize()
  {
    T m = Magnitude();

    x /= m;
    y /= m;
    z /= m;

    return *this;
  }

  template< typename T >
  TVector3<T> TVector3<T>::Normalized() const
  {
    TVector3<T> v( *this );
    v.Normalize();

    return v;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Magnitude
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  T TVector3<T>::Magnitude() const
  {
    //return std::sqrt( MagnitudeSquared() );
    return fastSqrt_Q3( MagnitudeSquared() );
  }

  template< typename T >
  T TVector3<T>::MagnitudeSquared() const
  {
    return ( x * x ) + ( y * y ) + ( z * z );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Helpers / General
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector3<T> TVector3<T>::Cross( const TVector3<T>& rhs ) const
  {
    return TVector3<T>( ( y * rhs.z ) - ( z * rhs.y ),
                        ( z * rhs.x ) - ( x * rhs.z ),
                        ( x * rhs.y ) - ( y * rhs.x )
                       );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Friend operations
  //////////////////////////////////////////////////////////////////////////////
  template< typename U >
  TVector3<U> operator*( float lhs, const TVector3<U>& rhs )
  {
    return TVector3<U>( rhs.x * lhs, rhs.y * lhs, rhs.z * lhs );
  }


  template< typename T >
  Networking::ByteStream& operator>>( Networking::ByteStream& bs, TVector3<T>& v )
  {
    bs >> v.x >> v.y >> v.z;
    return bs;
  }

  template< typename T >
  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const TVector3<T>& v )
  {
    bs << v.x << v.y << v.z;
    return bs;
  }


} // Math namespace
} // SF namespace


#endif // SF_T_VECTOR_3_H
