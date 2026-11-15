/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Delegates.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM3300/350
**  \par     Updated: 01/16/2009 @ 13:06:40 - nwilliam
**  \brief   Simple, low overhead delegate implementation parametized on return
             and up to 3 paramater types. Works with both methods on classes of 
             no, single, and multiple inheritence and with regular functions.
*******************************************************************************/

#ifndef SF_DELEGATES_H
#define SF_DELEGATES_H

namespace SF {
namespace Utilities
{
  //////////////////////////////////////////////////////////////////////////////
  // Delegate0 - Delegate that takes no parameters
  //             ex: Delegate0<void> (returns void)
  //                 Delegate0<void> d = Delegate1<void>::Function< Foo >
  //////////////////////////////////////////////////////////////////////////////
  template< typename TReturn >
  class Delegate0
  {
    public:
      Delegate0() : m_obj(0), m_func(0) {}
      
      template< typename T, TReturn (T::*MP)( void ) >
      static Delegate0<TReturn> Method( T* cp )
      {
        Delegate0<TReturn> d;
        d.m_obj = cp;
        d.m_func = &FuncPtr_Method< T, MP >;
        return d;      
      }
      
      template< TReturn (*Func)( void ) >
      static Delegate0<TReturn> Function()
      {
        Delegate0<TReturn> d;
        d.m_func = &FuncPtr_Function< Func >;
        return d;
      }
      
      TReturn operator()() const
      {
        return m_func( m_obj );
      }
      
      operator bool() const
      {
        return m_func ? true : false;
      }
      
    private:
      typedef TReturn (*FuncPtr)( void* );
      
      void* m_obj;
      FuncPtr m_func;
      
      template< typename T, TReturn (T::*MP)( void ) >
      static TReturn FuncPtr_Method( void* obj )
      {
        T* const p = static_cast< T* >( obj );
        return (p->*MP)();
      }
      
      template< TReturn (*Func)( void ) >
      static TReturn FuncPtr_Function( void* )
      {
        return (Func)();
      }
  };
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Delegate1 - Delegate that takes 1 paramater
  //             ex: Delegate1<void,int> (returns void, takes int)
  //                 Delegate1<void,int> d = Delegate1<void,int>::Function< Foo1 >
  //////////////////////////////////////////////////////////////////////////////
  template< typename TReturn, typename TParam1 >
  class Delegate1
  {
    public:
      Delegate1() : m_obj(0), m_func(0) {}
      
      template< typename T, TReturn (T::*MP)( TParam1 ) >
      static Delegate1<TReturn,TParam1> Method( T* cp )
      {
        Delegate1<TReturn,TParam1> d;
        d.m_obj = cp;
        d.m_func = &FuncPtr_Method< T, MP >;
        return d;      
      }
      
      template< TReturn (*Func)( TParam1 ) >
      static Delegate1<TReturn,TParam1> Function()
      {
        Delegate1<TReturn,TParam1> d;
        d.m_func = &FuncPtr_Function< Func >;
        return d;
      }
      
      TReturn operator()( TParam1 p1 ) const
      {
        return m_func( m_obj, p1 );
      }
      
      operator bool() const
      {
        return m_func ? true : false;
      }
      
    private:
      typedef TReturn (*FuncPtr)( void*, TParam1 );
      
      void* m_obj;
      FuncPtr m_func;
      
      template< typename T, TReturn (T::*MP)( TParam1 ) >
      static TReturn FuncPtr_Method( void* obj, TParam1 p1 )
      {
        T* const p = static_cast< T* >( obj );
        return (p->*MP)( p1 );
      }
      
      template< TReturn (*Func)( TParam1 ) >
      static TReturn FuncPtr_Function( void* , TParam1 p1 )
      {
        return (Func)( p1 );
      }
  };
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Delegate2 - Delegate that takes 2 paramaters
  //             ex: Delegate2<void,int,bool> (returns void, takes int and bool)
  //                 Delegate2<void,int,bool> d = Delegate2<void,int,bool>::Function< Foo2 >
  //////////////////////////////////////////////////////////////////////////////
  template< typename TReturn, typename TParam1, typename TParam2 >
  class Delegate2
  {
    public:
      Delegate2() : m_obj(0), m_func(0) {}
      
