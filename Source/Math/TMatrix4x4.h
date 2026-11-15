/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TMatrix4x4.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/10/2009 @ 00:08:50 - nwilliam
**  \brief   Templatized, for internal array type, 4x4 matrix class
*******************************************************************************/

#ifndef SF_T_MATRIX_4_X_4_H
#define SF_T_MATRIX_4_X_4_H

namespace SF {
namespace Math
{
  // Forward declarations
  template< typename T> struct TVector3;
  template< typename T> struct TVector4;
  
  
  template< typename T >
  class TMatrix4x4
  {
    const static int ROW_SIZE = 4;
    const static int COL_SIZE = 4;
    const static int DATA_SIZE = ROW_SIZE * COL_SIZE;
    
    public:
      TMatrix4x4();
      TMatrix4x4( T i0j0, T i0j1, T i0j2, T i0j3,
                  T i1j0, T i1j1, T i1j2, T i1j3,
                  T i2j0, T i2j1, T i2j2, T i2j3,
                  T i3j0, T i3j1, T i3j2, T i3j3 );
      
      // Default copy, assignment, destructor ok
      
      static TMatrix4x4<T> MakeTranslatation( T x, T y, T z );
      static TMatrix4x4<T> MakeAxisAngle( T angle, T x, T y, T z );
      static TMatrix4x4<T> MakeScale( T cx, T cy, T cz, T x, T y, T z );
      
      TMatrix4x4& Transpose();
      TMatrix4x4 Transposed() const;
      
      T& Get( int i, int j );
      T& operator[]( unsigned pos );
      
      T Get( int i, int j ) const; 
      T operator[]( unsigned pos ) const;
      
      TVector3<T> Row3( int i ) const;
      TVector4<T> Row4( int i ) const;
      
      TVector3<T> Col3( int j ) const;
      TVector4<T> Col4( int j ) const;

      const T * Array() const;
      T * Array();
      
      void Translate( T x, T y, T z );
      
      TMatrix4x4& operator*=( const TMatrix4x4& rhs );
      TMatrix4x4& operator+=( const TMatrix4x4& rhs );
      TMatrix4x4& operator-=( const TMatrix4x4& rhs );
      TMatrix4x4 operator*( const TMatrix4x4& rhs ) const;
      TMatrix4x4 operator+( const TMatrix4x4& rhs ) const;
      TMatrix4x4 operator-( const TMatrix4x4& rhs ) const;
      
      TMatrix4x4& operator*=( T rhs );
      TMatrix4x4& operator/=( T rhs );
      TMatrix4x4 operator*( T rhs ) const;
      TMatrix4x4 operator/( T rhs ) const;
      
      
      TVector3<T> operator*( const TVector3<T>& rhs ) const;
      
      private:
        T m_data[TMatrix4x4::DATA_SIZE]; 
    };
    
    template< typename T >
    TMatrix4x4<T> operator*( T lhs, const TMatrix4x4<T>& rhs );
    
    template< typename T >
    TMatrix4x4<T> operator/( T lhs, const TMatrix4x4<T>& rhs );
    
    
    
    // Default matrix4 type
    typedef TMatrix4x4<float> Matrix4;
    
    
} // Math namespace
} // SF namespace




#include <cstring> //memset
#include "TVector3.h"
#include "TVector4.h"
#include "Utilities/Misc.h"

namespace SF {
namespace Math
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TMatrix4x4<T>::TMatrix4x4()
  {
    std::memset( m_data, 0, sizeof( T ) * DATA_SIZE );
    m_data[0] = m_data[5] = m_data[10] = m_data[15] = 1.0f;
  }
  
