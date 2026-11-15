/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    KeplerOrbit.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/19/2008 @ 18:35:12 - pmera
**  \brief   
*******************************************************************************/

#include <cmath>

#include "KeplerOrbit.h"

#include "Math/Constants.h"

namespace SF {
namespace Physics
{

KeplerOrbit::KeplerOrbit()
{
  period = 0;
  perihelionDistance = 0;
  sunDistance = 0;
  timeShift = 0;
}

KeplerOrbit::KeplerOrbit(
  float sunDistance,
  float perihelionDistance,
  float period)
{
  period = period;
  perihelionDistance = perihelionDistance;
  sunDistance = sunDistance;

  Update();
}

void KeplerOrbit::Update()
{
  m_semiMajorAxis = perihelionDistance;
  m_eccentricity = sunDistance / m_semiMajorAxis;
  m_semiMinorAxis = m_semiMajorAxis * std::sqrt( 1 - m_eccentricity * m_eccentricity );
  m_semiLatusRectum = (m_semiMinorAxis * m_semiMinorAxis) / m_semiMajorAxis;
}

Math::Vector3 KeplerOrbit::GetPosition(float time)
{
  if(perihelionDistance < 0.001f)
    return Math::Vector3(0,0,0);

  time += timeShift * period;

  m_meanAnomaly = float(Math::Constants::p_2_pi) * time / period;

  float& M = m_meanAnomaly;
  float& e = m_eccentricity;

  m_eccentricAnomaly = M;

  for(int i = 1; i < 5; ++i)
    m_eccentricAnomaly = M + e * std::sin( m_eccentricAnomaly );

  m_trueAnomaly = 2 * std::atan(std::sqrt((1+e/1-e)) * std::tan(m_eccentricAnomaly * 0.5f));

  m_heliocentricDistance = m_semiMajorAxis * ( 1 - e*e ) / ( 1 + e * std::cos(m_trueAnomaly) );

  Math::Vector3 position(
    m_heliocentricDistance * std::cos( m_trueAnomaly ), 
    0.0f,
    m_heliocentricDistance * std::sin( m_trueAnomaly ));

  return position;
}

}

}
