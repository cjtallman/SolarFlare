/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CBaseEmitter.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:35:04 CTallman
**  \brief   
*******************************************************************************/

#include "CBaseEmitter.h"

namespace SF
{
  namespace ParticleSystem
  {
    CBaseEmitter::~CBaseEmitter()
    { 
    }

    Math::Vector3 CBaseEmitter::GetPosition() const
    {
      return m_transform->GetWorldPosition();
    }

    Math::Quaternion CBaseEmitter::GetOrientation() const
    {
      return m_transform->GetWorldOrientation();
    }
  }
}
