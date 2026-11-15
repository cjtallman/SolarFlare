/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    PlaneGenerator.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/30/2009 @ 19:01:54 - nwilliam
**  \brief   NoiseArray generator that maps output to a plane
*******************************************************************************/

#ifndef SF_PLANE_GENERATOR_H
#define SF_PLANE_GENERATOR_H

#include "Noise.h"
#include "BaseGenerator.h"

namespace SF {
namespace Utilities {
namespace Noise
{
  class PlaneGenerator : public BaseGenerator
  {
    public:
      PlaneGenerator();
      virtual ~PlaneGenerator();

      virtual void Build();

      NoiseVal xLower() const;
      NoiseVal yLower() const;
      NoiseVal xUpper() const;
      NoiseVal yUpper() const;
      void xLower( NoiseVal xLower );
      void yLower( NoiseVal yLower );
      void xUpper( NoiseVal xUpper );
      void yUpper( NoiseVal yUpper );
      void SetBounds( NoiseVal xLower, NoiseVal xUpper, NoiseVal yLower, NoiseVal yUpper );

    private:
      NoiseVal m_xLower;
      NoiseVal m_yLower;
      NoiseVal m_xUpper;
      NoiseVal m_yUpper;
  };

} // Noise namespace
} // Utilities namespace
} // SF namespace

#endif // SF_PLANE_GENERATOR_H
