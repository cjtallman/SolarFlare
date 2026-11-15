/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ScriptEngine.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 10:52:44 - nwilliam
**  \brief   Implementation of Lua based script engine.
*******************************************************************************/

#include <sstream>
#include "CScript.h"
#include "GameLib.h"
#include "ScriptEngine.h"
#include "Utilities/Asserts.h"
#include "SolarFlare/Globals.h" // console, entityFactory, assetManager
#include "Engine/AssetManager.h"
#include "Utilities/DevConsole.h"
#include "Engine/EntityFactory.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace
{
  void OpenAllLibs( lua_State* ls )
  {
    lua_CFunction stdLibs[] = { luaopen_base,
                                luaopen_string,
                                luaopen_math,
                                luaopen_io,
                                luaopen_os,
                                //luaopen_debug,
                                luaopen_table,
                                //luaopen_package,
                                0 
                               };

    for( int i = 0; stdLibs[i]; ++i )
    {
      lua_pushcfunction( ls, stdLibs[i] );
      lua_call( ls, 0, 0 );
    }

    SF::Script::GameLib::OpenGameLib( ls );
  }

  int SF_LuaPanic( lua_State* ls )
  {
    std::string msg( "Lua Panic - " );

    if( lua_gettop( ls ) > 0 )
    {
      const char* str = lua_tostring( ls, -1 );
      msg += str;
    }

    throw( msg );
  }
}


namespace SF {
namespace Script
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  ScriptEngine::ScriptEngine()
  {
    m_masterState = luaL_newstate();
    OpenAllLibs( m_masterState );
    RegisterScriptClasses();

    lua_atpanic( m_masterState, SF_LuaPanic );

    Globals::entityFactory->Creator( "SCRIPT", Engine::CCreator::Method< ScriptEngine, &ScriptEngine::CreateCScript >( this ) );
    Globals::entityFactory->CreateNotifier( Engine::ComponentType::SCRIPT, Engine::Notifier::Method< ScriptEngine, &ScriptEngine::CScriptCreateTrack >( this ) );
    Globals::entityFactory->DeleteNotifier( Engine::ComponentType::SCRIPT, Engine::Notifier::Method< ScriptEngine, &ScriptEngine::CScriptDeleteTrack >( this ) );
  }

