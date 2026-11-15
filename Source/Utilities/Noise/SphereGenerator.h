/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SphereGenerator.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/30/2009 @ 18:57:23 - nwilliam
**  \brief   NoiseArray generator that spherically maps output.
*******************************************************************************/

#ifndef SF_SPHERE_GENERATOR_H
#define SF_SPHERE_GENERATOR_H

#include "Noise.h"
#include "Math/Constants.h"
#include "BaseGenerator.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  class SphereGenerator : public BaseGenerator
  {
    public:
      SphereGenerator();
      SphereGenerator( int width, int height );
      ~SphereGenerator();

      virtual void Build();
      void BuildAnim();
  };

} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_SPHERE_GENERATOR_H
