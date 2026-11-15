/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    EntityFactory.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/20/2009 @ 19:24:24 - nwilliam
**  \brief   Assembly line, 1920's style factory (for entities).
*******************************************************************************/

#ifndef SF_ENTITY_FACTORY_H
#define SF_ENTITY_FACTORY_H

#include <map>
#include <queue>
#include <string>
#include "Entity.h"
#include "BaseComponent.h"
#include "Script/ScriptHelper.h"
#include "Utilities/Delegates.h"
#include "Utilities/HashTable.h"

// Forward declaration
namespace ticpp { class Element; }

namespace SF {
namespace Engine
{
  typedef Utilities::Delegate1< void, EntityID > Notifier;
  typedef Utilities::Delegate1< BaseComponent*, const ticpp::Element* > CCreator;

  class EntityFactory
  {
    public:
      EntityFactory();
      ~EntityFactory();

      void Update();

      Entity* GetEntity( int id );
      void DeleteEntity( EntityID id );
      Entity* Create( const std::string& defName );
      Entity* CreateID( const std::string& defName, EntityID id, bool local );

      void Reset( EntityID lower );
      void LoadDefinitions( const std::string& defFile );
      
      void Creator( const std::string& cName, const CCreator& del );
      void CreateNotifier( ComponentType::ID type, const Notifier& notifier );
      void DeleteNotifier( ComponentType::ID type, const Notifier& notifier );

      void CallOnCreate( lua_State* ls, Entity* entity );

    private:
      typedef std::queue< EntityID > DeleteQueue;
      typedef std::map< EntityID, Entity* > EntityMap;
      typedef Utilities::HashTable< Entity* > EntityTable;
      typedef Utilities::HashTable< CCreator > CreatorTable;
      typedef Utilities::HashTable< Script::LuaFunction > LuaFuncTable;
      typedef std::multimap< ComponentType::ID, Notifier > NotifierMap;

      void InternalDeleteEntity( EntityID id );
      void Notify( Entity* entity, const NotifierMap& map );
      void NewEntityDef( const std::string& mappedName, ticpp::Element* elem );


      EntityID m_curLocalID;
      EntityID m_curNetworkID;

      EntityMap m_entities;
      EntityTable m_entityDefs;

      CreatorTable m_creators;
      DeleteQueue m_deleteQueue;
      NotifierMap m_createNotifiers;
      NotifierMap m_deleteNotifiers;

      LuaFuncTable m_createFuncs;
      LuaFuncTable m_deleteFuncs;

      LUA_DECLARE_CLASS( EntityFactory )
      LUA_DECLARE_METHOD( LGetEntity )
      LUA_DECLARE_METHOD( LAddOnCreate )
      LUA_DECLARE_METHOD( LAddOnDelete )
      LUA_DECLARE_METHOD( LCreateEntity )
      LUA_DECLARE_METHOD( LDeleteEntity )
  };
  
} // Engine namespace
} // SF namespace

#endif // SF_ENTITY_FACTORY_H
