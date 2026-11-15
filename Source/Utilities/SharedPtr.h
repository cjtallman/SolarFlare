/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SharedPtr.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/02/2009 @ 19:06:02 - nwilliam
**  \brief   Managed pointer to an object allocated with new (-not- new[]).
*******************************************************************************/

#ifndef SF_SHARED_PTR_H
#define SF_SHARED_PTR_H

#include "RefCount.h"

namespace SF {
namespace Utilities
{
  template< typename T >
  class SharedPtr
  {
    public:
      SharedPtr();
      explicit SharedPtr( T* pointer );
      SharedPtr( const SharedPtr<T>& rhs );
      ~SharedPtr();

      // Operators
      T& operator*() const;
      T* operator->() const;
      SharedPtr& operator=( T* pointer );
      SharedPtr& operator=( const SharedPtr<T>& rhs );

      // Other Methods
      T* Get() const;
      int Count() const;
      bool IsUnique() const;
      void Reset( T* pointer );
      void Swap( SharedPtr<T>& rhs );

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
  SharedPtr<T>::SharedPtr() : m_count(),
                              m_pointer( 0 )
  {
  }

  template< typename T >
  SharedPtr<T>::SharedPtr( T* pointer ) : m_count(),
                                          m_pointer( pointer )
  {
  }

  template< typename T >
  SharedPtr<T>::SharedPtr( const SharedPtr<T>& rhs ) : m_count( rhs.m_count ),
                                                       m_pointer( rhs.m_pointer )
  {
  }

  template< typename T >
  SharedPtr<T>::~SharedPtr()
  {
    if( m_count.Unique() )
    {
      delete m_pointer;
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  template< typename T >
  T& SharedPtr<T>::operator*() const
  {
    return *m_pointer;
  }

  template< typename T >
  T* SharedPtr<T>::operator->() const
  {
    return m_pointer;
  }

  template< typename T >
  SharedPtr<T>& SharedPtr<T>::operator=( T* pointer )
  {
    Reset( pointer );
    return *this;
  }

  template< typename T >
  SharedPtr<T>& SharedPtr<T>::operator=( const SharedPtr<T>& rhs )
  {
    // Check for self-assignment
    if( this != &rhs )
    {
      if( IsUnique() )
      {
        delete m_pointer;
      }

      m_count = rhs.m_count;
      m_pointer = rhs.m_pointer;
    }

    return *this;
  }

  // Other Methods
  template< typename T >
  T* SharedPtr<T>::Get() const
  {
    return m_pointer;
  }

  template< typename T >
  int SharedPtr<T>::Count() const
  {
    return m_count.Count();
  }

  template< typename T >
  void SharedPtr<T>::Reset( T* pointer )
  {
    SharedPtr<T>( pointer ).Swap( *this );
  }

  template< typename T >
  bool SharedPtr<T>::IsUnique() const
  {
    return m_count.Unique();
  }

  template< typename T >
  void SharedPtr<T>::Swap( SharedPtr<T>& rhs )
  {
    T* temp1 = m_pointer;
    m_pointer = rhs.m_pointer;
    rhs.m_pointer = temp1;

    m_count.Swap( rhs.m_count );
  }

  // Conversion
  template< typename T >
  SharedPtr<T>::operator bool() const
  {
    return m_pointer != 0;
  }

} // Utilities namespace
} // SF namespace

#endif // SF_SHARED_PTR_H
