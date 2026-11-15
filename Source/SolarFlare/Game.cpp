/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Game.cpp
**  \author  Scorched Muffin Entertainment
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 01:35:05 - nwilliam
**  \brief   Implementation of the 'Game'
*******************************************************************************/

#include <ctime>

#include "Game.h"
#include "Globals.h"
#include "resource.h"
#include "Sound/SoundEngine.h"
#include "Engine/InputEngine.h"
#include "Script/ScriptEngine.h"
#include "Engine/AssetManager.h"
#include "Engine/EntityFactory.h"
#include "Interface/MenuSystem.h"
#include "Engine/MessageEngine.h"
#include "Physics/PhysicsEngine.h"
#include "Utilities/Random/SNCG.h"
#include "Graphics/GraphicsEngine.h"
#include "Graphics/ParticleEngine.h"
#include "Networking/NetworkEngine.h"

namespace SF
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  Game::Game() : m_running( true ),
                 m_window( "Solar Flare" ),
                 m_engineTimerCount( 0 )
  {
    Globals::graphicsEngine->SetupWindow( m_window.GetOSSpecific( 0 ),
                                          m_window.GetOSSpecific( 1 ),
                                          m_window.Width(),
                                          m_window.Height(),
                                          Color(0,0,0,255) );
  }

  Game::~Game()
  {
  }

  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void Game::Run()
  {
    Globals::dRandom->Seed( 0 );
    Globals::rRandom->Seed( static_cast<int>( std::time( 0 ) ) );

    Globals::scriptEngine->RegisterGlobal( this, "Game" );
    Globals::scriptEngine->RegisterGlobal( Globals::input, "Input" );
    Globals::scriptEngine->RegisterGlobal( Globals::console, "Console" );
    Globals::scriptEngine->RegisterGlobal( Globals::networkEngine, "Network" );
    Globals::scriptEngine->RegisterGlobal( Globals::menuSystem, "MenuSystem" );
    Globals::scriptEngine->RegisterGlobal( Globals::soundEngine, "SoundEngine" );
    Globals::scriptEngine->RegisterGlobal( Globals::scriptEngine, "ScriptEngine" );
    Globals::scriptEngine->RegisterGlobal( Globals::entityFactory, "EntityFactory" );
    Globals::scriptEngine->RegisterGlobal( Globals::physicsEngine, "PhysicsEngine" );
    Globals::scriptEngine->RegisterGlobal( Globals::graphicsEngine, "GraphicsEngine" );

    InitAssetLoaders();
    InitComponentCreators();

    #ifdef _WIN32
      HWND hwnd = *(HWND*)m_window.GetOSSpecific(0);
      HICON icon = (HICON)LoadImage( GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 32, 32, 0);
      SendMessage( hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon );
      SendMessage( hwnd, WM_SETICON, ICON_BIG, (LPARAM)icon );
    #endif

    if( Globals::scriptEngine->DoFile( "config", "config.lua" ) )
    {
      throw( Globals::scriptEngine->LastHandledError() );
    }

    Globals::graphicsEngine->Init();
    Globals::particleEngine->Initialize();

    Utilities::Timer engineTimer;

    while( m_running )
    {
      Globals::timer->Update();
      float dt = Globals::timer->Elapsed();

      m_engineTimer.Update();
      ++m_engineTimerCount;

      Globals::physicsEngine->Update( dt );         AddEngineTime("Physics");
      Globals::graphicsEngine->UpdateCamera( dt );  
      Globals::taskManager->Update();               AddEngineTime("Task");
      Globals::messageEngine->Update( dt );         AddEngineTime("Message");
      Globals::scriptEngine->Update( dt );          AddEngineTime("Script");
      Globals::entityFactory->Update();             AddEngineTime("Entity");
      Globals::menuSystem->Update( dt );            AddEngineTime("Menus");
      Globals::particleEngine->Update( dt );        AddEngineTime("Particles");
      Globals::graphicsEngine->Update( dt );        AddEngineTime("Graphics");
      Globals::networkEngine->Update( dt );         AddEngineTime("Network");
      Globals::soundEngine->Update( dt );           AddEngineTime("Sound");
      Globals::input->Update();                     AddEngineTime("Input");

      m_window.Sleep( 0 );

      m_running &= m_window.Update();
    }

    Globals::messageEngine->AddMessage( "GameShutdown" );
    Globals::messageEngine->Update(16.667f);
  }

  void Game::AddEngineTime( const std::string& engineName )
  {
    m_engineTimer.Update();
    m_engineTimes[ engineName ] += m_engineTimer.Elapsed();
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void Game::InitAssetLoaders()
  {
    using namespace Globals;
    using namespace Engine;

    assetManager->AddFileType( "lua", "Scripts/", Assets::T_SCRIPT, Assets::ScriptAsset() );

    assetManager->AddFileType( "bmp", "Textures/", Assets::T_TEXTURE, Assets::Texture() );
    assetManager->AddFileType( "png", "Textures/", Assets::T_TEXTURE, Assets::Texture() );
    assetManager->AddFileType( "jpg", "Textures/", Assets::T_TEXTURE, Assets::Texture() );
    assetManager->AddFileType( "tga", "Textures/", Assets::T_TEXTURE, Assets::Texture() );
    assetManager->AddFileType( "dds", "Textures/", Assets::T_TEXTURE, Assets::Texture() );
    assetManager->AddFileType( "psd", "Textures/", Assets::T_TEXTURE, Assets::Texture() );
    assetManager->AddFileType( "jpeg", "Textures/", Assets::T_TEXTURE, Assets::Texture() );

    assetManager->AddFileType( "tai", "Textures/", Assets::T_TEXTURE_ATLAS, Assets::TextureAtlas() );

    assetManager->AddFileType( "3ds", "Models/", Assets::T_MODEL, Assets::ModelAsset() );
    assetManager->AddFileType( "obj", "Models/", Assets::T_MODEL, Assets::ModelAsset() );
    assetManager->AddFileType( "lwo", "Models/", Assets::T_MODEL, Assets::ModelAsset() );

    assetManager->AddFileType( "frag", "Shaders/", Assets::T_SHADER, Assets::FragmentShader() );
    assetManager->AddFileType( "vert", "Shaders/", Assets::T_SHADER, Assets::VertexShader() );

    assetManager->AddFileType( "ogg", "Audio/", Assets::T_SOUND, Assets::Sound() );
  }

  void Game::InitComponentCreators()
  {
    using namespace Engine;
    using namespace Globals;

    // Register Creators here
    entityFactory->Creator( "TRANSFORM", CCreator::Function< Math::CTransform::Creator >() );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Lua methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( Game )
    LUA_NAME_METHOD( Game, "Quit", LQuit )
    LUA_NAME_METHOD( Game, "EngineTimes", LEngineTimes )
    LUA_NAME_METHOD( Game, "ReloadAsset", LReloadAsset )
    LUA_NAME_METHOD( Game, "GameElapsed", LGameElapsed )
    LUA_NAME_METHOD( Game, "FrameElapsed", LFrameElapsed )
    LUA_NAME_METHOD( Game, "FullscreenOn", LFullscreenOn )
    LUA_NAME_METHOD( Game, "MouseCentered", LMouseCentered )
  LUA_END_DEFINE_CLASS

  
  LUA_DEFINE_METHOD_NOLS( Game, LQuit )
  {
    m_running = false;
    return 0;
  }

  LUA_DEFINE_METHOD( Game, LEngineTimes )
  {
    int index = 1;

    lua_createtable( ls, static_cast<int>( m_engineTimes.size() ), 0 );

    for(std::map<std::string, double>::iterator iter = m_engineTimes.begin(); iter != m_engineTimes.end(); ++iter)
    {
      lua_pushinteger(ls, index++ );

      lua_createtable(ls, 0, 2);

        lua_pushstring(ls, "Engine");
        lua_pushstring(ls, iter->first.c_str());
        lua_settable( ls, -3 );

        lua_pushstring(ls, "Time");
        lua_pushnumber(ls, iter->second / m_engineTimerCount);
        lua_settable( ls, -3 );

      lua_settable( ls, -3 );

      iter->second = 0;
    }

    m_engineTimerCount = 0;

    return 1;
  }

  LUA_DEFINE_METHOD( Game, LReloadAsset )
  {
    const char * str = luaL_checkstring( ls, 1 );
    Globals::assetManager->ReloadFile(std::string(str));
    return 0;
  }

  LUA_DEFINE_METHOD( Game, LGameElapsed )
  {
    lua_pushnumber( ls, Globals::timer->ElapsedSinceStart() );
    return 1;
  }

  LUA_DEFINE_METHOD( Game, LFrameElapsed )
  {
    lua_pushnumber( ls, Globals::timer->Elapsed() );
    return 1;
  }

  LUA_DEFINE_METHOD( Game, LFullscreenOn )
  {
    int b = lua_toboolean( ls, 1 );
    m_window.FullscreenOn( b != 0 );
    return 0;
  }

  LUA_DEFINE_METHOD( Game, LMouseCentered )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushboolean( ls, m_window.IsMouseCentered() );
      return 1;
    }
    else
    {
      m_window.MouseCentered( lua_toboolean( ls, 1 ) == 1 );
      return 0;
    }
  }

} // SF namespace
