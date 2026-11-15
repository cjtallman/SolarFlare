/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    EntityFactory.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/26/2009 @ 15:28:14 - nwilliam
**  \brief   Implementation of entity definition loading and creating.
*******************************************************************************/

#include "EntityFactory.h"
#include "SolarFlare/Globals.h"   // console, messageEngine
#include "Script/ScriptEngine.h"
#include "Utilities/DevConsole.h"
#include "Engine/MessageEngine.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Engine
{
  //////////////////////////////////////////////////////////////////////////////
  // Consturctor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  EntityFactory::EntityFactory() : m_curLocalID( 0 ),
                                   m_curNetworkID( 0 )
  {
  }

  EntityFactory::~EntityFactory()
  {
    // Delete Definitions
    for( EntityTable::iterator it = m_entityDefs.begin(); it != m_entityDefs.end(); ++it )
    {
      delete *it;
    }

    m_entityDefs.clear();

    for( EntityMap::iterator it = m_entities.begin(); it != m_entities.end(); ++it )
    {
      Notify( it->second, m_deleteNotifiers );
      delete it->second;
    }

    m_entities.clear();
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  Entity* EntityFactory::GetEntity( int id )
  {
    EntityMap::iterator it = m_entities.find( id );

    if( it != m_entities.end() )
    {
      return it->second;
    }

    return 0;
  }

  Entity* EntityFactory::Create( const std::string& defName )
  {
    Entity* entity = CreateID( defName, 0, true );
    return entity;
  }

  Entity* EntityFactory::CreateID( const std::string& defName, EntityID id, bool local )
  {
    if( m_entities.find( id ) != m_entities.end() )
    {
      Globals::console->Error() << "CreateID called with duplicate id: "
                                << id << std::endl;
      return 0;
    }

    Entity* newEntity = 0;

    EntityTable::iterator entityDef = m_entityDefs.find( defName );

    if( entityDef != m_entityDefs.end() )
    {
      // Locally created etity (from plain Create() func)
      if( id == 0 )
      {
        if( (*entityDef)->Has( Engine::ComponentType::NETWORK ) )
        {
          id = ++m_curNetworkID;
        }
        else
        {
          id = ++m_curLocalID;
        }
      }

      newEntity = new Entity( *(*entityDef) );
      newEntity->SetID( id );
      newEntity->InitComponents();

      m_entities.insert( EntityMap::value_type( id, newEntity ) );

      Notify( newEntity, m_createNotifiers );

      Message* msg = Globals::messageEngine->AddMessage( "EntityCreated" );

      if( local && newEntity->Has( ComponentType::NETWORK ) )
      {
        msg->route = Message::MSG_ALL_PEERS;
      }
      else
      {
        msg->route = Message::MSG_LOCAL;
      }

      msg->stack.Push( defName );
      msg->stack.Push( newEntity->ID() );
    }
    else
    {
      Globals::console->Error() << "Unknown definition: " << defName << std::endl;
    }

    return newEntity;
  }

  void EntityFactory::Reset( EntityID lower )
  {
    m_curNetworkID = lower;

    EntityMap::iterator it = m_entities.begin();

    while( it != m_entities.end() )
    {
      if( it->second->Has( ComponentType::NETWORK ) )
      {
        EntityID id = it->second->ID();
        ++it;
        InternalDeleteEntity( id ); // modifies m_entities
      }
      else
      {
        ++it;
      }
    }
  }

  void EntityFactory::LoadDefinitions( const std::string& defFile )
  {
    try
    {
      ticpp::Document defDoc( defFile );
      defDoc.LoadFile();

      // Make sure top level is EntityDefs (throws if it isn't)
      ticpp::Element* defList = defDoc.FirstChildElement( "EntityDefs" );

      // iterate over all "entity" items
      ticpp::Iterator< ticpp::Element > entity( defList->FirstChildElement( "entity" ), "entity" );

      while( entity != entity.end() )
      {
        std::string name;
        entity->GetAttribute( "name", &name, false );

        if( name.size() != 0 )
        {
          // It was a file, load it as such
          NewEntityDef( name, &( *entity ) );
        }
        else
        {
          Globals::console->Error() << "No definition name, line " 
                                    << entity->Row() << std::endl;
        }

        ++entity;
      }
    }
    catch( const ticpp::Exception& e )
    {
      Globals::console->Error() << "Exception in LoadDefinitions: "
                                << e.what() << std::endl;
    }
  }

  void EntityFactory::Creator( const std::string& name, const CCreator& del )
  {
    if( m_creators.find( name ) == m_creators.end() )
    {
      m_creators.insert( name, del );
    }
    else
    {
      Globals::console->Error() <<  "Creator already registered: "
                                << name << std::endl;
    }
  }

  void EntityFactory::CreateNotifier( ComponentType::ID type, const Notifier& notifier )
  {
    m_createNotifiers.insert( NotifierMap::value_type( type, notifier ) );
  }

  void EntityFactory::DeleteNotifier( ComponentType::ID type, const Notifier& notifier )
  {
    m_deleteNotifiers.insert( NotifierMap::value_type( type, notifier ) );
  }

  void EntityFactory::DeleteEntity( EntityID id )
  {
    m_deleteQueue.push( id );
  }

  void EntityFactory::Update()
  {
    while( !m_deleteQueue.empty() )
    {
      EntityID id = m_deleteQueue.front();
      InternalDeleteEntity( id );
      m_deleteQueue.pop();
    }
  }

  void EntityFactory::CallOnCreate( lua_State* ls, Entity* entity )
  {
    if( !entity )
    {
      return;
    }

    LuaFuncTable::iterator func = m_createFuncs.find( entity->m_defType );

    if( func != m_createFuncs.end() )
    {
      int origTop = lua_gettop( ls );
      entity->PushToLua( ls );

      for( Entity::LuaValueList::iterator it = entity->m_onCreateParams.begin(); it != entity->m_onCreateParams.end(); ++it )
      {
        it->ToLua( ls );
      }

      // Valid arguments + entity
      int args = static_cast<int>( entity->m_onCreateParams.size() ) + 1;

      if( (*func)( ls, args ) )
      {
        Globals::scriptEngine->HandleLuaError( ls, "OnCreate", false );
      }
      else
      {
        if( lua_gettop( ls ) > origTop )
        {
          // OnCreate should return 1 thing at most, if not...tsk tsk
          entity->SetLuaData( ls );
        }
      }

      lua_settop( ls, origTop );
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void EntityFactory::InternalDeleteEntity( EntityID id )
  {
    EntityMap::iterator it = m_entities.find( id );

    if( it != m_entities.end() )
    {
      Notify( it->second, m_deleteNotifiers );

      LuaFuncTable::iterator func = m_deleteFuncs.find( it->second->DefType() );

      if( func != m_deleteFuncs.end() )
      {
        lua_State* ls = Globals::scriptEngine->MasterState();
        it->second->PushToLua( ls );
        (*func)( ls, 1 );
      }

      delete it->second;
      m_entities.erase( it );
    }
  }

  void EntityFactory::Notify( Entity* entity, const NotifierMap& map )
  {
    if( !entity )
      return;

    for( int i = 0; i < ComponentType::LAST; ++i )
    {
      ComponentType::ID ctype = static_cast< ComponentType::ID >( i );

      if( entity->Has( ctype ) )
      {
        std::pair< NotifierMap::const_iterator, NotifierMap::const_iterator > itPair;
        itPair = map.equal_range( ctype );

        while( itPair.first != itPair.second )
        {
          itPair.first->second( entity->ID() );
          ++(itPair.first);
        }
      }
    }
  }

  void EntityFactory::NewEntityDef( const std::string& mappedName, ticpp::Element* elem )
  {
    if( m_entityDefs.find( mappedName ) != m_entityDefs.end() )
    {
      Globals::console->Error() << "Duplicate entity definition: "
                                << mappedName << ", line " << elem->Row() << std::endl;
      return;
    }

    Entity* entity = new Entity();

    try
    {
      // iterate over all "entity" items
      ticpp::Iterator< ticpp::Element > component( elem->FirstChildElement( "component", "component" ) );

      while( component != component.end() )
      {
        std::string type;
        component->GetAttribute( "type", &type, false );

        if( type.length() != 0 )
        {
          CreatorTable::iterator it = m_creators.find( type );

          if( it != m_creators.end() )
          {
            BaseComponent* bc = (*it)( &( *component ) );

            if( bc )
            {
              bc->name = component->GetAttribute( std::string( "name" ) );
              entity->AddComponent( bc );
            }
            else
            {
              Globals::console->Error() << type << " creator failed, line "
                                        << component->Row() << std::endl;
            }
          }
          else
          {
            Globals::console->Error() << type << " creator not registered, line "
                                      << component->Row() << std::endl;
          }
        }
        else
        {
          // Entity def does not have a name, useless
          Globals::console->Error() << "Invalid tag on line "
                                    << component->Row() << std::endl;
        }

        ++component;
      }
    }
    catch( const ticpp::Exception& e )
    {
      // Something quite wrong (no component tags, out of memory, etc)
      delete entity;
      Globals::console->Error() << "Exception from LoadDefinitions: "
                                << e.what() << std::endl;
      return;
    }

    if( entity->InitComponents() == false )
    {
      Globals::console->Error() << "InitComponents failed: " << mappedName << std::endl;
      delete entity;
    }
    else
    {
      Entity test( *entity );
      test.InitComponents();

      Networking::ByteStream bs;
      test.InitOut( bs, true );
      test.InitIn( bs, true );

      test.InitOut( bs, false );
      test.InitIn( bs, false );

      test.DeltaOut( bs );
      test.DeltaIn( bs );

      if( bs.Size() != 0 || bs.HadError () )
      {
        Globals::console->Error() << "Entity def did not stream correctly: "
                                  << mappedName << std::endl;
        delete entity;
      }
      else
      {
        entity->m_defType = mappedName;
        m_entityDefs.insert( mappedName, entity );
      }
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // Lua methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( EntityFactory )
    LUA_NAME_METHOD( EntityFactory, "GetEntity", LGetEntity )
    LUA_NAME_METHOD( EntityFactory, "AddOnCreate", LAddOnCreate )
    LUA_NAME_METHOD( EntityFactory, "AddOnDelete", LAddOnDelete )
    LUA_NAME_METHOD( EntityFactory, "CreateEntity", LCreateEntity )
    LUA_NAME_METHOD( EntityFactory, "DeleteEntity", LDeleteEntity )

    // This is left in for backwards compatibility
    LUA_NAME_METHOD( EntityFactory, "Create", LCreateEntity )
  LUA_END_DEFINE_CLASS


  LUA_DEFINE_METHOD( EntityFactory, LGetEntity )
  {
    int id = luaL_checkint( ls, -1 );
    Entity* e = GetEntity( id );
    Lunar<Engine::Entity>::push( ls, e );
    return 1;
  }

  LUA_DEFINE_METHOD( EntityFactory, LAddOnCreate )
  {
    const char* defType = luaL_checkstring( ls, 1 );
    luaL_checktype( ls, 2, LUA_TFUNCTION );

    LuaFuncTable::iterator it = m_createFuncs.find( defType );

    if(it != m_createFuncs.end() )
    {
      it->Set( ls );
      Globals::console->Warning() << "Overriding OnCreate for " << defType << std::endl;
    }
    else
    {
      Script::LuaFunction func;
      func.Set( ls );

      m_createFuncs.insert( defType, func );
    }

    return 0;
  }

  LUA_DEFINE_METHOD( EntityFactory, LAddOnDelete )
  {
    const char* defType = luaL_checkstring( ls, 1 );
    luaL_checktype( ls, 2, LUA_TFUNCTION );

    LuaFuncTable::iterator it = m_deleteFuncs.find( defType );

    if(it != m_deleteFuncs.end() )
    {
      it->Set( ls );
      Globals::console->Warning() << "Overriding OnDelete for " << defType << std::endl;
    }
    else
    {
      Script::LuaFunction func;
      func.Set( ls );

      m_deleteFuncs.insert( defType, func );
    }

    return 0;
  }

  LUA_DEFINE_METHOD( EntityFactory, LCreateEntity )
  {
    const char* mappedName = luaL_checkstring( ls, 1 );
    Entity* entity = Create( mappedName );

    if( entity )
    {
      int top = lua_gettop( ls );

      for( int i = 2; i <= top; ++i )
      {
        Script::LuaValue lv;
        lv.FromLua( ls, i );

        if( lv.Vald() )
        {
          lv.ToLua( ls );
          entity->m_onCreateParams.push_back( lv );
        }
      }

      CallOnCreate( ls, entity );
    }

    Lunar<Engine::Entity>::push( ls, entity );
    return 1;
  }

  LUA_DEFINE_METHOD( EntityFactory, LDeleteEntity )
  {
    int id = luaL_checkint( ls, -1 );
    DeleteEntity( id );
    return 0;
  }

} // Engine namespace
} // SF namespace
