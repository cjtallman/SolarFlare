/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Particle.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:35:12 CTallman
**  \brief   
*******************************************************************************/

#include "Particle.h"


namespace SF
{
  namespace ParticleSystem
  {
    Particle::Particle()
    {
      additivity = 1;
      size = 1;
      type = 1;

      behavior = 0;
    }

    Particle::~Particle()
    {
      // Note: Behavior is (should) be deleted in ParticleEngine::DeleteParticle
      //delete behavior;
    }

    bool Particle::Update(float time_elapsed)
    { 
      if(behavior)
      {
        return behavior->Update(time_elapsed, *this);
      }
      else
        return false;
    }
  }
}
