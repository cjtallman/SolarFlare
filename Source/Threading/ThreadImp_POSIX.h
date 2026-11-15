/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ThreadImp_POSIX.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 01/15/2009 @ 01:00:46 -- nwilliam
**  \brief   POSIX/pthread interface.
*******************************************************************************/

#if defined( __linux )

#ifndef SF_THREADIMP_POSIX_H
#define SF_THREADIMP_POSIX_H

#include <pthread.h>

#include "Thread.h"
#include "CriticalSection.h"

namespace SF {
namespace Threading
{

class ThreadImp : public IThread
{
  public:
    ThreadImp( ThreadFunction function );
    ThreadImp( ThreadFunction function, void* parameter );
    ~ThreadImp();

    void Start();
    void Pause();
    bool IsRunning();

    void Idle();

    void CheckErrors();

    void Terminate();

    void Wait();
    void Wait( unsigned int timeout );

    pthread_t handle;

    static void YieldThread();

  private:
    void Create( ThreadFunction function, void* parameter );

    bool m_running;
    ThreadParam m_threadParam;
    CriticalSection m_cs_suspended;
  };

} // Threading namespace
} // SF namespace

#endif // SF_THREADIMP_POSIX_H

#endif // __linux
