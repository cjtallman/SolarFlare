/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    GameLibrary.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/06/2009 @ 16:17:24 - nwilliam
**  \brief   Helper/general functions for Lua that aren't bound through a class.
*******************************************************************************/

#include <cmath>  // modf
#include <sstream>

#include "GameLib.h"
#include "ScriptEngine.h"
#include "Sound/SoundEngine.h"
#include "SolarFlare/Globals.h"
#include "Engine/InputEngine.h"
#include "Utilities/OSHelper.h"
#include "Engine/AssetManager.h"
#include "Engine/EntityFactory.h"
#include "Utilities/DevConsole.h"
#include "Engine/MessageEngine.h"
#include "Physics/PhysicsEngine.h"
#include "Graphics/ParticleEngine.h"
#include "Utilities/Random/SNCG.h"

#define FUNC(x)  { #x, x }

namespace SF {
namespace Script {
namespace GameLib
{
  // Map the functions to their script equivalents
  const luaL_reg gameLib[] = { FUNC( AddMessage ),
                               FUNC( AddMessageSubscription ),
                               
                               FUNC( PausePhysicsToggle ),
                               FUNC( PausePhysics ),
                               FUNC( UnpausePhysics ),
                               FUNC( PhysicsPaused ),
                                
                               FUNC( EnableParticles ),
                               FUNC( DisableParticles ),
                               FUNC( ClearParticles ),
                               
                               FUNC( DRand ),
                               FUNC( RRand ),
                               FUNC( LoadAssets ),
                               FUNC( SetAssetDir ),
                               FUNC( GetUserPath ),
                               FUNC( CreateFolder ),
                               FUNC( LoadEntityDefs ),
                               FUNC( GetClipboardText ),
                               
                               { 0 , 0 }
                              };
  #undef FUNC
  
  
  int OpenGameLib( lua_State* ls )
  {
    luaL_register( ls , "GameLib" , gameLib );
    lua_pop(ls, 1);
    return 0;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Messages
  //////////////////////////////////////////////////////////////////////////////
  void AddMessage_Helper( lua_State* ls, int index, Engine::Message* msg )
  {
    switch( lua_type( ls, index ) )
    {
      case LUA_TBOOLEAN:
        msg->stack.Push( lua_toboolean( ls, index  ) != 0 );
      break;
      
      case LUA_TNUMBER:
      {
        double ip;
        double val = lua_tonumber( ls, index );
        double frac = std::modf( val, &ip );

        if( frac == 0.0 )
          msg->stack.Push<int>( static_cast<int>( val ) );
        else
          msg->stack.Push( static_cast<float>( val ) );
      }
      break;
      
      case LUA_TSTRING:
        msg->stack.Push( std::string( lua_tostring( ls, index ) ) );
      break;
      
      case LUA_TTABLE:
      {
        size_t size = lua_objlen( ls, index );

        if( size == 0 )
          luaL_error( ls, "Zero length table passed to AddMessage (best argument guess: #%d)", index );

        for( size_t i = 1; i <= size; ++i )
        {
          lua_pushinteger( ls, i );
          lua_gettable( ls, -2 );
          AddMessage_Helper( ls, -1, msg );
          lua_pop( ls, 1 );
        }
      }
      break;
      
      default:
        luaL_error( ls, "Argument #%d to AddMessage was non-primitive (%s)", index, lua_typename( ls, lua_type( ls, index ) ) );
      break;
    }
  }
  
  int AddMessage( lua_State* ls )
  {
    const char* target = luaL_checkstring( ls, 1 );
    const char* msgType = luaL_checkstring( ls, 2 );
    float delay = luaL_checkfloat( ls, 3 );
    int route = luaL_checkint( ls, 4 );
    
    int index = 5;
    Engine::Message* msg = 0;

    if( std::strcmp( target, "global" ) == 0 )
    {
      msg = Globals::messageEngine->AddMessage( msgType, delay );
    }
    else if( std::strcmp( target, "entity" ) == 0 )
    {
      int id = luaL_checkint( ls, index++ );
      msg = Globals::messageEngine->AddMessage( msgType, id, delay );
    }
    else
    {
      luaL_error( ls, "Invalid destination (must be global or entity)" );
    }

    // Message::ROUTE
    if( route < 0 || route > 3 )
    {
      luaL_error( ls, "Invalid route" );
    }

    msg->route = static_cast<Engine::Message::ROUTE>( route );

    int size = lua_gettop( ls );
    
    while( index <= size )
    {
      AddMessage_Helper( ls, index, msg );
      ++index;
    }
    
    return 0;
  }
  
  int AddMessageSubscription( lua_State* ls )
  {
    std::string str = luaL_checkstring( ls, 1 );
    int eid = luaL_checkint( ls, 2 );
    
    Globals::messageEngine->AddSubscription( str, eid );
    
    return 0;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Pausing
  //////////////////////////////////////////////////////////////////////////////
  int PausePhysicsToggle( lua_State * )
  {
    //Globals::soundEngine->SoundFXPaused( !Globals::soundEngine->SoundFXPaused() );

    Globals::physicsEngine->TogglePause();
    Globals::particleEngine->TogglePause();
    return 0;
  }
  
  int PausePhysics( lua_State * )
  {
    //Globals::soundEngine->SoundFXPaused( true );
    
    Globals::physicsEngine->Pause();
    Globals::particleEngine->Pause();
    return 0;
  }

  int UnpausePhysics( lua_State * )
  {
    //Globals::soundEngine->SoundFXPaused( false );
    
    Globals::physicsEngine->Unpause();
    Globals::particleEngine->Unpause();
    return 0;
  }
  
  int EnableParticles( lua_State * )
  {
    Globals::particleEngine->Enable();
    return 0;
  }

  int DisableParticles( lua_State * )
  {
    Globals::particleEngine->Disable();
    return 0;
  }
  
  int ClearParticles( lua_State * )
  {
    Globals::particleEngine->Clear();
    return 0;
  }

  int PhysicsPaused( lua_State *ls )
  {
    lua_pushboolean( ls, Globals::physicsEngine->IsPaused() );
    return 1;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Other...
  //////////////////////////////////////////////////////////////////////////////
  int DRand( lua_State* ls )
  {
    // Behaves exactly as default math.random
    double num = 0;
    
    switch( lua_gettop( ls ) )
    {
      case 0:   num = Globals::dRandom->Rand();                                                     break;
      case 1:   num = Globals::dRandom->RandInt( luaL_checkint( ls, 1 ) );                          break;
      default:  num = Globals::dRandom->RandInt( luaL_checkint( ls, 1 ), luaL_checkint( ls, 2 ) );  break;
    }
    
    lua_pushnumber( ls, num );
    return 1;
  }
  
  int RRand( lua_State* ls )
  {
    // Behaves exactly as default math.random
    double num = 0;
    
    switch( lua_gettop( ls ) )
    {
      case 0:   num = Globals::rRandom->Rand();                                                     break;
      case 1:   num = Globals::rRandom->RandInt( luaL_checkint( ls, 1 ) );                          break;
      default:  num = Globals::rRandom->RandInt( luaL_checkint( ls, 1 ), luaL_checkint( ls, 2 ) );  break;
    }
    
    lua_pushnumber( ls, num );
    return 1;
  }
  
  int LoadAssets( lua_State* ls )
  {
    const char* file = luaL_checkstring( ls, 1 );
    Globals::assetManager->LoadAssets( file );
    return 0;
  }
  
  int SetAssetDir( lua_State* ls )
  {
    const char* file = luaL_checkstring( ls, 1 );
    Globals::assetManager->SetDirectory( file );
    return 0;
  }
  
  int GetUserPath( lua_State* ls )
  {
    std::string path = Utilities::GetUserAppDataPath();
    lua_pushlstring( ls, path.c_str(), path.size() );
    return 1;
  }
  
  int CreateFolder( lua_State* ls )
  {
    const char* str = luaL_checkstring( ls, 1 );
    bool result = Utilities::CreateFolder( str );
    lua_pushboolean( ls, result );
    return 1;
  }
  
  int LoadEntityDefs( lua_State* ls )
  {
    const char* file = luaL_checkstring( ls, 1 );
    Globals::entityFactory->LoadDefinitions( file );
    return 0;
  }





  int GetClipboardText( lua_State* ls )
  {
    std::string text = Utilities::GetClipboardText();
    lua_pushstring( ls, text.c_str() );
    return 1;
  }
  
} // GameLib namespace
} // Script namespace
} // SF namespace
