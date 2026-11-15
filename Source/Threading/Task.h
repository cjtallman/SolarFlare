/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Task.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/02/2009 @ 18:18:45 - nwilliam
**  \brief   Wrapper around a generic 'task', fully defined externally.
*******************************************************************************/

#ifndef SF_TASK_H
#define SF_TASK_H

#include "Utilities/Delegates.h"

namespace SF {
namespace Threading
{
  class Task
  {
    public:
      typedef Utilities::Delegate0<void> OnFinishDelegate;

      Task();
      ~Task();

      template< typename T >
      void Create( void (*Function)(const T&), const T& param, OnFinishDelegate onFinish );

      void Abort();
      bool Aborted() const;

      bool Critical() const;
      void SetCritical(bool critical);

      unsigned int GetDependency() const;
      void SetDependency(unsigned int taskID);

      unsigned int ID() const;

      void DoTask();
      void OnFinish();

    private:
      typedef void (*GenericUnaryFunction)(void *);
      typedef void (*TaskFunction)(GenericUnaryFunction, void *);

      template< typename T >
      static void ParamDeleteWrapper( T* param );

      template <typename T>
      static void FunctionWrapper(GenericUnaryFunction, void *param);

      unsigned int          m_id;
      bool                  m_aborted;
      bool                  m_critical;
      unsigned int          m_dependency;

      void *                m_parameter;
      GenericUnaryFunction  m_deleteFunction;

      GenericUnaryFunction  m_function;
      TaskFunction          m_functionWrapper;

      OnFinishDelegate      m_onFinish;
  };

  template< typename T >
  void Task::ParamDeleteWrapper( T* param )
  {
    delete param;
  }

  template <typename T>
  void Task::FunctionWrapper(GenericUnaryFunction func, void *param)
  {
    if( func )
      reinterpret_cast<void (*)(const T&)>(func)( *static_cast<T *>( param ) );
  }

  template< typename T >
  void Task::Create( void (*function)(const T&), const T& param, OnFinishDelegate onFinish )
  {
    m_parameter = new T( param );

    void (*Func)(T*) = Task::ParamDeleteWrapper<T>;

    m_deleteFunction = reinterpret_cast<GenericUnaryFunction>( Func );

    m_function = reinterpret_cast<GenericUnaryFunction>( function );
    m_functionWrapper = Task::FunctionWrapper<T>;

    m_onFinish = onFinish;
  }


  #if defined( _MSC_VER )
    unsigned long __stdcall ExecuteTask(void *param);
  #elif defined( __GNUC__ )
    void* ExecuteTask(void *param);
  #else
    #error "Task never compiled with this compiler"
  #endif

}
}

#endif
