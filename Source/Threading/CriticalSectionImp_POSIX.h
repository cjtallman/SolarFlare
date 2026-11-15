/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CriticalSectionImp_Win32.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/03/2009 @ 22:42:27 -- nwilliam
**  \brief   POSIX/pthread based mutex.
*******************************************************************************/

#if defined( __linux )

#ifndef SF_CRITICAL_SECTION_IMP_POSIX_H
#define SF_CRITICAL_SECTION_IMP_POSIX_H

#include "CriticalSection.h"
#include <pthread.h>

namespace SF {
namespace Threading
{
  class CriticalSectionImp : public ICriticalSection
  {
    public:
      CriticalSectionImp();
      ~CriticalSectionImp();

      void Enter();
      void Leave();

    private:
      pthread_mutex_t m_mutex;
      pthread_mutexattr_t m_mutexAttr;
  };

} // Threading namespace
} // SF namespace

#endif // SF_CRITICAL_SECTION_IMP_POSIX_H

#endif // __linux
