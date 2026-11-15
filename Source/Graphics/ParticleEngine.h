/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ParticleEngine.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:29:23 CTallman
**  \brief   
*******************************************************************************/

#ifndef SF_PARTICLE_ENGINE_H_
#define SF_PARTICLE_ENGINE_H_

#include <vector>

#include "Particle.h"
#include "CBaseEmitter.h"

#include "Utilities/SharedPtr.h"
#include "Texture2D.h"
#include "TextureAtlas.h"

#include "ShaderLoader.h"


using namespace SF::Math;

namespace SF
{
    namespace ParticleSystem
    {
      class Particle;
  
      class DepthSorter
      {
      public:
        DepthSorter(){}
        bool operator()(const Particle & A, const Particle & B)
        {
          return A.depth < B.depth;
        }
      };


      class ParticleEngine
      {
        static const int expand_amount = 2500;
        static const int maximum_particles = 10000;

      public:
        ParticleEngine( void );
        ~ParticleEngine( void );

        void Update( float time_elapsed );

        //Called by emitters
        void CreateParticle( const Particle & p );
        void DeleteParticle( Particle * p );
        
        //Called by Graphics
        void Draw( void );

        void Pause();
        void Unpause();
        void TogglePause();

        void Enable();
        void Disable();
        void Clear();

        float PortionUsed() const;
        void Initialize();

        void SetVertexShader( const std::string & shader );
        void SetVertexShader( Graphics::SharedShader2 shader );
        void SetFragmentShader( const std::string & shader );
        void SetFragmentShader( Graphics::SharedShader2 shader );

      private:
        
        
        bool initialized;

        bool UpdateParticle( Particle * p, float time_elapsed );
        bool IsFull( void );
        bool IsEmpty( void );
        bool Expand( void );

        void BindTexture();
        void UnbindTexture();

        void RadixSort();
        std::vector<Particle *> sortedParticles;
        bool m_sortReverse;
        Particle *m_particleBuffer;

        void SortParticles();

        int GetArrayEnd() { return virtual_end; }

        std::vector<Particle> particles;
        int virtual_end;
        int true_end;

		    Utilities::SharedPtr<Graphics::TextureType::TextureAtlas2D> particle_texture;

        Graphics::SharedShader2 frag;
        Graphics::SharedShader2 vert;

        bool m_pause;
        bool m_enabled;
      };
    }
}

#endif
