/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SoundEngine.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/04/2009 @ 18:27:38 - nwilliam
**  \brief   FMOD Ex based sound engine implementation.
*******************************************************************************/

#include <cstring> // strstr

#include "SoundEngine.h"
#include "SolarFlare/Globals.h"    // entityFactory, assetManager, console, graphicsEngine (camera)
#include "Engine/AssetManager.h"
#include "Engine/EntityFactory.h"
#include "Utilities/DevConsole.h"
#include "Graphics/GraphicsEngine.h"
#include "External/ticpp/include/ticpp.h"
#include "External/FMOD/include/fmod_errors.h"

namespace
{
  void LogError( FMOD_RESULT result )
  {
    if( result != FMOD_OK )
    {
      Globals::console->Error() << "FMOD Error: " << FMOD_ErrorString( result ) << std::endl;
    }
  }
}

namespace SF {
namespace Sound
{ 
  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  SoundEngine::SoundEngine() : m_system( 0 )
  {
    InitFMOD();

    MasterVolume( 0.5f );
    MusicVolume( 0.25f );
    SoundFXVolume( 1.0f );

    Globals::entityFactory->Creator( "SOUND", Engine::CCreator::Method< SoundEngine, &SoundEngine::CSoundCreator >( this ) );
    Globals::entityFactory->CreateNotifier( Engine::ComponentType::SOUND, Engine::Notifier::Method< SoundEngine, &SoundEngine::CSoundCreateTrack >( this ) );
    Globals::entityFactory->DeleteNotifier( Engine::ComponentType::SOUND, Engine::Notifier::Method< SoundEngine, &SoundEngine::CSoundDeleteTrack >( this ) );
  }

