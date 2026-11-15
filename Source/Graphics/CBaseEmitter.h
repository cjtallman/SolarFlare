/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CBaseEmitter.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:35:07 CTallman
**  \brief   
*******************************************************************************/

#ifndef _SF_CBASE_EMITTER_H_
#define _SF_CBASE_EMITTER_H_

#include "Math/CTransform.h"
#include "Graphics/Particle.h"
#include "Engine/BaseComponent.h"
#include "Graphics/ParticleEngine.h"

namespace ticpp
{
  class Element;
}

namespace SF
{
  namespace ParticleSystem
  {
    class ParticleEngine;

    class CBaseEmitter : public Engine::BaseComponent
    {
    public:
      CBaseEmitter() : Engine::BaseComponent( Engine::ComponentType::EMITTER ), m_transform(0) {}
      virtual ~CBaseEmitter();

      virtual void Update( float ){}
      //virtual bool Behavior( Particle &, float ) = 0;
      
      virtual BaseComponent * Clone() const = 0;
      virtual bool InitDependencies( Engine::Entity *entity ) = 0;

      Math::Vector3      GetPosition() const;
      Math::Quaternion  GetOrientation() const;

    protected:
      Math::CTransform *m_transform;
    };


  }
}

#endif
