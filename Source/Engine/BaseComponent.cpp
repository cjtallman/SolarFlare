/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseComponent.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/14/2008 @ 17:02:32 - nwilliam
**  \brief   BaseComponent implementation.
*******************************************************************************/

#include "BaseComponent.h"
#include "External/Lua/include/lua.hpp"

namespace SF {
namespace Engine
{
  const char* ComponentType::name[ComponentType::LAST+1] = { "sun",
                                                             "planet",
                                                             "skybox",
                                                             "model",
                                                             "ellipse",
                                                             "text",
                                                             "world_quad",
                                                             "ui_graphic",
                                                             "ui_element",
                                                             "physics",
                                                             "script",
                                                             "sound",
                                                             "transform",
                                                             "emitter",
                                                             "network",
                                                             0 };
  
  //////////////////////////////////////////////////////////////////////////////
  // Construtors / Destructor
  //////////////////////////////////////////////////////////////////////////////
  BaseComponent::BaseComponent( ComponentType::ID _type ) : name(),
                                                            type( _type )
  {
  }
  
  BaseComponent::BaseComponent( const std::string& _name, ComponentType::ID _type ) : name( _name ),
                                                                                      type( _type )
  {
  }
  
  BaseComponent::~BaseComponent()
  {
  }

  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void BaseComponent::PushToLua( lua_State* ls )
  {
    lua_pushnil( ls );
  }
  
  void BaseComponent::InitIn( Networking::ByteStream& )
  {
  }
  
  void BaseComponent::DeltaIn( Networking::ByteStream& )
  {
  }
  
  void BaseComponent::InitOut( Networking::ByteStream& ) const
  {
  }
  
  void BaseComponent::DeltaOut( Networking::ByteStream& ) const
  {
  }
  
} // Engine namespace
} // SF namespace 
