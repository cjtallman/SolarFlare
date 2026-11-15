/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Task.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/02/2009 @ 17:58:14 - nwilliam
**  \brief
*******************************************************************************/

#include "Task.h"
#include "TaskManager.h"

namespace SF {
namespace Threading
{

Task::Task() : m_parameter( 0 ), m_deleteFunction( 0 ), m_function( 0 ), m_functionWrapper( 0 )
{
  static unsigned int nextID = 1;

  m_id = nextID++;

  m_aborted = false;
  m_critical = false;
  m_dependency = 0;
}
Task::~Task()
{
  m_deleteFunction( m_parameter );
}

void Task::Abort()
{
  m_aborted = true;
}

bool Task::Aborted() const
{
  return m_aborted;
}

void Task::SetCritical(bool critical)
{
  m_critical = critical;
}

bool Task::Critical() const
{
  return m_critical;
}

unsigned int Task::GetDependency() const
{
  return m_dependency;
}

void Task::SetDependency(unsigned int taskID)
{
  m_dependency = taskID;
}

unsigned int Task::ID() const
{
  return m_id;
}

void Task::DoTask()
{
  if( !Aborted() )
    m_functionWrapper(m_function, m_parameter);
}

void Task::OnFinish()
{
  if( !Aborted() && m_onFinish )
    m_onFinish();
}

#if defined( _MSC_VER )
  unsigned long __stdcall ExecuteTask(void *param)
#elif defined( __GNUC__ )
  void* ExecuteTask( void *param )
#else
  #error "Need implementation"
#endif
{
  ThreadParam *threadParam = (ThreadParam *)param;

  TaskManager *manager = (TaskManager *)threadParam->param;
  IThread *thread = threadParam->thread;

  Task *task = 0;

  for(;;)
  {
    // Idle until we need to do stuff or shutdown
    while( !thread->IsRunning() && !manager->ShuttingDown() )
    {
      thread->Idle();
    }

    // The task manager that owns this thread is shutting down
    // TODO: Add individual thread shutdown
    if( manager->ShuttingDown() )
    {
      return 0;
    }

    // Attempt to get a task from the task manager
    task = manager->GetWaitingTask();

    if(task)
    {
      // Perform the task
      task->DoTask();

      // Tell the manager that we've finished
      manager->FinishTask( task );
    }

    // Tell the manager that this thread is no longer in use
    manager->ReturnThread(thread);
  }
}

}

}
