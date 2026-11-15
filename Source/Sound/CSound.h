/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CSound.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/17/2009 @ 19:37:02 - nwilliam
**  \brief   FMOD Ex based sound component.
*******************************************************************************/

#ifndef SF_C_SOUND_EFFECT_H
#define SF_C_SOUND_EFFECT_H

#include "Math/CTransform.h"
#include "Physics/CPhysics.h"
#include "Engine/BaseComponent.h"
#include "External/FMOD/include/fmod.hpp"

namespace SF {
namespace Sound
{
  class CSound : public Engine::BaseComponent
  {
    public:
      CSound();
      CSound( const CSound& rhs );
      virtual ~CSound();

      virtual BaseComponent* Clone() const;
      virtual void PushToLua( lua_State* ls );
      virtual bool InitDependencies( Engine::Entity *entity );

    private:
      friend class SoundEngine;

      void Play();
      void Stop();

      void Mute();
      void UnMute();

      void Pause();
      void UnPause();

      void Update();

      bool m_playMe;
      int m_loopCount;
      bool m_positional;
      Math::CTransform* m_ct;   // If exists, will be positioned
      Physics::CPhysics* m_cp;  // Non-required dependency (will use if exists)

      FMOD::Sound* m_sound;
      FMOD::Channel* m_channel;
      FMOD::ChannelGroup* m_group;


      LUA_DECLARE_CLASS( CSound )
      LUA_DECLARE_METHOD( LPlay )
      LUA_DECLARE_METHOD( LStop )
      LUA_DECLARE_METHOD( LMute )
      LUA_DECLARE_METHOD( LUnMute )
      LUA_DECLARE_METHOD( LPause )
      LUA_DECLARE_METHOD( LUnPause )
  };

} // Sound namespace
} // SF namespace

#endif // SF_C_SOUND_EFFECT_H
