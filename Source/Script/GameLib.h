/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    GameLibrary.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/06/2009 @ 16:17:43 - nwilliam
**  \brief   Helper/general functions for Lua that aren't bound through a class.
*******************************************************************************/

#ifndef SF_GAME_LIBRARY_H
#define SF_GAME_LIBRARY_H

#include "External/Lua/include/lua.hpp"

namespace SF {
namespace Script {
namespace GameLib
{
  // Function->Name array and register fuctiong
  extern const luaL_reg gameLib[];
  int OpenGameLib( lua_State* ls );
  
  
  // Messages
  int AddMessage( lua_State* ls );
  int AddMessageSubscription( lua_State* ls );
  
  // Pausing
  int PausePhysicsToggle( lua_State * );
  int PausePhysics( lua_State *ls );
  int UnpausePhysics( lua_State *ls );
  int PhysicsPaused( lua_State *ls );
  
  int EnableParticles( lua_State* ls );
  int DisableParticles( lua_State* ls );
  int ClearParticles( lua_State* ls );
  
  // Other..
  int DRand( lua_State* ls );
  int RRand( lua_State* ls );
  int LoadAssets( lua_State* ls );
  int SetAssetDir( lua_State* ls );
  int GetUserPath( lua_State* ls );
  int CreateFolder( lua_State* ls );
  int LoadEntityDefs( lua_State* ls );
  int GetClipboardText( lua_State* ls );
  
  
  
} // GameLib namespace
} // Script namespace
} // SF namespace

#endif // SF_GAME_LIBRARY_H
