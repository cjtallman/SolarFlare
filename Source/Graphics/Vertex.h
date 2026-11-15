/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Vertex.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:43:39 - nwilliam
**  \brief   Graphics vertex definition / attributes.
*******************************************************************************/

#ifndef SF_VERTEX_H
#define SF_VERTEX_H

#include "Math/TVector2.h"
#include "Math/TVector3.h"
#include "Math/TVector4.h"
#include "Math/TVector2.h"
#include "Math/TVector3.h"
#include "Math/TVector4.h"
#include "Utilities/Color.h"

#include <cstddef>

using namespace SF;
using namespace Math;

namespace ATTRIBUTES
{
  enum Attributes
  {
    POSITION = 0,
    ATTR0 = 0,

    BLENDWEIGHT = 1,
    BLENDWEIGHT0 = 1,
    ATTR1 = 1,

    NORMAL = 2,
    NORMAL0 = 2,
    ATTR2 = 2,

    COLOR = 3,
    COLOR0 = 3,
    DIFFUSE = 3,
    ATTR3 = 3,

    COLOR1 = 4,
    SPECULAR = 4,
    ATTR4 = 4,

    TESSFACTOR = 5,
    FOGCOORD = 5,
    TESSFACTOR0 = 5,
    FOGCOORD0 = 5,
    ATTR5 = 5,

    PSIZE = 6,
    PSIZE0 = 6,
    ATTR6 = 6,

    BLENDINDICES = 7,
    BLENDINDICES0 = 7,
    ATTR7 = 7,

    TEXCOORD = 8,
    TEXCOORD0 = 8,
    ATTR8 = 8,

    TEXCOORD1 = 9,
    ATTR9 = 9,

    TEXCOORD2 = 10,
    ATTR10 = 10,

    TEXCOORD3 = 11,
    ATTR11 = 11,

    TEXCOORD4 = 12,
    ATTR12 = 12,

    TEXCOORD5 = 13,
    ATTR13 = 13,

    TANGENT = 14,
    TANGENT0 = 14,
    TEXCOORD6 = 14,
    ATTR14 = 14,

    BINORMAL = 15,
    BINORMAL0 = 15,
    TEXCOORD7 = 15,
    ATTR15 = 15
  };
}

#include "ParticleVertex.h"
#include "HeightVertex.h"
#include "PlanetVertex.h"
#include "ModelVertex.h"
#include "DeformVertex.h"


#endif // SF_VERTEX_H
