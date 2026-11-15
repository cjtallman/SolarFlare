/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Entity.h
**  \author  Patrick Mera, Nathan williams
**  \par     email: pmera@digipen.edu, nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/06/2009 @ 18:43:14 - nwilliam
**  \brief   Basic game entity.
*******************************************************************************/

#ifndef SF_ENTITY_H
#define SF_ENTITY_H

#include <list>
#include "Message.h"
#include "BaseComponent.h"
#include "Script/ScriptHelper.h"

namespace SF {
namespace Engine
{
  typedef int EntityID;
  typedef std::list< EntityID > EntityIDList;

  class Entity
  {
    public:
      Entity();

      EntityID ID() const;
      const std::string& DefType() const;

      bool InitComponents();
      void PushToLua( lua_State* ls );
      bool Has( ComponentType::ID type ) const;
      void HandleMessage( const Engine::Message* msg );
      BaseComponent* GetFirstComponent( ComponentType::ID type );
      ComponentList& GetComponentsByType( ComponentType::ID type );
      BaseComponent* GetComponentByName( ComponentType::ID type, const std::string& name );

      // Networking
      void DeltaIn( Networking::ByteStream & bs );
      void DeltaOut( Networking::ByteStream & bs ) const;
      void InitIn( Networking::ByteStream& bs, bool initial );
      void InitOut( Networking::ByteStream& bs, bool initial ) const;

    private:
      typedef std::list< Script::LuaValue > LuaValueList;
      friend class EntityFactory;
      friend class Lunar<Entity>;  // Due to destrutor

      Entity( const Entity& rhs );
      ~Entity();

      void SetID( EntityID newID );
      void SetLuaData( lua_State* ls );
      void AddComponent( BaseComponent* comp );

      EntityID m_id;
      std::string m_defType;
      Script::LuaRef m_luaObject;
      LuaValueList m_onCreateParams;
      ComponentList m_components[ ComponentType::LAST ];


      LUA_DECLARE_CLASS( Entity )
      LUA_DECLARE_METHOD( LID )
      LUA_DECLARE_METHOD( LType )
      LUA_DECLARE_METHOD( LGetLuaData )
      LUA_DECLARE_METHOD( LSetLuaData )
      LUA_DECLARE_METHOD( LHasComponent )
      LUA_DECLARE_METHOD( LGetFirstComponent )
      LUA_DECLARE_METHOD( LGetComponentByName )
      LUA_DECLARE_METHOD( LGetComponentsByType )
  };

} // Engine namespace
} // SF namespace

#endif // SF_ENTITY_H
