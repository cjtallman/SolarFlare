/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    KeplerOrbit.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/19/2008 @ 18:35:08 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_ORBIT_H_
#define SF_ORBIT_H_

#include "Math/Quaternion.h"
#include "Math/TVector3.h"
#include "Math/TVector3.h"

namespace SF {
namespace Physics
{

class KeplerOrbit
{
public:
  KeplerOrbit();

  KeplerOrbit(
    float sunDistance,
    float perihelionDistance,
    float period);

  Math::Vector3 GetPosition(float t);
  void          Update();

  float             sunDistance;
  float             perihelionDistance;
  float             period;
  float             timeShift;

private:
  friend class COrbitalPhysics;
  
  float m_eccentricAnomaly;
  float m_eccentricity;
  float m_heliocentricDistance;
  float m_meanAnomaly;
  float m_semiMajorAxis;
  float m_semiMinorAxis;
  float m_semiLatusRectum;
  float m_trueAnomaly;
};


}

}

#endif
