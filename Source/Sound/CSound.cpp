/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CSound.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/17/2009 @ 19:37:16 - nwilliam
**  \brief   Sound component implementation.
*******************************************************************************/

#include "CSound.h"
#include "SoundEngine.h"
#include "SolarFlare/Globals.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Sound
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors / destructor
  //////////////////////////////////////////////////////////////////////////////
  CSound::CSound() : Engine::BaseComponent( Engine::ComponentType::SOUND ),
                     m_loopCount( 0 ),
                     m_positional( false ),
                     m_ct( 0 ),
                     m_cp( 0 ),
                     m_sound( 0 ),
                     m_channel( 0 ),
                     m_group( 0 ),
                     m_playMe( false )
  {
  }

  CSound::CSound( const CSound& rhs ) : Engine::BaseComponent( rhs ),
                                        m_loopCount( rhs.m_loopCount ),
                                        m_positional( rhs.m_positional ),
                                        m_ct( 0 ),
                                        m_cp( 0 ),
                                        m_sound( rhs.m_sound ),
                                        m_channel( 0 ),
                                        m_group( rhs.m_group ),
                                        m_playMe( false )
  {
  }

  CSound::~CSound()
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  Engine::BaseComponent* CSound::Clone() const
  {
    return new CSound( *this );
  }

  void CSound::PushToLua( lua_State *ls )
  {
    Lunar<CSound>::push( ls, this );
  }

  bool CSound::InitDependencies( Engine::Entity* entity )
  {
    if( !entity )
    {
      return false;
    }

    if( m_positional )
    {
      if( !entity->Has( Engine::ComponentType::TRANSFORM ) )
      {
        return false;
      }

      m_ct = (Math::CTransform*)entity->GetFirstComponent( Engine::ComponentType::TRANSFORM );

      if( entity->Has( Engine::ComponentType::PHYSICS ) )
      {
        m_cp = (Physics::CPhysics*)entity->GetFirstComponent( Engine::ComponentType::PHYSICS );
      }
    }

    return true;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void CSound::Play()
  {
    Globals::soundEngine->PlayCSound( *this );
    m_playMe = true;
  }

  void CSound::Stop()
  {
    m_channel->stop();
  }

  void CSound::Mute()
  {
    m_channel->setMute( true );
  }

  void CSound::UnMute()
  {
    m_channel->setMute( false );
  }

  void CSound::Pause()
  {
    m_channel->setPaused( true );
  }

  void CSound::UnPause()
  {
    m_channel->setPaused( false );
  }

  void CSound::Update()
  {
    if( m_positional )
    {
      Math::Vector3 v;
      Math::Vector3 p = m_ct->GetWorldPosition();

      if( m_cp )
      {
        v = m_cp->GetVelocity();
      }

      FMOD_VECTOR vel = { v.x, v.y, v.z };
      FMOD_VECTOR pos = { p.x, p.y, p.z };

      m_channel->set3DAttributes( &pos, &vel );
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // Lua methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( CSound )
    LUA_NAME_METHOD( CSound, "Play",  LPlay )
    LUA_NAME_METHOD( CSound, "Stop",  LStop )
    LUA_NAME_METHOD( CSound, "Mute",  LMute )
    LUA_NAME_METHOD( CSound, "Mute",  LUnMute )
    LUA_NAME_METHOD( CSound, "Pause", LPause )
    LUA_NAME_METHOD( CSound, "Pause", LUnPause )
  LUA_END_DEFINE_CLASS


  LUA_DEFINE_METHOD_NOLS( CSound, LPlay )
  {
    Play();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( CSound, LStop )
  {
    Stop();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( CSound, LMute )
  {
    Mute();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( CSound, LUnMute )
  {
    UnMute();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( CSound, LPause )
  {
    Pause();
    return 0;
  }

  LUA_DEFINE_METHOD_NOLS( CSound, LUnPause )
  {
    UnPause();
    return 0;
  }

} // Sound namespace
} // SF namespace
