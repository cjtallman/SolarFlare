/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SoundEngine.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/04/2009 @ 18:27:29 - nwilliam
**  \brief   FMOD Ex based sound engine.
*******************************************************************************/

#ifndef SF_SOUND_ENGINE_H
#define SF_SOUND_ENGINE_H

#include "CSound.h"
#include "Engine/Entity.h"
#include "Engine/BaseEngine.h"
#include "Script/ScriptHelper.h"
#include "Utilities/HashTable.h"
#include "External/FMOD/include/fmod.hpp"

// Forward declaration
namespace ticpp { class Element; }

namespace SF {
namespace Sound
{
  class SoundEngine : public Engine::BaseEngine
  {
    public:
      SoundEngine();
      virtual ~SoundEngine();

      void Update( float elapsed );

      float MusicVolume() const;
      float MasterVolume() const;
      float SoundFXVolume() const;
      
      void MusicVolume( float val );
      void MasterVolume( float val );
      void SoundFXVolume( float val );
      
      bool MusicMuted() const;
      bool MusicPaused() const;
      void MusicMuted( bool muted );
      void MusicPaused( bool paused );

      bool SoundFXMuted() const;
      bool SoundFXPaused() const;
      void SoundFXMuted( bool muted );
      void SoundFXPaused( bool paused );

      void CSoundCreateTrack( Engine::EntityID id );
      void CSoundDeleteTrack( Engine::EntityID id );
      Engine::BaseComponent* CSoundCreator( const ticpp::Element* );

      void PlayCSound( CSound& sound );

    private:
      typedef Utilities::HashTable< FMOD::Sound* > SoundTable;

      void InitFMOD();
      FMOD::Sound* CreateSound( const std::string& path, bool _3D );
      FMOD::Sound* CreateStream( const std::string& path );

      std::list< CSound* > toPlay;

      float m_musicVolume;
      float m_masterVolume;
      float m_soundFXVolume;

      SoundTable m_sounds;
      FMOD::System* m_system;
      FMOD::ChannelGroup* m_musicGroup;
      FMOD::ChannelGroup* m_soundFXGroup;
      Engine::EntityIDList m_soundEntities;


      LUA_DECLARE_CLASS( SoundEngine )
      LUA_DECLARE_METHOD( LMusicMuted )
      LUA_DECLARE_METHOD( LMusicPaused )
      LUA_DECLARE_METHOD( LSoundFXMuted )
      LUA_DECLARE_METHOD( LSoundFXPaused )
      LUA_DECLARE_METHOD( LMusicVolume )
      LUA_DECLARE_METHOD( LMasterVolume )
      LUA_DECLARE_METHOD( LSoundFXVolume )
  };

} // Sound namespace
} // SF namespace

#endif // SF_SOUND_ENGINE_H
