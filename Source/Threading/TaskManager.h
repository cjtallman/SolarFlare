/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TaskManager.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/02/2009 @ 18:54:20 - nwilliam
**  \brief   TODO:  -Consider making thread pool grow dynamically (like std::vector).
                    -Keep track of each thread's working time, and periodically
                     raise the priority of threads that have been working longer.
*******************************************************************************/

#ifndef SF_TASKMANAGER_H
#define SF_TASKMANAGER_H

#include <queue>
#include <vector>

#include "Utilities/LeakCheck.h"

#include "Task.h"
#include "Thread.h"
#include "CriticalSection.h"

namespace SF {
namespace Threading
{
  class TaskManager
  {
    public:
      typedef std::deque< Task * > TaskList;
      typedef std::vector< Thread * > ThreadList;

      TaskManager(unsigned int threadCount = 5);
      ~TaskManager();

      void Update();

      template <typename T>
      unsigned int AddTask(
        void (*function)(const T&),
        const T& parameter,
        bool critical = false,
        unsigned int dependency = 0);

      template <typename T>
      unsigned int AddTask(
        void (*function)(const T&),
        const T& parameter,
        Task::OnFinishDelegate onFinish,
        bool critical = false,
        unsigned int dependency = 0 );

      void AbortTask( unsigned int taskID );
      void AbortAllTasks();
      void FinishTask( Task *task );

      void RunThread();
      void ReturnThread( IThread *thread );

      Task *GetWaitingTask();

      bool IsValidTaskID(unsigned int taskID) const;
      bool IsWorkingTaskID(unsigned int taskID) const;

      bool ShuttingDown() const;

      void WaitForAbortedTask(unsigned int taskID);
      void WaitForAllTasks();
      void WaitForCriticalTasks();

      unsigned int FinishedTaskCount() const;
      unsigned int CriticalTaskCount() const;
      unsigned int WorkingTaskCount() const;
      unsigned int TotalTaskCount() const;

      unsigned int IdleThreadCount() const;
      unsigned int WorkingThreadCount() const;
      unsigned int TotalThreadCount() const;

    private:
      TaskList      m_waitingTasks;
      TaskList      m_workingTasks;
      TaskList      m_finishedTasks;
      ThreadList    m_threads;

      unsigned int  m_activeThreadCount;

      unsigned int  m_criticalTaskCount;
      unsigned int  m_finishedTaskCount;
      unsigned int  m_totalTaskCount;

      bool          m_shutdown;

      mutable CriticalSection m_cs_finishedTaskCount;
      mutable CriticalSection m_cs_totalTaskCount;
      mutable CriticalSection m_cs_tasks;
  };

  template <typename T>
  unsigned int TaskManager::AddTask(
    void (*function)(const T&),
    const T& parameter,
    bool critical,
    unsigned int dependency )
  {
    Task::OnFinishDelegate defaultDelegate;
    return AddTask(function, parameter, defaultDelegate, critical, dependency);
  }

  template <typename T>
  unsigned int TaskManager::AddTask(
    void (*function)(const T&),
    const T & parameter,
    Task::OnFinishDelegate onFinish,
    bool critical,
    unsigned int dependency )
  {
    if( !function )
      throw("TaskManager Error: Attempt to add task with no task callback.");

    m_cs_tasks.Enter();

    Task *newTask = new Task();
    newTask->Create( function, parameter, onFinish );

    newTask->SetCritical( critical );
    newTask->SetDependency( dependency );

    m_waitingTasks.push_back(newTask);

    // Run a thread to pick up the task we just added
    RunThread();

    m_cs_tasks.Leave();

    m_cs_totalTaskCount.Enter();
      ++m_totalTaskCount;

      if(critical)
        ++m_criticalTaskCount;
    m_cs_totalTaskCount.Leave();

    return newTask->ID();
  }

} // Threading namespace
} // SF namespace

#endif // SF_TASKMANAGER_H
