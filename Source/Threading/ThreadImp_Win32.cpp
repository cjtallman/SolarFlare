/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ThreadImp_Win32.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/03/2009 @ 22:38:44 -- nwilliam
**  \brief
*******************************************************************************/

#if defined( _WIN32 )

#include "ThreadImp_Win32.h"

namespace SF {
namespace Threading
{
  ThreadImp::ThreadImp(ThreadFunction function)
  {
    Create(function, 0);
  }

  ThreadImp::ThreadImp(ThreadFunction function, void *parameter)
  {
    Create(function, parameter);
  }

  ThreadImp::~ThreadImp()
  {
    CloseHandle( handle );
  }

  void ThreadImp::Create(ThreadFunction function, void *parameter)
  {
    m_threadParam = ThreadParam(parameter, this);

    m_running = true;

    handle = CreateThread(
      NULL,
      0,
      function,
      &m_threadParam,
      0,
      &id);
  }

  void ThreadImp::CheckErrors()
  {
    if(handle == NULL)
    {
      throw("Thread creation failed.");
    }
  }

  void ThreadImp::Terminate()
  {
    TerminateThread(handle, 0);
  }

  void ThreadImp::Start()
  {
    m_cs_suspended.Enter();

    m_running = true;
    SetThreadPriority(handle, THREAD_PRIORITY_NORMAL);

    m_cs_suspended.Leave();
  }

  void ThreadImp::Pause()
  {
    m_cs_suspended.Enter();

    m_running = false;
    SetThreadPriority(handle, THREAD_PRIORITY_IDLE);

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
    Yield();
  }

  void ThreadImp::Wait()
  {
    Wait(INFINITE);
  }

  void ThreadImp::Wait(unsigned int timeout)
  {
    DWORD result = WaitForSingleObject(handle, timeout);

    if(result == WAIT_TIMEOUT)
    {
      TerminateThread(handle, 0);
    }
  }

  void ThreadImp::YieldThread()
  {
    #ifdef SWITCH_TO_THREAD
      SwitchToThread();
    #else
      Sleep(1);
    #endif
  }

} // Threading namespace
} // SF namespace

#endif // _WIN32
