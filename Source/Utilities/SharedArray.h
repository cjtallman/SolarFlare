/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SharedArray.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/02/2009 @ 19:06:02 - nwilliam
**  \brief   Managed pointer to an array allocated with new[] (-not- new).
*******************************************************************************/

#ifndef SF_SHARED_ARRAY_H
#define SF_SHARED_ARRAY_H

#include <cstddef>  // std::ptrdiff_t
#include "RefCount.h"

namespace SF {
namespace Utilities
{
  template< typename T >
  class SharedArray
  {
    public:
      SharedArray();
      explicit SharedArray( T* pointer );
      SharedArray( const SharedArray<T>& rhs );
      ~SharedArray();

      // Operators
      T& operator*() const;
      T* operator->() const;
      SharedArray& operator=( T* pointer );
      SharedArray& operator=( const SharedArray<T>& rhs );

      T& operator[]( std::ptrdiff_t i );
      const T& operator[]( std::ptrdiff_t i ) const;

      // Other Methods
      T* Get() const;
      int Count() const;
      bool IsUnique() const;
      void Reset( T* pointer );
      void Swap( SharedArray<T>& rhs );

      // Conversion
      operator bool() const;

    private:
      T* m_pointer;     // contained pointer
      RefCount m_count;
  };

} // Utilities namespace
} // SF namespace


namespace SF {
namespace Utilities
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  SharedArray<T>::SharedArray() : m_count(),
                                  m_pointer( 0 )
  {
  }

  template< typename T >
  SharedArray<T>::SharedArray( T* pointer ) : m_count(),
                                              m_pointer( pointer )
  {
  }

  template< typename T >
  SharedArray<T>::SharedArray( const SharedArray<T>& rhs ) : m_count( rhs.m_count ),
                                                             m_pointer( rhs.m_pointer )
  {
  }

  template< typename T >
  SharedArray<T>::~SharedArray()
  {
    if( m_count.Unique() )
    {
      delete[] m_pointer;
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  T& SharedArray<T>::operator*() const
  {
    return *m_pointer;
  }

  template< typename T >
  T* SharedArray<T>::operator->() const
  {
    return m_pointer;
  }

  template< typename T >
  SharedArray<T>& SharedArray<T>::operator=( T* pointer )
  {
    Reset( pointer );
    return *this;
  }

  template< typename T >
  SharedArray<T>& SharedArray<T>::operator=( const SharedArray<T>& rhs )
  {
    // Check for self-assignment
    if( this != &rhs )
    {
      if( IsUnique() )
      {
        delete[] m_pointer;
      }

      m_count = rhs.m_count;
      m_pointer = rhs.m_pointer;
    }

    return *this;
  }

  template< typename T >
  T& SharedArray<T>::operator[]( std::ptrdiff_t i )
  {
    return m_pointer[i];
  }

  template< typename T >
  const T& SharedArray<T>::operator[]( std::ptrdiff_t i ) const
  {
    return m_pointer[i];
  }

  // Other Methods
  template< typename T >
  T* SharedArray<T>::Get() const
  {
    return m_pointer;
  }

  template< typename T >
  int SharedArray<T>::Count() const
  {
    return m_count.Count();
  }

  template< typename T >
  void SharedArray<T>::Reset( T* pointer )
  {
    SharedArray<T>( pointer ).Swap( *this );
  }

  template< typename T >
  bool SharedArray<T>::IsUnique() const
  {
    return m_count.Unique();
  }

  template< typename T >
  void SharedArray<T>::Swap( SharedArray<T>& rhs )
  {
    T* temp1 = m_pointer;
    m_pointer = rhs.m_pointer;
    rhs.m_pointer = temp1;

    m_count.Swap( rhs.m_count );
  }

  // Conversion
  template< typename T >
  SharedArray<T>::operator bool() const
  {
    return m_pointer != 0;
  }

} // Utilities namespace
} // SF namespace

#endif // SF_SHARED_ARRAY_H