  template< typename T >
  TMatrix4x4<T>::TMatrix4x4( T i0j0, T i0j1, T i0j2, T i0j3,
                             T i1j0, T i1j1, T i1j2, T i1j3,
                             T i2j0, T i2j1, T i2j2, T i2j3,
                             T i3j0, T i3j1, T i3j2, T i3j3 )
  {
    // Stored column major
    m_data[0] = i0j0;
    m_data[1] = i1j0;
    m_data[2] = i2j0;
    m_data[3] = i3j0;
    
    m_data[4] = i0j1;
    m_data[5] = i1j1;
    m_data[6] = i2j1;
    m_data[7] = i3j1;
    
    m_data[8]  = i0j2;
    m_data[9]  = i1j2;
    m_data[10] = i2j2;
    m_data[11] = i3j2;
    
    m_data[12] = i0j3;
    m_data[13] = i1j3;
    m_data[14] = i2j3;
    m_data[15] = i3j3;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TMatrix4x4<T> TMatrix4x4<T>::MakeTranslatation( T x, T y, T z )
  {
    TMatrix4x4<T> m( 1.0f, 0.0f, 0.0f, x,
                     0.0f, 1.0f, 0.0f, y,
                     0.0f, 0.0f, 1.0f, z,
                     0.0f, 0.0f, 0.0f, 1.0f );
    
    return m;
  }
  
  template< typename T >
  TMatrix4x4<T> TMatrix4x4<T>::MakeAxisAngle( T angle, T x, T y, T z )
  {
    Math::Vector3 n( x, y, z );
    n.Normalize();
    
    x = n.x;
    y = n.y;
    z = n.z;
    
    T c = std::cos( angle );
    T s = std::sin( angle );
    T t = 1 - c;
    
    // Standard axis-angle rotation matrix, column major
    TMatrix4x4<T> m( t*x*x + c,    t*x*y + s*z,  t*x*z - s*y,  0.0f,
                     t*x*y - s*z,  t*y*y + c,    t*y*z + s*x,  0.0f,
                     t*x*y + s*y,  t*y*z - s*x,  t*z*z + c,    0.0f,
                     0.0f,         0.0f,         0.0f,         1.0f
                   );
    
    return m;
  }
  
  template< typename T >
  TMatrix4x4<T> TMatrix4x4<T>::MakeScale( T cx, T cy, T cz, T x, T y, T z )
  {
    TMatrix4x4<T> trans1 = TMatrix4x4<T>::MakeTranslatation(  cx,  cy,  cz );
    TMatrix4x4<T> trans2 = TMatrix4x4<T>::MakeTranslatation( -cx, -cy, -cz );
    
    TMatrix4x4<T> scale(    x,   0.0f,   0.0f,   0.0f,
                         0.0f,      y,   0.0f,   0.0f,
                         0.0f,   0.0f,     z,    0.0f,
                         0.0f,   0.0f,   0.0f,   1.0f );
                
    return trans2 * scale * trans1;
  }
  
  template< typename T >
  TMatrix4x4<T>& TMatrix4x4<T>::Transpose()
  {
    Utilities::Swap( m_data[1], m_data[4] );
    Utilities::Swap( m_data[2], m_data[8] );
    Utilities::Swap( m_data[3], m_data[12] );
    Utilities::Swap( m_data[6], m_data[9] );
    Utilities::Swap( m_data[7], m_data[13] );
    Utilities::Swap( m_data[1], m_data[14] );
    return *this;
  }
  
  template< typename T >
  TMatrix4x4<T> TMatrix4x4<T>::Transposed() const
  {
    TMatrix4x4<T> copy( *this );
    copy.Transpose();
    return copy;
  }
  
  template< typename T >
  T& TMatrix4x4<T>::Get( int i, int j )
  {
    return m_data[ j + ( i * TMatrix4x4<T>::COL_SIZE ) ];
  }
  
  template< typename T >
  T& TMatrix4x4<T>::operator[]( unsigned pos )
  {
    return m_data[pos];
  }
  
  template< typename T >
  T TMatrix4x4<T>::Get( int i, int j ) const
  {
    return m_data[ j + ( i * TMatrix4x4<T>::COL_SIZE ) ];
  }
  
  template< typename T >
  T TMatrix4x4<T>::operator[]( unsigned pos ) const
  {
    return m_data[pos];
  }
  
  template< typename T >
  TVector3<T> TMatrix4x4<T>::Row3( int i ) const
  {
    return TVector3<T>( Get(i,0), Get(i,1), Get(i,2) );
  }
  
  template< typename T >
  TVector4<T> TMatrix4x4<T>::Row4( int i ) const
  {
    return TVector4<T>( Get(i,0), Get(i,1), Get(i,2), Get(i,3) );
  }
  
  template< typename T >
  TVector3<T> TMatrix4x4<T>::Col3( int j ) const
  {
    return TVector3<T>( Get(0,j), Get(1,j), Get(2,j) );
  }
  
  template< typename T >
  TVector4<T> TMatrix4x4<T>::Col4( int j ) const
  {
    return TVector4<T>( Get(0,j), Get(1,j), Get(2,j), Get(3,j) );
  }

  template <typename T>
  const T * TMatrix4x4<T>::Array() const
  {
    return m_data;
  }

  template <typename T>
  T * TMatrix4x4<T>::Array()
  {
    return m_data;
  }
  
  template< typename T >
  void TMatrix4x4<T>::Translate( T x, T y, T z )
  {
    m_data[12] = m_data[12] + m_data[0]*x + m_data[4]*y + m_data[8]*z;
    m_data[13] = m_data[13] + m_data[1]*x + m_data[5]*y + m_data[9]*z;
    m_data[14] = m_data[14] + m_data[2]*x + m_data[6]*y + m_data[10]*z;
  }
  
  template< typename T >
  TMatrix4x4<T>& TMatrix4x4<T>::operator*=( const TMatrix4x4<T>& rhs )
  {
    TMatrix4x4<T> mm( *this );
    
    const T* const a = m_data;
    const T* const b = rhs.m_data;
    
    mm[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2] + a[12]*b[3];
    mm[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2] + a[13]*b[3]; 
    mm[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2] + a[14]*b[3];
    mm[3] = a[3]*b[0] + a[7]*b[1] + a[11]*b[2] + a[15]*b[3];
    
    mm[4] = a[0]*b[4] + a[4]*b[5] + a[8]*b[6] + a[12]*b[7];
    mm[5] = a[1]*b[4] + a[5]*b[5] + a[9]*b[6] + a[13]*b[7];
    mm[6] = a[2]*b[4] + a[6]*b[5] + a[10]*b[6] + a[14]*b[7];
    mm[7] = a[3]*b[4] + a[7]*b[5] + a[11]*b[6] + a[15]*b[7];
    
    mm[8] = a[0]*b[8] + a[4]*b[9] + a[8]*b[10] + a[12]*b[11];
    mm[9] = a[1]*b[8] + a[5]*b[9] + a[9]*b[10] + a[13]*b[11];
    mm[10] = a[2]*b[8] + a[6]*b[9] + a[10]*b[10] + a[14]*b[11];
    mm[11] = a[3]*b[8] + a[7]*b[9] + a[11]*b[10] + a[15]*b[11];
    
    mm[12] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14] + a[12]*b[15];
    mm[13] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14] + a[13]*b[15];
    mm[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14]*b[15];
    mm[15] = a[3]*b[12] + a[7]*b[13] + a[11]*b[14] + a[15]*b[15];
    
