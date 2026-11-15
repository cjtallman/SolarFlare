/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    vectorField.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 12/24/2008 @ 15:35:56 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_VECTORFIELD_H_
#define SF_VECTORFIELD_H_

#include <vector>

#include "Math/TVector3.h"
#include "Utilities/Delegates.h"

namespace SF {
namespace Physics
{

using namespace Math;
using namespace Utilities;

class VectorField
{
public:
  typedef std::vector< std::vector< std::vector< Vector3 > > > VectorContainer;
  typedef Delegate1<Vector3, const Vector3&> VectorFunction;

  void Build(const Vector3& center, const Vector3& extents, float spacing, const VectorFunction& vectorFunction);

  Vector3 GetVector(const Vector3& position);

private:
  Vector3         m_center;
  Vector3         m_extents;
  VectorContainer m_vectors;
};

}

}

#endif
