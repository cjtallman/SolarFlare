/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TTVector2.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 12/23/2008 @ 21:39:46 - nwilliam
**  \brief   A 2D vector class
*******************************************************************************/

#ifndef SF_T_VECTOR_2_H
#define SF_T_VECTOR_2_H

namespace SF {
namespace Math
{
  template< typename T >
  struct TVector2
  {
    typedef T Type;
    
    // Constructors
    TVector2();
    TVector2( T _x , T _y = 0 );
    
    // Default copy, assignment ok
    
    // Unary negation
    TVector2 operator-() const;
    
    // Scalar operations
    TVector2 operator/=( T rhs );
    TVector2 operator*=( T rhs );
    TVector2 operator/( T rhs ) const;
    TVector2 operator*( T rhs ) const;
    
    // Vector operations
    T operator*( const TVector2& rhs ) const;
    TVector2& operator-=( const TVector2& rhs );
    TVector2& operator+=( const TVector2& rhs );
    bool operator==( const TVector2& rhs ) const;
    bool operator!=( const TVector2& rhs ) const;
    TVector2 operator-( const TVector2& rhs ) const;
    TVector2 operator+( const TVector2& rhs ) const;
    
    // Normalization
    TVector2& Normalize();
    TVector2 Normalized() const;
    
    // Magnitude
    T Magnitude() const;
    T MagnitudeSquared() const;
    
    // Helpers / General
    void Negate();
    void Rotate(float angle);
    TVector2 Rotated(float angle) const;
     
    // Data members
    T x;
    T y;
  };
  
  
  // "Default" vector2
  typedef TVector2<float> Vector2;
  
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
  TVector2<T>::TVector2() : x( 0 ),
                            y( 0 )
  {
  }
  
  template< typename T >
  TVector2<T>::TVector2( T _x , T _y ) : x( _x ),
                                         y( _y )
  {
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Unary Negation
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector2<T> TVector2<T>::operator-() const
  {
    TVector2<T> neg( *this );
    neg.Negate();
    return neg;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Scalar operations
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector2<T> TVector2<T>::operator/=( T rhs )
  {
    x /= rhs;
    y /= rhs;
    return *this;
  }
  
  template< typename T >
  TVector2<T> TVector2<T>::operator*=( T rhs )
  {
    x *= rhs;
    y *= rhs;
    return *this;
  }
  
  template< typename T >
  TVector2<T> TVector2<T>::operator/( T rhs ) const
  {
    return TVector2<T>( x / rhs, y / rhs );
  }
  
  template< typename T >
  TVector2<T> TVector2<T>::operator*( T rhs ) const
  {
    return TVector2<T>( x * rhs, y * rhs );
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Vector operations
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  T TVector2<T>::operator*( const TVector2& rhs ) const
  {
    return (x * rhs.x) + (y * rhs.y );
  }
  
  template< typename T >
  TVector2<T>& TVector2<T>::operator-=( const TVector2& rhs )
  {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  
  template< typename T >
  TVector2<T>& TVector2<T>::operator+=( const TVector2& rhs )
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  
  template< typename T >
  bool TVector2<T>::operator==( const TVector2& rhs) const
  {
    return ( x == rhs.x ) && ( y == rhs.y );
  }
  
  template< typename T >
  bool TVector2<T>::operator!=( const TVector2& rhs ) const
  {
    return !operator==( rhs );
  }
  
  template< typename T >
  TVector2<T> TVector2<T>::operator+( const TVector2& rhs ) const
  {
    return TVector2<T>( x + rhs.x, y + rhs.y );
  }
  
  template< typename T >
  TVector2<T> TVector2<T>::operator-( const TVector2& rhs ) const
  {
    return TVector2<T>( x - rhs.x, y - rhs.y );
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Normalization
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TVector2<T>& TVector2<T>::Normalize()
  {
    T m = Magnitude();
    
    if( m <= 0.00001 )
    {
      // TODO: Decide behavior/document normalization of zero length vector
      m = 1.0F;
    }
    
    m = 1 / m;
    
    x *= m;
    y *= m;
    
    return *this;
  }
  
  template< typename T >
  TVector2<T> TVector2<T>::Normalized() const
  {
    TVector2 v(*this);
    v.Normalize();
    return v;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Magnitude
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  T TVector2<T>::Magnitude() const
  {
    return fastSqrt_Q3( MagnitudeSquared() );
  }
  
  template< typename T >
  T TVector2<T>::MagnitudeSquared() const
  {
    return ( x * x ) + ( y * y );
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Helpers / General
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  void TVector2<T>::Negate()
  {
    x = -x;
    y = -y;
  }

  template< typename T >
  void TVector2<T>::Rotate(float angle)
  {
    float sinAngle = sin(angle);
    float cosAngle = cos(angle);

    x = x * cosAngle - y * sinAngle;
    y = x * sinAngle + y * cosAngle;
  }

  template< typename T >
  TVector2<T> TVector2<T>::Rotated(float angle) const
  {
    TVector2 v(*this);

    v.Rotate(angle);

    return v;
  }

} // Math namespace
} // SF namespace


#endif // SF_T_VECTOR_2_H
