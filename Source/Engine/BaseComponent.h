/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseComponent.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/14/2008 @ 17:02:28 - nwilliam
**  \brief   Base class for all game components.
*******************************************************************************/

#ifndef SF_BASE_COMPONENT_H
#define SF_BASE_COMPONENT_H

#include <list>
#include <string>
#include "Networking/ByteStream.h"

// Forward declaration
struct lua_State;

namespace SF {
namespace Engine
{
  namespace ComponentType
  {
    enum ID { SUN, PLANET, SKYBOX, MODEL, ELLIPSE, TEXT, WORLD_QUAD, UI_GRAPHIC, UI_ELEMENT, PHYSICS, SCRIPT, SOUND, 
              TRANSFORM, EMITTER, NETWORK, LAST };
              
    extern const char* name[LAST+1];
  }
  
  
  // Forward declaration
  class Entity;
  
  
  class BaseComponent
  {
    public:
      BaseComponent( ComponentType::ID _type );
      BaseComponent( const std::string& _name, ComponentType::ID _type );
      virtual ~BaseComponent();
      
      virtual BaseComponent* Clone() const = 0;
      virtual bool InitDependencies( Entity* ) = 0;
      virtual void PushToLua( lua_State* ls );
      
      // Networking
      virtual void InitIn( Networking::ByteStream& bs );
      virtual void DeltaIn( Networking::ByteStream& bs );
      virtual void InitOut( Networking::ByteStream& bs ) const;
      virtual void DeltaOut( Networking::ByteStream& bs ) const;
      
      std::string name;
      ComponentType::ID type;
  };
  
  
  typedef std::list< BaseComponent* > ComponentList;
  
  
} // Engine namespace
} // SF namespace 

#endif // SF_BASE_COMPONENT_H
