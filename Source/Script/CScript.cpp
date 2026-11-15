/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CScript.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/07/2009 @ 17:38:41 - nwilliam
**  \brief   Implementation of Lua based script component.
*******************************************************************************/

#include "CScript.h"
#include "GameLib.h"
#include "ScriptEngine.h"
#include "Engine/Message.h"
#include "SolarFlare/Globals.h"   // console, scriptEngine
#include "Engine/MessageEngine.h"
#include "Utilities/DevConsole.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Script
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors / Destructor
  //////////////////////////////////////////////////////////////////////////////
  CScript::CScript() : BaseComponent( Engine::ComponentType::SCRIPT ),
                       m_wait( 0.0f ),
                       m_state( CScript::NOT_LOADED ),
                       m_threadState( 0 )
  {
  }

  CScript::CScript( const CScript& rhs ) : BaseComponent( Engine::ComponentType::SCRIPT ),
                                           m_wait( 0.0f ),
                                           m_state( CScript::NOT_LOADED ),
                                           m_update( rhs.m_update ),
                                           m_msgHandler( rhs.m_msgHandler ),
                                           m_threadState( 0 )
  {
    Init();
  }

  CScript::~CScript()
  {
    m_threadState = 0;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void CScript::Init()
  {
    m_state = DONE;

    if( m_update.Valid() )
    {
      lua_State* master = Globals::scriptEngine->MasterState();
      m_threadState = lua_newthread( master );
      m_thread.Set( master );

      m_wait = 0.0f;
      m_state = WAIT_FRAME;
      m_update.Get( m_threadState );
    }
  }

  int CScript::ResumeScript()
  {
    int result = lua_resume( m_threadState, 0 );
    
    if( result == LUA_YIELD )
    {
      m_state = static_cast<State>( WAIT_FRAME + luaL_checkint( m_threadState, 1 ) );
      m_wait  = static_cast<float>( luaL_checknumber( m_threadState, 2 ) );
    }
    else if( result == 0 )
    {
      m_state = CScript::DONE;
    }

    return result;
  }
  
  int CScript::Update( float elapsed )
  {
    bool resume = false;
    
    switch( m_state )
    {
      case WAIT_FRAME: resume = ( (m_wait -= 1.0f)    <= 0 ); break;
      case WAIT_TIME:  resume = ( (m_wait -= elapsed) <= 0 ); break;
    }
    
    return resume ? ResumeScript() : 0;
  }
  
  Engine::BaseComponent* CScript::Clone() const
  {
    return new CScript( *this );
  }
  
  void CScript::HandleMessage( const Engine::Message* msg )
  {
    if( m_msgHandler.Valid() )
    {
      // NOTE: If MessageStack::PushToLua ever changes (ie, pushes something 
      //       other than a table of its elements), this won't behave correctly
      
      // Push message type and stack
      lua_State* ls = Globals::scriptEngine->MasterState();

      lua_pushstring( ls, msg->type.c_str() );
      msg->stack.PushToLua( ls );
      
      if( m_msgHandler( ls, 2 ) )
      {
        Globals::scriptEngine->HandleLuaError( ls, "HandleMessage", true );
      }
    }
  }
  
  bool CScript::InitDependencies( Engine::Entity* )
  {
    return true;  // No dependencies
  }

  void CScript::PushToLua( lua_State* ls )
  {
    Lunar<CScript>::push( ls, this );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Lua bound methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( CScript )
    LUA_NAME_METHOD( CScript, "SetUpdate", LSetUpdate )
    LUA_NAME_METHOD( CScript, "SetMessageHandler", LSetMessageHandler )
  LUA_END_DEFINE_CLASS

  LUA_DEFINE_METHOD( CScript, LSetUpdate )
  {
    if( m_update.Set( ls ) )
    {
      Init();
    }

    return 0;
  }

  LUA_DEFINE_METHOD( CScript, LSetMessageHandler )
  {
    m_msgHandler.Set( ls );
    return 0;
  }
  
} // Script namespace
} // SF namespace