      template< typename T, TReturn (T::*MP)( TParam1, TParam2 ) >
      static Delegate2<TReturn,TParam1,TParam2> Method( T* cp )
      {
        Delegate2<TReturn,TParam1,TParam2> d;
        d.m_obj = cp;
        d.m_func = &FuncPtr_Method< T, MP >;
        return d;      
      }
      
      template< TReturn (*Func)( TParam1, TParam2 ) >
      static Delegate2<TReturn,TParam1,TParam2> Function()
      {
        Delegate2<TReturn,TParam1,TParam2> d;
        d.m_func = &FuncPtr_Function< Func >;
        return d;
      }
      
      TReturn operator()( TParam1 p1, TParam2 p2 ) const
      {
        return m_func( m_obj, p1, p2 );
      }
      
      operator bool() const
      {
        return m_func ? true : false;
      }
      
    private:
      typedef TReturn (*FuncPtr)( void*, TParam1, TParam2 );
      
      void* m_obj;
      FuncPtr m_func;
      
      template< typename T, TReturn (T::*MP)( TParam1, TParam2 ) >
      static TReturn FuncPtr_Method( void* obj, TParam1 p1, TParam2 p2 )
      {
        T* const p = static_cast< T* >( obj );
        return (p->*MP)( p1, p2 );
      }
      
      template< TReturn (*Func)( TParam1, TParam2 ) >
      static TReturn FuncPtr_Function( void* , TParam1 p1, TParam1 p2 )
      {
        return (Func)( p1, p2 );
      }
  };
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Delegate3 - Delegate that takes 3 paramaters
  //             ex: Delegate3<void,int,bool,float> (returns void, takes int, bool, and float)
  //                 Delegate3<void,int,bool,float> d = Delegate3<void,int,bool,float>::Function< Foo3 >
  //////////////////////////////////////////////////////////////////////////////
  template< typename TReturn, typename TParam1, typename TParam2, typename TParam3 >
  class Delegate3
  {
    public:
      Delegate3() : m_obj(0), m_func(0) {}
      
      template< typename T, TReturn (T::*MP)( TParam1, TParam2, TParam3 ) >
      static Delegate3<TReturn,TParam1,TParam2,TParam3> Method( T* cp )
      {
        Delegate3<TReturn,TParam1,TParam2,TParam3> d;
        d.m_obj = cp;
        d.m_func = &FuncPtr_Method< T, MP >;
        return d;      
      }
      
      template< TReturn (*Func)( TParam1, TParam2, TParam3 ) >
      static Delegate3<TReturn,TParam1,TParam2,TParam3> Function()
      {
        Delegate3<TReturn,TParam1,TParam2,TParam3> d;
        d.m_func = &FuncPtr_Function< Func >;
        return d;
      }
      
      TReturn operator()( TParam1 p1, TParam2 p2, TParam3 p3 ) const
      {
        return m_func( m_obj, p1, p2, p3 );
      }
      
      operator bool() const
      {
        return m_func ? true : false;
      }
      
    private:
      typedef TReturn (*FuncPtr)( void*, TParam1, TParam2, TParam3 );
      
      void* m_obj;
      FuncPtr m_func;
      
      template< typename T, TReturn (T::*MP)( TParam1, TParam2, TParam3 ) >
      static TReturn FuncPtr_Method( void* obj, TParam1 p1, TParam2 p2, TParam3 p3 )
      {
        T* const p = static_cast< T* >( obj );
        return (p->*MP)( p1, p2, p3 );
      }
      
      template< TReturn (*Func)( TParam1, TParam2, TParam3 ) >
      static TReturn FuncPtr_Function( void* , TParam1 p1, TParam1 p2, TParam3 p3 )
      {
        return (Func)( p1, p2, p3 );
      }
  };
  
} // Utilities namespace
} // SF namespace

#endif // SF_DELEGATES_H
