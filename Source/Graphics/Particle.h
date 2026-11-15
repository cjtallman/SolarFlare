/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Particle.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:35:18 CTallman
**  \brief   
*******************************************************************************/

#ifndef SF_PARTICLE_H_
#define SF_PARTICLE_H_

#include "Math/TVector3.h"
#include "Math/TVector2.h"
#include "Math/TVector3.h"
#include "Utilities/Color.h"

#include "Utilities/Delegates.h"




namespace SF
{
  namespace ParticleSystem
  {
    class BaseParticleBehavior;

    class Particle
    {
    public:
      Particle();
      ~Particle();

      bool Update(float elapsed);

      bool operator >= ( const Particle & p );

      float         additivity;
      Color         color;
      float         depth;
      Math::Vector3 position;
      float         size;
      Math::Vector2 texcoord_offset;
  	  Math::Vector2 texcoord_size;
      unsigned int  type;
      unsigned char version;

      BaseParticleBehavior *behavior;
    };

    class BaseParticleBehavior
    {
      public:
        virtual ~BaseParticleBehavior() {}
        virtual bool Update(float elapsed, Particle& particle) = 0;
    };
  }
}
#endif
