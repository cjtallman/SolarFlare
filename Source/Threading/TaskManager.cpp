/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TaskManager.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:46:09 - nwilliam
**  \brief   Task manager implementation (managers running thread, tasks, etc).
*******************************************************************************/

#include <algorithm>
#include "Thread.h"
#include "TaskManager.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Threading
{

TaskManager::TaskManager(unsigned int threadCount)
{
  m_shutdown = false;
  
  m_activeThreadCount = 0;
  m_criticalTaskCount = 0;
  m_finishedTaskCount = 0;
  m_totalTaskCount = 0;

  m_threads.reserve(threadCount);

  // Create our threads
  m_cs_tasks.Enter();
  for(unsigned int i = 0; i < threadCount; ++i)
  {
    Thread *thread = new Thread(ExecuteTask, this);

    ++m_activeThreadCount;

    m_threads.push_back(thread);
  }
  m_cs_tasks.Leave();
}

TaskManager::~TaskManager()
{
  m_shutdown = true;

  m_cs_tasks.Enter();

  // Get rid of any waiting tasks
  while( !m_waitingTasks.empty() )
  {
    Task *task = m_waitingTasks.back();
    m_waitingTasks.pop_back();

    delete task;
  }

  m_cs_tasks.Leave();

  // Make sure no threads are paused
  std::for_each( m_threads.begin(), m_threads.end(), std::mem_fun( &Thread::Start ) );

  // Wait for all the threads to return.
  for(unsigned int i = 0; i < m_threads.size(); ++i)
    m_threads[i]->Wait(500);

  // Delete thread objects
  for(unsigned int i = 0; i < m_threads.size(); ++i)
    delete m_threads[i];

  m_threads.clear();

  // All threads should be stopped by this point

  while( !m_finishedTasks.empty() )
  {
    Task *task = m_finishedTasks.back();
    m_finishedTasks.pop_back();

    delete task;
  }
}

void TaskManager::Update()
{
  m_cs_tasks.Enter();

  if( !m_waitingTasks.empty() && IdleThreadCount() > 0 )
    RunThread();

  while( !m_finishedTasks.empty() )
  {
    Task *task = m_finishedTasks.back();
    m_finishedTasks.pop_back();

    // if !task->Aborted()
    task->OnFinish();

    m_cs_finishedTaskCount.Enter();
      ++m_finishedTaskCount;

      if( task->Critical() )
        --m_criticalTaskCount;
    m_cs_finishedTaskCount.Leave();

    delete task;
  }

  m_cs_tasks.Leave();
}

void TaskManager::RunThread()
{
  m_cs_tasks.Enter();

  if( IdleThreadCount() > 0 )
  {
    Thread *thread = 0;

    // Find an available thread
    for(unsigned int i = 0; i < m_threads.size(); ++i)
    {
      if( !m_threads[i]->IsRunning() )
      {
        thread = m_threads[i];
        break;
      }
    }

    // Start the thread if we found one
    if(thread)
    {
      ++m_activeThreadCount;
      thread->Start();
    }
  }

  m_cs_tasks.Leave();
}

void TaskManager::ReturnThread( IThread *thread )
{
  m_cs_tasks.Enter();

  // Only pause the thread if there are no more tasks
  if( m_waitingTasks.empty() )
  {
    --m_activeThreadCount;
    thread->Pause();
  }

  m_cs_tasks.Leave();
}

Task *TaskManager::GetWaitingTask()
{
  Task *task = 0;

  m_cs_tasks.Enter();

  for(unsigned int i = 0; i < m_waitingTasks.size(); ++i)
  {
    if( !IsValidTaskID( m_waitingTasks[i]->GetDependency() ) )
    {
      task = m_waitingTasks[i];

      m_workingTasks.push_back(task);
      
      m_waitingTasks.erase( m_waitingTasks.begin() + i );

      break;
    }
  }

  m_cs_tasks.Leave();

  return task;
}

bool TaskManager::IsValidTaskID(unsigned int taskID) const
{
  bool valid = false;

  if (taskID == 0)
    return false;

  m_cs_tasks.Enter();

  valid = IsWorkingTaskID( taskID );

  if( !valid )
  {
    for(unsigned int i = 0; i < m_finishedTasks.size(); ++i)
    {
      if(m_finishedTasks[i]->ID() == taskID)
      {
        valid = true;
        break;
      }
    }
  }

  m_cs_tasks.Leave();

  return valid;
}

// TODO: Naming? This returns whether the taskID is a working OR waiting task
bool TaskManager::IsWorkingTaskID(unsigned int taskID) const
{
  bool valid = false;

  if (taskID == 0)
    return false;

  m_cs_tasks.Enter();

  for(unsigned int i = 0; i < m_waitingTasks.size(); ++i)
  {
    if(m_waitingTasks[i]->ID() >= taskID)
    {
      if(m_waitingTasks[i]->ID() == taskID)
        valid = true;
      break;
    }
  }

  if( !valid )
  {
    for(unsigned int i = 0; i < m_workingTasks.size(); ++i)
    {
      if(m_workingTasks[i]->ID() >= taskID)
      {
        if(m_workingTasks[i]->ID() == taskID)
          valid = true;
        break;
      }
    }
  }

  m_cs_tasks.Leave();

  return valid;
}

bool TaskManager::ShuttingDown() const
{
  return m_shutdown;
}

void TaskManager::WaitForAbortedTask(unsigned int taskID)
{
  // Wait for aborted tasks to finish (aborting a working task will not actually stop it)
  while( IsValidTaskID( taskID ) )
  {
    Update();
    Thread::YieldThread();
  }
}

void TaskManager::WaitForAllTasks()
{
  // Wait until all tasks (working and waiting) have finished
  while( FinishedTaskCount() < TotalTaskCount() )
  {
    Update();
    Thread::YieldThread();
  }
}

void TaskManager::WaitForCriticalTasks()
{
  // Wait until all critical tasks have finished
  while( CriticalTaskCount() > 0 )
  {
    Update();
    Thread::YieldThread();
  }
}

void TaskManager::AbortTask( unsigned int taskID )
{
  if( taskID == 0 )
    return;

  bool aborted = false;

  m_cs_tasks.Enter();

  // Abort the task if it is currently working
  for(unsigned int i = 0; i < m_workingTasks.size(); ++i)
  {
    if( m_workingTasks[i]->ID() == taskID )
    {
      m_workingTasks[i]->Abort();
      aborted = true;
      break;
    }
  }

  //  Abort and remove the task if it is waiting
  if( !aborted )
  {
    for(unsigned int i = 0; i < m_waitingTasks.size(); ++i)
    {
      if( m_waitingTasks[i]->ID() == taskID )
      {
        m_waitingTasks[i]->Abort();

        delete m_waitingTasks[i];
        m_waitingTasks.erase( m_waitingTasks.begin() + i );

        --m_totalTaskCount;

        aborted = true;
        break;
      }
    }
  }

  // Abort and remove the task if it is finished
  if( !aborted )
  {
    for(unsigned int i = 0; i < m_finishedTasks.size(); ++i)
    {
      if( m_finishedTasks[i]->ID() == taskID )
      {
        m_finishedTasks[i]->Abort();

        delete m_finishedTasks[i];
        m_finishedTasks.erase( m_finishedTasks.begin() + i );

        --m_totalTaskCount;

        break;
      }
    }
  }

  m_cs_tasks.Leave();
}

void TaskManager::AbortAllTasks()
{
  m_cs_tasks.Enter();

  while( !m_waitingTasks.empty() )
  {
    m_waitingTasks.back()->Abort();

    delete m_waitingTasks.back();
    m_waitingTasks.pop_back();

    --m_totalTaskCount;
  }

  m_cs_tasks.Leave();
}

void TaskManager::FinishTask( Task *task )
{
  m_cs_tasks.Enter();

  TaskList::iterator iter = std::find( m_workingTasks.begin(), m_workingTasks.end(), task );

  if( iter != m_workingTasks.end() )
  {
    m_workingTasks.erase( iter );

    if( !task->Aborted() )
    {
      m_finishedTasks.push_back( task );
    }
    else
    {
      task->OnFinish();

      m_cs_finishedTaskCount.Enter();
        ++m_finishedTaskCount;

        if( task->Critical() )
          --m_criticalTaskCount;
      m_cs_finishedTaskCount.Leave();

      delete task;
    }
  }

  m_cs_tasks.Leave();
}

unsigned int TaskManager::FinishedTaskCount() const
{
  return m_finishedTaskCount;
}

unsigned int TaskManager::TotalTaskCount() const
{
  return m_totalTaskCount;
}

unsigned int TaskManager::CriticalTaskCount() const
{
  return m_criticalTaskCount;
}

unsigned int TaskManager::WorkingTaskCount() const
{
  return m_totalTaskCount - m_finishedTaskCount;
}

unsigned int TaskManager::IdleThreadCount() const
{
  return TotalThreadCount() - m_activeThreadCount;
}

unsigned int TaskManager::WorkingThreadCount() const
{
  return m_activeThreadCount;
}

unsigned int TaskManager::TotalThreadCount() const
{
  return (unsigned int) m_threads.size();
}

}

}
