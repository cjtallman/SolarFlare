/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Thread.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:45:44 - nwilliam
**  \brief   Portable, generic wrapper around a thread.
*******************************************************************************/

#ifndef SF_THREAD_H
#define SF_THREAD_H

#include "CriticalSection.h"

namespace SF {
namespace Threading
{
  #if defined( _MSC_VER )
    typedef unsigned long (__stdcall *ThreadFunction)(void *);
  #elif defined( __GNUC__ )
    typedef void* (*ThreadFunction)(void *);
  #else
    #error "Thread never compiled on this compiler"
  #endif

  class IThread;
  class Thread;
  class ThreadImp;

  struct ThreadParam
  {
    ThreadParam();
    ThreadParam(void *_param, IThread *_thread);

    void *param;
    IThread *thread;
  };

  class IThread
  {
  public:
    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual bool IsRunning() = 0;

    virtual void Idle() = 0;

    virtual void CheckErrors() = 0;

    virtual void Terminate() = 0;

    virtual void Wait() = 0;
    virtual void Wait(unsigned int timeout) = 0;
  };

  class Thread : public IThread
  {
  public:
    Thread(ThreadFunction function);
    Thread(ThreadFunction function, void *parameter);
    ~Thread();

    void Start();
    void Pause();
    bool IsRunning();

    void Idle();

    void CheckErrors();

    void Terminate();

    void Wait();
    void Wait(unsigned int timeout);

    static void YieldThread();
  private:
    ThreadImp *m_imp;
  };

} // Threading namespace
} // SF namespace

#endif // SF_THREAD_H