  SoundEngine::~SoundEngine()
  {
    if( m_system )
    {
      for( SoundTable::iterator it = m_sounds.begin(); it != m_sounds.end(); ++it )
      {
        (*it)->release();
      }

      m_musicGroup->release();
      m_soundFXGroup->release();

      m_system->release();
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void SoundEngine::Update( float /*elapsed*/ )
  {
    if( !m_system )
    {
      return;
    }

    Math::Vector3 p = Globals::graphicsEngine->CameraPosition();
    Math::Vector3 v = Globals::graphicsEngine->CameraView();

    FMOD_VECTOR pos = { p.x, p.y, p.z };
    FMOD_VECTOR view = { v.x, v.y, v.z };
    FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };

    m_system->set3DListenerAttributes( 0, &pos, 0, &view, &up );

    for( Engine::EntityIDList::iterator it = m_soundEntities.begin(); it != m_soundEntities.end(); ++it )
    {
      Engine::Entity* e = Globals::entityFactory->GetEntity( *it );

      if( e )
      {
        Engine::ComponentList& cl = e->GetComponentsByType( Engine::ComponentType::SOUND );

        for( Engine::ComponentList::iterator cit = cl.begin(); cit != cl.end(); ++cit )
        {
          CSound* cs = static_cast<CSound*>( *cit );
          cs->Update();

          if( cs->m_playMe )
          {
            cs->m_playMe = false;
            cs->UnPause();
          }
        }
      }
    }

    m_system->update();
  }

  float SoundEngine::MusicVolume() const
  {
    return m_musicVolume;
  }

  float SoundEngine::MasterVolume() const
  {
    return m_masterVolume;
  }

  float SoundEngine::SoundFXVolume() const
  {
    return m_soundFXVolume;
  }

  void SoundEngine::MusicVolume( float val )
  {
    m_musicVolume = val;
    m_musicGroup->setVolume( m_masterVolume * val );
  }

  void SoundEngine::MasterVolume( float val )
  {
    m_masterVolume = val;
    MusicVolume( m_musicVolume );
    SoundFXVolume( m_soundFXVolume );
  }

  void SoundEngine::SoundFXVolume( float val )
  {
    m_soundFXVolume = val;
    m_soundFXGroup->setVolume( m_masterVolume * val );
  }

  bool SoundEngine::MusicMuted() const
  {
    bool muted = false;
    m_musicGroup->getMute( &muted );
    return muted;
  }

  bool SoundEngine::MusicPaused() const
  {
    bool paused = false;
    m_musicGroup->getPaused( &paused );
    return paused;
  }

  void SoundEngine::MusicMuted( bool muted )
  {
    m_musicGroup->setMute( muted );
  }

  void SoundEngine::MusicPaused( bool paused )
  {
    m_musicGroup->setPaused( paused );
  }

  bool SoundEngine::SoundFXMuted() const
  {
    bool muted = false;
    m_soundFXGroup->getMute( &muted );
    return muted;
  }

  bool SoundEngine::SoundFXPaused() const
  {
    bool paused = false;
    m_soundFXGroup->getPaused( &paused );
    return paused;
  }

  void SoundEngine::SoundFXMuted( bool muted )
  {
    m_soundFXGroup->setVolume( muted ? 0 : m_soundFXVolume );
    m_soundFXGroup->setMute( muted );
  }

  void SoundEngine::SoundFXPaused( bool paused )
  {
    m_soundFXGroup->setPaused( paused );
  }

  void SoundEngine::CSoundCreateTrack( Engine::EntityID id )
  {
    Engine::Entity* entity = Globals::entityFactory->GetEntity( id );

    if( entity )
    {
      m_soundEntities.push_back( id );
    }
  }

  void SoundEngine::CSoundDeleteTrack( Engine::EntityID id )
  {
    m_soundEntities.remove( id );
  }

  Engine::BaseComponent* SoundEngine::CSoundCreator( const ticpp::Element* element )
  {
    using namespace Engine;

    if( !element )
    {
      return 0;
    }

    Engine::BaseComponent* bc = 0;

    ticpp::Iterator< ticpp::Element > sound( element->FirstChildElement( "sound", false ) );

    if( sound != sound.end() )
    {
      while( sound != sound.end() )
      {
        std::string asset = sound->GetAttribute( "asset" );

        if( asset.length() != 0 )
        {
          Assets::BaseAsset* ba = Globals::assetManager->GetAsset( asset );

          if( ba )
          {
            SoundTable::iterator it = m_sounds.find( asset );

            bool stream = asset.find( "_stream" ) != std::string::npos;

            std::string type = sound->GetAttribute( "type" );
            std::string positional = sound->GetAttribute( "positional" );

            CSound* cs = new CSound();

            sound->GetAttribute( "loops", &cs->m_loopCount, false );
            cs->m_positional = positional.compare( "true" ) ? false : true;
            cs->m_group = type.compare( "music" ) ? m_soundFXGroup :m_musicGroup;

            if( it == m_sounds.end() )
            {
              FMOD::Sound* sound = 0;

              if( stream )
                sound = CreateStream( ba->filePath );
              else
                sound = CreateSound( ba->filePath, cs->m_positional );

              it = m_sounds.insert( asset, sound );
            }

            cs->m_sound = *it;

            bc = cs;
          }
          else
          {
            Globals::console->Error() << "Unknown sound asset, line: " 
                                      << element->Row() << std::endl;
          }
        }
        else
        {
          Globals::console->Error() << "Sound asset needed, line: "
                                    << element->Row() << std::endl;
        }

        ++sound;
      }
    }

    return bc;
  }

  void SoundEngine::PlayCSound( CSound& sound )
  {
    if( m_system )
    {
      m_system->playSound( FMOD_CHANNEL_REUSE, sound.m_sound, true, &sound.m_channel );
      sound.m_channel->setChannelGroup( sound.m_group );
      sound.m_channel->setLoopCount( sound.m_loopCount );
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void SoundEngine::InitFMOD()
  {
    // This is the FMOD recommended sequence that comes from the docs
    // ie, the one with the big NO GAME SHOULD NOT USE THIS warning...

    FMOD_RESULT result = FMOD_OK;

    // Create a System object and initialize.
    result = FMOD::System_Create( &m_system );
    LogError( result );

    if( !m_system )
    {
      return;
    }

    unsigned int version = 0;
    result = m_system->getVersion(&version);
    LogError( result );

    if( version < FMOD_VERSION )
    {
      Globals::console->Error() << "An outdated FMOD dll is being used: " << version
                                << ", required is" << FMOD_VERSION << std::endl;

      m_system->release();
      m_system = 0;
      return;
    }

    FMOD_CAPS caps;
    FMOD_SPEAKERMODE speakermode;
    result = m_system->getDriverCaps( 0, &caps, 0, 0, &speakermode );
    LogError( result );

    // Set the user selected speaker mode
    result = m_system->setSpeakerMode( speakermode );
    LogError( result );

    if( caps & FMOD_CAPS_HARDWARE_EMULATED )
    {
      // The user has the 'Acceleration' slider set to off!  
      // This is really bad for latency, might want to warn user about this
      Globals::console->Warning() << "FMOD fell back to software. "
                                  << "HW acceleration disabled/not available."
                                  << std::endl;

      // At 48khz, the latency between issuing an fmod command and hearing 
      // it will now be about 213ms.
      result = m_system->setDSPBufferSize( 1024, 10 );
      LogError( result );
    }

    char name[256];
    result = m_system->getDriverInfo( 0, name, 256, 0 );
    LogError( result );

    if( std::strstr( name, "SigmaTel" ) )
    {
      // Sigmatel sound devices crackle for some reason if the format is 
      // pcm 16bit. pcm floating point output seems to solve it.
      result = m_system->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR );
      LogError( result );
    }

    // Replace with whatever channel count and flags you use!
    result = m_system->init( 100, FMOD_INIT_3D_RIGHTHANDED, 0 );

    m_system->set3DSettings( 100.0f, 3.0f, 1.0f );

    if( result == FMOD_ERR_OUTPUT_CREATEBUFFER )
    {
      // Speaker mode selected isn't supported, switch back to stereo
      result = m_system->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );
      LogError( result );

      // Replace with whatever channel count and flags you use!
      result = m_system->init( 100, FMOD_INIT_3D_RIGHTHANDED, 0 );
      LogError( result );
    }

    result = m_system->createChannelGroup( 0, &m_musicGroup );
    LogError( result );

    result = m_system->createChannelGroup( 0, &m_soundFXGroup );
    LogError( result );
  }

  FMOD::Sound* SoundEngine::CreateSound( const std::string& path, bool _3D )
  {
    FMOD::Sound* sound = 0;

    if( m_system )
    {
      int mode = _3D ? FMOD_3D : FMOD_2D;
      FMOD_RESULT result = m_system->createSound( path.c_str(), FMOD_NONBLOCKING | mode | FMOD_3D_LINEARROLLOFF, 0, &sound );
      LogError( result );

      sound->set3DMinMaxDistance( 0, 60000 );
    }

    return sound;
  }

  FMOD::Sound* SoundEngine::CreateStream( const std::string& path )
  {
    FMOD::Sound* sound = 0;

    if( m_system )
    {
      FMOD_RESULT result = m_system->createStream( path.c_str(), FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &sound );
      LogError( result );
    }

    return sound;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Lua Methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( SoundEngine )
    LUA_NAME_METHOD( SoundEngine, "MusicMuted",     LMusicMuted )
    LUA_NAME_METHOD( SoundEngine, "MusicPaused",    LMusicPaused )
    LUA_NAME_METHOD( SoundEngine, "SoundFXMuted",   LSoundFXMuted )
    LUA_NAME_METHOD( SoundEngine, "SoundFXPaused",  LSoundFXPaused )
    LUA_NAME_METHOD( SoundEngine, "MusicVolume",    LMusicVolume )
    LUA_NAME_METHOD( SoundEngine, "MasterVolume",   LMasterVolume )
    LUA_NAME_METHOD( SoundEngine, "SoundFXVolume",  LSoundFXVolume )
  LUA_END_DEFINE_CLASS


  LUA_DEFINE_METHOD( SoundEngine, LMusicMuted )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushboolean( ls, MusicMuted() );
      return 1;
    }

    int b = lua_toboolean( ls, 1 );
    MusicMuted( b != 0 );
    return 0;
  }