  ScriptEngine::~ScriptEngine()
  {
    lua_close( m_masterState );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void ScriptEngine::Update( float elapsed )
  {
    using namespace Engine;

    // Update entities
    for( EntityIDList::iterator eit = m_entities.begin(); eit != m_entities.end(); ++eit )
    {
      m_curUpdate = *eit;

      Entity* entity = Globals::entityFactory->GetEntity( *eit );

      if( entity )
      {
        ComponentList& list = entity->GetComponentsByType( ComponentType::SCRIPT );

        for( ComponentList::iterator lit = list.begin(); lit != list.end(); ++lit )
        {
          CScript* const sc = static_cast< CScript *>( *lit );

          if( sc->Update( elapsed ) == LUA_ERRRUN )
          {
            sc->m_state = CScript::DONE;
            HandleLuaError( sc->m_threadState, 0, true );
          }
        }
      }
    }
    
    CheckNeededAssets();
  }

  lua_State* ScriptEngine::MasterState()
  {
    return m_masterState;
  }

  const std::string& ScriptEngine::LastHandledError() const
  {
    return m_lastHandledError;
  }

  bool ScriptEngine::DoFile( const char* errorPrefix, const char* path )
  {
    int code = luaL_dofile( m_masterState, path );

    if( code )
    {
      HandleLuaError( m_masterState, errorPrefix, false );
    }

    return code != 0;
  }

  bool ScriptEngine::DoString( const char* info, const char* str )
  {
    int code = luaL_dostring( m_masterState, str );

    if( code )
    {
      HandleLuaError( m_masterState, info, false );
    }

    return code != 0;
  }

  void ScriptEngine::CScriptCreateTrack( Engine::EntityID entity )
  {
    m_entities.push_back( entity );
  }

  void ScriptEngine::CScriptDeleteTrack( Engine::EntityID entity )
  {
    m_entities.remove( entity );
  }

  Engine::BaseComponent* ScriptEngine::CreateCScript( const ticpp::Element* element )
  {
    std::string asset;
    element->GetAttribute( "asset", &asset, false );

    CScript* bc = 0;

    if( asset.length() != 0 )
    {
      using namespace Engine;
      using namespace Assets;

      ScriptAsset* sa = static_cast<ScriptAsset*>( Globals::assetManager->GetAsset( asset ) );

      if( sa )
      {
        if( sa->loadStatus == -1 )
        {
          // Bad file, error will be in console
        }
        else if( sa->loadStatus == 0 )
        {
          CScript* cs = new CScript();
          m_needsAssets.push_back( cs );
          bc = cs;
        }
        else if( sa->loadStatus == 1 )
        {
          CScript* cs = new CScript();
          cs->m_update = sa->mainFunc;
          cs->m_msgHandler = sa->msgHandler;
          cs->Init();
          bc = cs;
        }
      }
      else
      {
        std::stringstream msg;
        msg << "Unknown asset (" << asset << ", line " << element->Row() << ")";
        Globals::console->Error( msg.str() );
      }
    }
    else
    {
      bc = new CScript();
    }

    return bc;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////

  // ScriptEngine::RegisterScriptClasses() is in BoundClasses.cpp

  void ScriptEngine::CheckNeededAssets()
  {
    CScriptList::iterator it = m_needsAssets.begin();

    while( it != m_needsAssets.end() )
    {
      using namespace Engine;
      using namespace Assets;

      ScriptAsset* sa = static_cast<ScriptAsset*>( Globals::assetManager->GetAsset( (*it)->m_asset ) );

      if( sa && sa->loadStatus == 1 )
      {
        (*it)->m_update = sa->mainFunc;
        (*it)->m_msgHandler = sa->msgHandler;
        (*it)->Init();
        
        it = m_needsAssets.erase( it );
      }
      else
      {
        ++it;
      }
    }
  }

  void ScriptEngine::HandleLuaError( lua_State* ls, const char* info, bool trace, int regIndex )
  {
    const std::string indent( "   " );
    const std::string infoHeader( "From: " );
    const std::string traceHeader( "Trace:" );
    const std::string funcHeader( "Function:" );

    if( lua_gettop( ls ) > 0 )
    {
      std::vector< std::string > infoDump;

      m_lastHandledError = lua_tostring( ls, -1 );
      lua_pop( ls, 1 );

      infoDump.push_back( m_lastHandledError );

      if( info )
        infoDump.push_back( infoHeader + std::string( info ) );

      if( regIndex != LUA_NOREF )
      {
        infoDump.push_back( funcHeader );

        lua_Debug ar;
        lua_rawgeti( ls, LUA_REGISTRYINDEX, regIndex );
        lua_getinfo( ls, ">Snl", &ar );

        std::stringstream msg;
        msg << "Name: ";

        if( ar.name )
          msg << ar.name << " (" << ar.namewhat << ")";
        else
          msg << "(No Guess)";

        infoDump.push_back( indent + msg.str() );

        msg.str( "" );
        msg << "Location: " << ar.short_src << ":" << ar.linedefined;

        infoDump.push_back( indent + msg.str() );
      }

      if( trace )
      {
        lua_Debug ar;
        int depth = -1;
        std::stringstream msg;

        infoDump.push_back( traceHeader );

        while( lua_getstack( ls, depth, &ar ) )
        {
          lua_getinfo( ls, "Sln", &ar );

          std::stringstream msg;
          msg << "Function: ";

          if( ar.name )
            msg << ar.name << " (" << ar.namewhat << ")";
          else
            msg << "(No Info)";

          infoDump.push_back( indent + msg.str() );

          msg.str( "" );
          msg << "Location: " << ar.short_src << ":" << ar.linedefined;

          infoDump.push_back( indent + indent + msg.str() );

          depth++;
        }
      }

      int removeLens[] = { 3, 7, 8 };
      const char* removeStrs[] = { "../", "Assets/", "Scripts/" };

      for( size_t i = 0; i < infoDump.size(); ++i )
      {
        for( int j = 0; j < 3; ++j )
        {
          size_t pos = infoDump[i].find( removeStrs[j] );

          if( pos != std::string::npos )
            infoDump[i].erase( pos, removeLens[j] );
        }

        Globals::console->Error( indent + infoDump[i] );
      }
    }
  }




  //////////////////////////////////////////////////////////////////////////////
  // Lua Methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( ScriptEngine )
    LUA_NAME_METHOD( ScriptEngine, "CurUpdateID",   LCurUpdateID )
    LUA_NAME_METHOD( ScriptEngine, "ReloadScripts", LReloadScripts )
  LUA_END_DEFINE_CLASS

  LUA_DEFINE_METHOD( ScriptEngine, LCurUpdateID )
  {
    lua_pushinteger( ls, m_curUpdate );
    return 1;
  }

  LUA_DEFINE_METHOD_NOLS( ScriptEngine, LReloadScripts )
  {
    Globals::assetManager->ReloadType( Engine::Assets::T_SCRIPT );
    return 0;
  }

} // Script namespace
} // SF namespace
