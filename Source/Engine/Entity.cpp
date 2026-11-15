/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Entity.cpp
**  \author  Patrick Mera, Nathan williams
**  \par     email: pmera@digipen.edu, nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/06/2009 @ 18:43:05 - nwilliam
**  \brief   Implementation of game entity
*******************************************************************************/

#include "Entity.h"
#include "Script/CScript.h"       // CScript, handling messages
#include "SolarFlare/Globals.h"   // ScriptEngine 
#include "Script/ScriptEngine.h"
#include "Engine/EntityFactory.h" // CallOnCreate

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Engine
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  Entity::Entity() : m_id( -1 )
  {
  }

  Entity::Entity( const Entity& rhs ) : m_id( -1 ),
                                        m_defType( rhs.m_defType )
  {
    for( int i = 0; i < ComponentType::LAST; ++i )
    {
      ComponentList::const_iterator it = rhs.m_components[i].begin();

      while( it != rhs.m_components[i].end() )
      {
        m_components[i].push_back( (*it)->Clone() );
        ++it;
      }
    }
  }

  Entity::~Entity()
  {
    for( int i = 0; i < ComponentType::LAST; ++i )
    {
      ComponentList::iterator it = m_components[i].begin();

      while( it != m_components[i].end() )
      {
        delete *it;
        ++it;
      }
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  EntityID Entity::ID() const
  {
    return m_id;
  }

  const std::string& Entity::DefType() const
  {
    return m_defType;
  }

  bool Entity::InitComponents()
  {
    bool result = true;

    for( int i = 0; i < ComponentType::LAST; ++i )
    {
      ComponentList::iterator it = m_components[i].begin();

      while( it != m_components[i].end() )
      {
        result &= (*it)->InitDependencies( this );
        ++it;
      }
    }

    return result;
  }

  void Entity::PushToLua( lua_State* ls )
  {
    Lunar<Entity>::push( ls, this );
  }

  bool Entity::Has( ComponentType::ID type ) const
  {
    return m_components[type].size() != 0;
  }

  void Entity::HandleMessage( const Engine::Message* msg )
  {
    if( !Has( Engine::ComponentType::SCRIPT ) )
    {
      return;  // No script, currently only way to handle message
    }

    ComponentList& list = GetComponentsByType( Engine::ComponentType::SCRIPT );

    for( ComponentList::iterator it = list.begin(); it != list.end(); ++it )
    {
      Script::CScript* const cs = static_cast<Script::CScript*>( *it );
      cs->HandleMessage( msg );
    }
  }

  BaseComponent* Entity::GetFirstComponent( ComponentType::ID type )
  {
    if( !Has( type ) )
    {
      return 0;
    }

    return *( m_components[type].begin() );
  }

  ComponentList& Entity::GetComponentsByType( ComponentType::ID type )
  {
    return m_components[type];
  }

  BaseComponent* Entity::GetComponentByName( ComponentType::ID type, const std::string& name )
  {
    BaseComponent* comp = 0;
    
    ComponentList& list = m_components[type];
    
    for( ComponentList::iterator it = list.begin(); it != list.end(); ++it )
    {
      if( (*it)->name == name )
      {
        comp = *it;
        break;
      }
    }
    
    return comp;
  }

  void Entity::DeltaIn( Networking::ByteStream& bs )
  {
    for( int i = 0; i < ComponentType::LAST; ++i )
    {
      for( ComponentList::iterator it = m_components[i].begin(); it != m_components[i].end(); ++it )
      {
        (*it)->DeltaIn( bs );
      }
    }
  }

  void Entity::DeltaOut( Networking::ByteStream & bs ) const
  {
    for( int i = 0; i < ComponentType::LAST; ++i )
    {
      for( ComponentList::const_iterator it = m_components[i].begin(); it != m_components[i].end(); ++it )
      {
        (*it)->DeltaOut( bs );
      }
    }
  }

  void Entity::InitIn( Networking::ByteStream& bs, bool initial )
  {
    for( int i = 0; i < ComponentType::LAST; ++i )
    {
      for( ComponentList::iterator it = m_components[i].begin(); it != m_components[i].end(); ++it )
      {
        (*it)->InitIn( bs );
      }
    }

    if( initial )
    {
      uint8 paramSize = 0;
      bs >> paramSize;

      for( uint8 i = 0; i < paramSize; ++i )
      {
        Script::LuaValue lv;
        bs >> lv;
        m_onCreateParams.push_back( lv );
      }

      Globals::entityFactory->CallOnCreate( Globals::scriptEngine->MasterState(), this );
    }

    /*
    Note: This isn't required yet, but if it is this should work
    
    if( m_luaObject.Valid() )
    {
      uint8 tblSize = 0;
      bs >> tblSize;

      lua_State* ls = m_luaObject.State();
      m_luaObject.Get( ls );
      lua_pushlstring( ls, "SyncIn", 6 );
      lua_gettable( ls, -2 );

      if( lua_isfunction( ls, -1 ) )
      {
        lua_createtable( ls, tblSize, 0 );

        int count = 1;
        while( count <= tblSize )
        {
          Script::LuaValue val;
          bs >> val;

          lua_pushnumber( ls, count );
          val.ToLua( ls );
          lua_settable( ls, -3 );

          ++count;
        }

        if( lua_pcall( ls, 1, 0, 0 ) )
        {
          Globals::scriptEngine->HandleLuaError( ls, "Entity SyncIn", false );
        }
      }
      else
      {
        lua_pop( ls, 1 ); // pop nil (or whatever SyncIn was)
      }

      lua_pop( ls, 1 ); // pop m_luaObject
    }
    */
  }

  void Entity::InitOut( Networking::ByteStream& bs, bool initial ) const
  {
    for( int i = 0; i < ComponentType::LAST; ++i )
    {
      for( ComponentList::const_iterator it = m_components[i].begin(); it != m_components[i].end(); ++it )
      {
        (*it)->InitOut( bs );
      }
    }

    if( initial )
    {
      bs << static_cast<uint8>( m_onCreateParams.size() );

      for( Entity::LuaValueList::const_iterator it = m_onCreateParams.begin(); it != m_onCreateParams.end(); ++it )
      {
        bs << *it;
      }
    }

    /*
    Note: This isn't required yet, but if it is this should work
    
    if( m_luaObject.Valid() )
    {
      lua_State* ls = m_luaObject.State();

      m_luaObject.Get( ls );
      lua_pushlstring( ls, "SyncOut", 7 );
      lua_gettable( ls, -2 );

      if( lua_isfunction( ls, -1 ) )
      {
        if( lua_pcall( ls, 0, 1, 0 ) )
        {
          Globals::scriptEngine->HandleLuaError( ls, "Entity SyncOut", false );
        }
        else
        {
          size_t count = lua_objlen( ls, -1 );

          bs << static_cast<uint8>( count ); // num elements in table;
          for( size_t i = 1; i <= count; ++i )
          {
            lua_pushnumber( ls, i );
            lua_gettable( ls, -2 );

            Script::LuaValue val;
            val.FromLua( ls, -1 );

            bs << val;
          }
        }
      }
      else
      {
        lua_pop( ls, 1 ); // pop nil (or whatever SyncOut was)
      }

      lua_pop( ls, 1 ); // pop m_luaObject
    }
    */
  }



  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void Entity::SetID( EntityID newID )
  {
    m_id = newID;
  }

  void Entity::SetLuaData( lua_State* ls )
  {
    m_luaObject.Set( ls );
  }

  void Entity::AddComponent( BaseComponent* comp )
  {
    // Make sure we have enough spots
    m_components[comp->type].push_back( comp );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Lua methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( Entity )
    LUA_NAME_METHOD( Entity, "ID", LID )
    LUA_NAME_METHOD( Entity, "Type",                LType )
    LUA_NAME_METHOD( Entity, "GetLuaData",          LGetLuaData )
    LUA_NAME_METHOD( Entity, "SetLuaData",          LSetLuaData )
    LUA_NAME_METHOD( Entity, "HasComponent",        LHasComponent )
    LUA_NAME_METHOD( Entity, "GetFirstComponent",   LGetFirstComponent )
    LUA_NAME_METHOD( Entity, "GetComponentByName",  LGetComponentByName )
    LUA_NAME_METHOD( Entity, "GetComponentsByType", LGetComponentsByType )
  LUA_END_DEFINE_CLASS


  LUA_DEFINE_METHOD( Entity, LID )
  {
    lua_pushinteger( ls, ID() );
    return 1;
  }

  LUA_DEFINE_METHOD( Entity, LType )
  {
    lua_pushstring(ls, m_defType.c_str());

    return 1;
  }

  LUA_DEFINE_METHOD( Entity, LGetLuaData )
  {
    m_luaObject.Get( ls );
    return 1;
  }

  LUA_DEFINE_METHOD( Entity, LSetLuaData )
  {
    luaL_checkany( ls, 1 );
    lua_pushvalue( ls, 1 );
    m_luaObject.Set( ls );
    return 0;
  }

  LUA_DEFINE_METHOD( Entity, LHasComponent )
  {
    int opt = luaL_checkoption( ls, -1, 0, ComponentType::name );

    lua_pushboolean( ls, Has( static_cast<ComponentType::ID>( opt ) ) );

    return 1;
  }

  LUA_DEFINE_METHOD( Entity, LGetFirstComponent )
  {
    int opt = luaL_checkoption( ls, -1, 0, ComponentType::name );

    BaseComponent* bc = GetFirstComponent( static_cast<ComponentType::ID>( opt ) );
    bc ? bc->PushToLua( ls ) : lua_pushnil( ls );

    return 1;
  }
  
  LUA_DEFINE_METHOD( Entity, LGetComponentByName )
  {
    int opt = luaL_checkoption( ls, -2, 0, ComponentType::name );
    std::string name = luaL_checkstring( ls, -1 );

    BaseComponent* bc = GetComponentByName( static_cast<ComponentType::ID>( opt ), name );
    bc ? bc->PushToLua( ls ) : lua_pushnil( ls );

    return 1;
  }
  
  LUA_DEFINE_METHOD( Entity, LGetComponentsByType )
  {
    int opt = luaL_checkoption( ls, 1, 0, ComponentType::name );
    ComponentType::ID type = static_cast<ComponentType::ID>( opt );

    ComponentList& compList = GetComponentsByType( type );

    lua_createtable( ls, static_cast<int>( compList.size() ), 0 );

    int counter = 1;
    for( ComponentList::iterator it = compList.begin(); it != compList.end(); ++it )
    {
      (*it)->PushToLua( ls );
      lua_rawseti( ls, -2, counter++ );
    }

    return 1;
  }

  
} // Engine namespace
} // SF namespace
