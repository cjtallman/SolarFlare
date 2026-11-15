/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ParticleEngine.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:26:14 CTallman
**  \brief
*******************************************************************************/

#include "OpenGLInclude.h"
#include "ParticleEngine.h"

#include <sstream>
#include <algorithm>

#include "Engine/AssetManager.h"
#include "Graphics/GraphicsEngine.h"
#include "SolarFlare/Globals.h"
#include "Utilities/DevConsole.h"
#include "Utilities/Misc.h"
#include "StateManager.h"

#include "Utilities/SmoothSort.h"

#include "Utilities/LeakCheck.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace SF
{
  namespace ParticleSystem
  {

    ParticleEngine::ParticleEngine( void ) : particles(expand_amount),
      true_end(expand_amount),
      virtual_end(0)
    {      
      m_sortReverse = false;
      initialized = false;

      m_pause = false;
      m_enabled = true;

      frag = new Graphics::CgShader();
      vert = new Graphics::CgShader();
    }

    ParticleEngine::~ParticleEngine( void )
    {
    }

    void ParticleEngine::SortParticles()
    {
      Utilities::smoothsort<Particle>(&particles[0], virtual_end, DepthSorter() );
    }

    void ParticleEngine::Update( float time_elapsed )
    {
      if( !m_enabled )
        return;

      Math::Vector3 view = Globals::graphicsEngine->CameraView();

      for( int i = 0; i < virtual_end; )
      {
        if(!m_pause)
        {
          if( UpdateParticle( &particles[i], time_elapsed ) )
            ++i;
        }
        else
          ++i;

        Math::Vector3 cameraToParticle = particles[i].position - Globals::graphicsEngine->CameraPosition();

        particles[i].depth = cameraToParticle * view;
      }
    }

    void GetTexCoords( int version, int corner, float & u, float & v )
    {
      switch( version )
      {
      default:
      case 0:
        switch( corner )
        {
        case 0:
          u = 0.0f;
          v = 1.0f;
          break;
        case 1:
          u = 1.0f;
          v = 1.0f;
          break;
        case 2:
          u = 1.0f;
          v = 0.0f;
          break;
        case 3:
          u = 0.0f;
          v = 0.0f;
          break;
        }
        break;
      }
    }


      void ParticleEngine::SetVertexShader( Graphics::SharedShader2 shader )
      {
        vert = shader;
      }

      void ParticleEngine::SetFragmentShader( Graphics::SharedShader2 shader )
      {
        frag = shader;
      }

      void ParticleEngine::SetVertexShader( const std::string & shader )
      {
        vert = Globals::shaderLoader->GetVertexShader(shader);
      }

      void ParticleEngine::SetFragmentShader( const std::string & shader )
      {
        frag = Globals::shaderLoader->GetFragmentShader(shader);
      }



    void ParticleEngine::Initialize()
    {
      initialized = true;
      SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
      SF::Engine::Assets::TextureAtlas * texture = (SF::Engine::Assets::TextureAtlas*)asset_manager->GetAsset( "particle_atlas" );

      if( texture )
        particle_texture = texture->image;

      SetVertexShader( "particle_vert" );
      SetFragmentShader( "particle_frag" );
    }

    void ParticleEngine::Draw( void )
    {
      if( !m_enabled || IsEmpty() )
        return;

      SortParticles();

      glPushAttrib( GL_ALL_ATTRIB_BITS  );
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
      glDepthMask(false);

      BindTexture();
      Graphics::Performance::StateManager::SetActiveFragmentShader(frag.Get());
      Graphics::Performance::StateManager::SetActiveVertexShader(vert.Get());

      glBegin(GL_QUADS);
      for( int i = 0; i < virtual_end; ++i )
      {
        glMultiTexCoord1f(GL_TEXTURE1, particles[i].size );
        glMultiTexCoord2f(GL_TEXTURE2, particles[i].texcoord_offset.x, particles[i].texcoord_offset.y );
        glMultiTexCoord2f(GL_TEXTURE3, particles[i].texcoord_size.x, particles[i].texcoord_size.y );
        glMultiTexCoord1f(GL_TEXTURE4, particles[i].additivity );

        glColor4ubv( particles[i].color.Array() );
        glNormal3fv( (float*)&particles[i].position );

        for( int j = 0; j < 4; ++j )
        {
          switch( (j+particles[i].version) & 3 )
          {   
            case 0:
              glTexCoord2f(0, 1);
              break;
            case 1:
              glTexCoord2f(1, 1);
              break;
            case 2:
              glTexCoord2f(1, 0);
              break;
            case 3:
              glTexCoord2f(0, 0);
              break;
          }
          switch( j )
          {   
            case 0:
              glVertex2f(-0.5f, -0.5f);
              break;
            case 1:
              glVertex2f(0.5f, -0.5f);
              break;
            case 2:
              glVertex2f(0.5f, 0.5f);
              break;
            case 3:
              glVertex2f(-0.5f, 0.5f);
              break;
          }
        }
      }
      glEnd();

      Graphics::Performance::StateManager::SetActiveFragmentShader(0);
      Graphics::Performance::StateManager::SetActiveVertexShader(0);
      UnbindTexture();

      glPopAttrib();
    }

    void ParticleEngine::Pause()
    {
      m_pause = true;
    }

    void ParticleEngine::Unpause()
    {
      m_pause = false;
    }

    void ParticleEngine::TogglePause()
    {
      m_pause = !m_pause;
    }

    void ParticleEngine::Enable()
    {
      m_enabled = true;
    }

    void ParticleEngine::Disable()
    {
      m_enabled = false;
    }

    void ParticleEngine::Clear()
    {
      virtual_end = 0;
    }

    float ParticleEngine::PortionUsed() const
    {
      return float(virtual_end) / maximum_particles;
    }

    void ParticleEngine::BindTexture()
    {
      if( initialized == false )
        Initialize();

      particle_texture->texture.Bind(0);
    }

    void ParticleEngine::UnbindTexture()
    {
      particle_texture->texture.Unbind();
    }

    bool ParticleEngine::UpdateParticle( Particle * p, float time_elapsed )
    {
      if( p->Update(time_elapsed) == false )
      {
        DeleteParticle(p);
        return false;
      }
      else
        return true;
    }

    bool ParticleEngine::IsFull( void )
    {
      return ( virtual_end == true_end );
    }

    bool ParticleEngine::IsEmpty( void )
    {
      return ( virtual_end == 0 );
    }

    bool ParticleEngine::Expand( void )
    {
      if( true_end >= maximum_particles )
        return false;
      else
      {
        size_t expand = Utilities::Min(expand_amount, maximum_particles - (int)particles.size());

        particles.resize( particles.size() + expand );
        true_end += (int)expand;
      }
      return true;
    }

    void ParticleEngine::CreateParticle( const Particle & p )
    {
      if( !m_enabled )
        return;

      if( IsFull() && !Expand() )
        return;

      if( initialized == false )
        Initialize();

      unsigned int type = p.type;
      Graphics::TextureType::TextureAtlasInfo info = particle_texture->TextureInfo(type);

      particles[virtual_end++] = p;
      particles[virtual_end-1].texcoord_offset = Math::Vector2( info.u_offset, info.v_offset );
      particles[virtual_end-1].texcoord_size = Math::Vector2( info.width, info.height );
      particles[virtual_end-1].version = (char)(virtual_end & 3);
    }

    void ParticleEngine::DeleteParticle( Particle * p )
    {
      if( IsEmpty() )
        return;
      else
      {
        delete p->behavior;
        *p = particles[--virtual_end];
      }
    }
  }
}
