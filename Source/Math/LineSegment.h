/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    LineSegment.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/05/2008 @ 14:31:33 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_LINESEGMENT_H_
#define SF_LINESEGMENT_H_

#include "TVector3.h"

#include "Line3.h"

namespace SF {
namespace Math
{

struct LineSegment
{
  Vector3 start;
  Vector3 end;

  LineSegment();
  LineSegment(const Vector3& _start, const Vector3& _end);
  
  Vector3 operator()(float t) const;

  Line3 ToLine() const;
};

}
}

#endif
