/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CriticalSectionImp_Win32.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/03/2009 @ 22:54:33 -- nwilliam
**  \brief   POSIX/pthread based mutex implementation.
*******************************************************************************/

#if defined( __linux )

#include "CriticalSectionImp_POSIX.h"

namespace SF {
namespace Threading
{
  CriticalSectionImp::CriticalSectionImp()
  {
    pthread_mutexattr_init( &m_mutexAttr );
    pthread_mutexattr_settype( &m_mutexAttr, PTHREAD_MUTEX_RECURSIVE );
    pthread_mutex_init( &m_mutex , &m_mutexAttr );
  }

  CriticalSectionImp::~CriticalSectionImp()
  {
    pthread_mutex_destroy( &m_mutex );
    pthread_mutexattr_destroy( &m_mutexAttr );
  }

  void CriticalSectionImp::Enter()
  {
    pthread_mutex_lock( &m_mutex );
  }

  void CriticalSectionImp::Leave()
  {
    pthread_mutex_unlock( &m_mutex );
  }

} // Threading namespace
} // SF namespace

#endif // __linux
