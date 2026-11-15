/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CScript.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/07/2009 @ 17:28:25 - nwilliam
**  \brief   Lua based script wrapper.
*******************************************************************************/

#ifndef SF_C_SCRIPT_H
#define SF_C_SCRIPT_H

#include "Engine/Message.h"
#include "Script/ScriptHelper.h"
#include "Engine/BaseComponent.h"

namespace SF {

// Forward declaration
namespace Engine { class Entity; }

namespace Script
{
  class CScript : public Engine::BaseComponent
  {
    public:
      virtual ~CScript();
      
      int Update( float elapsed );
      virtual BaseComponent* Clone() const;
      virtual void PushToLua( lua_State* ls );
      void HandleMessage( const Engine::Message* msg );
      virtual bool InitDependencies( Engine::Entity* owner );
      
    private:
      friend class ScriptEngine;
      enum State { NOT_LOADED, WAIT_FRAME, WAIT_TIME, DONE };
      
      void Init();
      int ResumeScript();
      
      CScript();
      CScript( const CScript& rhs );
      CScript& operator=( const CScript& rhs );   // No assignment

      float m_wait;
      State m_state;
      std::string m_asset;

      LuaRef m_thread;
      lua_State* m_threadState;

      LuaFunction m_update;
      LuaFunction m_msgHandler;

      LUA_DECLARE_CLASS( CScript )
      LUA_DECLARE_METHOD( LSetUpdate )
      LUA_DECLARE_METHOD( LSetMessageHandler )
  };
  
} // Script namespace
} // SF namespace

#endif // SF_C_SCRIPT_H
