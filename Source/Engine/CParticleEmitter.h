/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CParticleEmitter.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:40:14 - nwilliam
**  \brief   Basic include for a particle and particle emitter.
*******************************************************************************/

#ifndef SF_CPARTICLE_EMITTER_H
#define SF_CPARTICLE_EMITTER_H

#include <vector>

#include "BaseComponent.h"
#include "Math/TVector3.h"
#include "Math/CTransform.h"
#include "Utilities/Color.h"

namespace SF {
namespace Engine {
namespace ParticleSystem
{
  class BaseParticle
  {
    public:
      Math::Vector3 position;
  };


  class BaseEmitter : public BaseComponent
  {
    public:
      BaseEmitter() : BaseComponent( ComponentType::EMITTER ), transform(0) {}

      virtual void Update(float) = 0;
      virtual int ParticleSize() { return 10; }

      virtual Engine::BaseComponent* Clone() const = 0;
      virtual bool InitDependencies( Engine::Entity* ) = 0;

      virtual void CreateParticle(){}
      virtual void KillParticle( BaseParticle * ){}
      virtual void Draw(){}

      virtual void InitSize( int ){};

    protected:
      Math::CTransform * transform; //DEPENDENCY
  };

} // ParticleSystem namespace
} // Engine namespace
} // SF namespace

#endif // SF_CPARTICLE_EMITTER_H
