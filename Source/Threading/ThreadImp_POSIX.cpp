/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ThreadImp_POSIX.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/03/2009 @ 22:39:48 -- nwilliam
**  \brief   POSIX/pthread based thread implementation.
*******************************************************************************/

#if defined( __linux )

#include "ThreadImp_POSIX.h"
#include <unistd.h> // usleep

namespace SF {
namespace Threading
{
  ThreadImp::ThreadImp( ThreadFunction function ) : handle( 0 ),
                                                    m_running( false )
  {
    Create( function, 0 );
  }

  ThreadImp::ThreadImp( ThreadFunction function, void* parameter ) : handle( 0 ),
                                                                     m_running( false )
  {
    Create( function, parameter );
  }

  ThreadImp::~ThreadImp()
  {
  }

  void ThreadImp::Create( ThreadFunction function, void* parameter )
  {
    m_threadParam = ThreadParam( parameter, this );

    if( pthread_create( &handle, 0, function, parameter ) )
      m_running = true;
    else
      m_running = false; // Failed, check error?
  }

  void ThreadImp::CheckErrors()
  {
    // TODO: check errors
  }

  void ThreadImp::Terminate()
  {
    // TODO: check error return vale
    pthread_cancel( handle );
  }

  void ThreadImp::Start()
  {
    m_cs_suspended.Enter();

    // TODO: would set the pthread_cond variable to free/open/etc
    m_running = true;
    //SetThreadPriority(handle, THREAD_PRIORITY_NORMAL);

    m_cs_suspended.Leave();
  }

  void ThreadImp::Pause()
  {
    m_cs_suspended.Enter();

    // TODO: Looks like pthread_cond_wait is the way to do this
    m_running = false;
    //SetThreadPriority(handle, THREAD_PRIORITY_IDLE);

    m_cs_suspended.Leave();
  }

  bool ThreadImp::IsRunning()
  {
    bool running;

    m_cs_suspended.Enter();

    running = m_running;

    m_cs_suspended.Leave();

    return running;
  }

  void ThreadImp::Idle()
  {

  }

  void ThreadImp::Wait()
  {
    Wait(0);
  }

  void ThreadImp::Wait(unsigned int timeout)
  {
		pthread_join( handle, 0 );
  }

  void ThreadImp::YieldThread()
  {
    usleep( 1 );
  }

} // Threading namespace
} // SF namespace

#endif // __linux