    *this = mm;
    
    return *this;
  }
  
  template< typename T >
  TMatrix4x4<T>& TMatrix4x4<T>::operator+=( const TMatrix4x4<T>& rhs )
  {
    for( int i = 0; i < TMatrix4x4<T>::DATA_SIZE; ++i )
      m_data[i] += rhs.m_data[i];
    
    return *this;
  }
  
  template< typename T >
  TMatrix4x4<T>& TMatrix4x4<T>::operator-=( const TMatrix4x4<T>& rhs )
  {
    for( int i = 0; i < TMatrix4x4<T>::DATA_SIZE; ++i )
      m_data[i] -= rhs.m_data[i];
    
    return *this;
  }
  
  template< typename T >
  TMatrix4x4<T> TMatrix4x4<T>::operator*( const TMatrix4x4<T>& rhs ) const
  {
    TMatrix4x4<T> copy( *this );
    copy *= rhs;
    return copy;
  }

  template< typename T >
  TMatrix4x4<T> TMatrix4x4<T>::operator+( const TMatrix4x4<T>& rhs ) const
  {
    TMatrix4x4<T> copy( *this );
    copy += rhs;
    return copy;
  }

  template< typename T >
  TMatrix4x4<T> TMatrix4x4<T>::operator-( const TMatrix4x4<T>& rhs ) const
  {
    TMatrix4x4<T> copy( *this );
    copy -= rhs;
    return copy;
  }
  
  template< typename T >
  TMatrix4x4<T>& TMatrix4x4<T>::operator*=( T rhs )
  {
    for( int i = 0; i < TMatrix4x4<T>::DATA_SIZE; ++i )
      m_data[i] *= rhs;
    
    return *this;
  }
  
  template< typename T >
  TMatrix4x4<T>& TMatrix4x4<T>::operator/=( T rhs )
  {
    for( int i = 0; i < TMatrix4x4<T>::DATA_SIZE; ++i )
      m_data[i] /= rhs;
    
    return *this;
  }
  
  template< typename T >
  TMatrix4x4<T> TMatrix4x4<T>::operator*( T rhs ) const
  {
    TMatrix4x4<T> copy( *this );
    
    for( int i = 0; i < TMatrix4x4<T>::DATA_SIZE; ++i )
      copy.m_data[i] *= rhs;
    
    return *this;
  }
  
  template< typename T >
  TMatrix4x4<T> TMatrix4x4<T>::operator/( T rhs ) const
  {
    TMatrix4x4<T> copy( *this );
    
    for( int i = 0; i < TMatrix4x4<T>::DATA_SIZE; ++i )
      copy.m_data[i] /= rhs;
    
    return *this;
  }
  
  template< typename T >
  TVector3<T> TMatrix4x4<T>::operator*( const TVector3<T>& rhs ) const
  {
    TVector3<T> v;
    
    v.x = m_data[0]*rhs.x + m_data[4]*rhs.y + m_data[8]*rhs.z;
    v.y = m_data[1]*rhs.x + m_data[5]*rhs.y + m_data[9]*rhs.z;
    v.z = m_data[2]*rhs.x + m_data[6]*rhs.y + m_data[10]*rhs.z;
    
    return v;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Global functions, T on left
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  TMatrix4x4<T> operator*( T lhs, const TMatrix4x4<T>& rhs )
  {
    return rhs * lhs;
  }
  
  template< typename T >
  TMatrix4x4<T> operator/( T lhs, const TMatrix4x4<T>& rhs )
  {
    return rhs / lhs;
  }
  
} // Math namespace
} // SF namespace



#endif // SF_T_MATRIX_4_X_4_H
