/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Thread.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/10/2008 @ 17:52:21 - pmera
**  \brief
*******************************************************************************/

#include "Thread.h"

#if defined( _WIN32 )
  #include "ThreadImp_Win32.h"
#elif defined( __linux )
  #include "ThreadImp_POSIX.h"
#else
  #error "No thread implementation."
#endif

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Threading
{

ThreadParam::ThreadParam()
{
  param = 0;
  thread = 0;
}

ThreadParam::ThreadParam(void *_param, IThread *_thread)
{
  param = _param;
  thread = _thread;
}

Thread::Thread(ThreadFunction function)
{
  m_imp = new ThreadImp(function);
}

Thread::Thread(ThreadFunction function, void *parameter)
{
  m_imp = new ThreadImp(function, parameter);
}

Thread::~Thread()
{
  delete m_imp;
}

void Thread::CheckErrors()
{
  m_imp->CheckErrors();
}

void Thread::Terminate()
{
  m_imp->Terminate();
}

void Thread::Start()
{
  m_imp->Start();
}

void Thread::Pause()
{
  m_imp->Pause();
}

bool Thread::IsRunning()
{
  return m_imp->IsRunning();
}

void Thread::Idle()
{
  m_imp->Idle();
}

void Thread::Wait()
{
  m_imp->Wait();
}

void Thread::Wait(unsigned int timeout)
{
  m_imp->Wait(timeout);
}

void Thread::YieldThread()
{
  ThreadImp::YieldThread();
}

}

}