  LUA_DEFINE_METHOD( SoundEngine, LMusicPaused )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushboolean( ls, MusicPaused() );
      return 1;
    }

    int b = lua_toboolean( ls, 1 );
    MusicPaused( b != 0 );
    return 0;
  }

  LUA_DEFINE_METHOD( SoundEngine, LSoundFXMuted )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushboolean( ls, SoundFXMuted() );
      return 1;
    }

    int b = lua_toboolean( ls, 1 );
    SoundFXMuted( b != 0 );
    return 0;
  }

  LUA_DEFINE_METHOD( SoundEngine, LSoundFXPaused )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushboolean( ls, SoundFXPaused() );
      return 1;
    }

    int b = lua_toboolean( ls, 1 );
    SoundFXPaused( b != 0 );
    return 0;
  }

  LUA_DEFINE_METHOD( SoundEngine, LMusicVolume )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushnumber( ls, m_musicVolume );
      return 1;
    }

    float val = luaL_checkfloat( ls, 1 );
    MusicVolume( val );
    return 0;
  }
  
  LUA_DEFINE_METHOD( SoundEngine, LMasterVolume )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushnumber( ls, m_masterVolume );
      return 1;
    }

    float val = luaL_checkfloat( ls, 1 );
    MasterVolume( val );
    return 0;
  }
  
  LUA_DEFINE_METHOD( SoundEngine, LSoundFXVolume )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushnumber( ls, m_soundFXVolume );
      return 1;
    }

    float val = luaL_checkfloat( ls, 1 );
    SoundFXVolume( val );
    return 0;
  }

} // Sound namespace
} // SF namespace
