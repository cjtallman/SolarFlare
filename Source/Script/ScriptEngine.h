/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ScriptEngine.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/04/2009 @ 19:08:27 - nwilliam
**  \brief   Lua based script engine
*******************************************************************************/

#ifndef SF_SCRIPT_ENGINE_H
#define SF_SCRIPT_ENGINE_H

#include <list>
#include "CScript.h"
#include "Engine/Entity.h"
#include "Engine/BaseEngine.h"
#include "External/Lua/include/LunarHelper.h"

// Forward declaration
namespace ticpp { class Element; }
namespace SF { namespace Engine { namespace Assets { struct ScriptAsset; } } }

namespace SF {
namespace Script
{
  class ScriptEngine : public Engine::BaseEngine
  {
    public:
      ScriptEngine();
      ~ScriptEngine();

      void Update( float elapsed );

      lua_State* MasterState();
      const std::string& LastHandledError() const;
      bool DoFile( const char* errorPrefix, const char* path );
      bool DoString( const char* errorPrefix, const char* str );

      void CScriptCreateTrack( Engine::EntityID entity );
      void CScriptDeleteTrack( Engine::EntityID entity );
      Engine::BaseComponent* CreateCScript( const ticpp::Element* element );

      template< typename T >
      void RegisterGlobal( T* ptr , const char* nameInGlobalTable );
      void HandleLuaError( lua_State* ls, const char* info, bool trace, int regIndex = LUA_NOREF );

    private:
      friend struct Engine::Assets::ScriptAsset;
      typedef std::list< CScript* > CScriptList;

      ScriptEngine( const ScriptEngine& );            // No copy
      ScriptEngine& operator=( const ScriptEngine& ); // No assignment

      void CheckNeededAssets();
      void RegisterScriptClasses();

      lua_State* m_masterState;
      CScriptList m_needsAssets;
      std::string m_lastHandledError;

      Engine::EntityID m_curUpdate;
      Engine::EntityIDList m_entities;


      LUA_DECLARE_CLASS( ScriptEngine )
      LUA_DECLARE_METHOD( LCurUpdateID )
      LUA_DECLARE_METHOD( LReloadScripts )
  };

} // Script namespace
} // SF namespace




namespace SF {
namespace Script
{
  // Template function, implementation
  template< typename T >
  void ScriptEngine::RegisterGlobal( T* ptr , const char* nameInGlobalTable )
  {
    Lunar<T>::push( m_masterState, ptr );
    lua_setglobal( m_masterState, nameInGlobalTable );
  }
}
}

#endif // SF_SCRIPT_ENGINE_H
