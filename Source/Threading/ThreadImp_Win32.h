/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ThreadImp_Win32.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/03/2009 @ 22:37:50 -- nwilliam
**  \brief
*******************************************************************************/

#if defined( _WIN32 )

#ifndef SF_THREADIMP_WIN32_H
#define SF_THREADIMP_WIN32_H

#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0400
  #define SWITCH_TO_THREAD
#endif

#include <windows.h>

#include "Thread.h"
#include "CriticalSection.h"

namespace SF {
namespace Threading
{

class ThreadImp : public IThread
{
public:
  ThreadImp(ThreadFunction function);
  ThreadImp(ThreadFunction function, void *parameter);
  ~ThreadImp();

  void Start();
  void Pause();
  bool IsRunning();

  void Idle();

  void CheckErrors();

  void Terminate();

  void Wait();
  void Wait(unsigned int timeout);

  HANDLE handle;
  DWORD id;

  static void YieldThread();

private:
  void Create(ThreadFunction function, void *parameter);

  bool m_running;

  ThreadParam m_threadParam;

  CriticalSection m_cs_suspended;
};

} // Threading namespace
} // SF namespace

#endif // SF_THREADIMP_WIN32_H

#endif // _WIN32
