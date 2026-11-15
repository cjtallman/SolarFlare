/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RefCount.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 23:41:52 - nwilliam
**  \brief   Non-thread safe ref counter implementation.
*******************************************************************************/

#include "RefCount.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Utilities
{
  //////////////////////////////////////////////////////////////////////////////
  // Static member
  //////////////////////////////////////////////////////////////////////////////
  RefCount::IntMemPool RefCount::s_memPool;

  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  RefCount::RefCount() : m_count( 0 )
  {
    m_count = s_memPool.AllocateBlock();
    *m_count = 1;
  }

  RefCount::RefCount( const RefCount& rhs ) : m_count( 0 )
  {
    m_count = rhs.m_count;
    ++(*m_count);
  }

  RefCount::~RefCount()
  {
    Release();
  }

  int RefCount::Count() const
  {
    return *m_count;
  }

  bool RefCount::Unique() const
  {
    return *m_count == 1;
  }

  void RefCount::Swap( RefCount& rhs )
  {
    int* temp = m_count;
    m_count = rhs.m_count;
    rhs.m_count = temp;
  }

  RefCount& RefCount::operator=( const RefCount& rhs )
  {
    if( this != &rhs )
    {
      Release();
      m_count = rhs.m_count;
      ++(*m_count);
    }

    return *this;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void RefCount::Release()
  {
    --(*m_count);

    if( Count() == 0 )
    {
      s_memPool.FreeBlock( m_count );
      m_count = 0;
    }
  }

} // Utilities namespace
} // SF namespace
