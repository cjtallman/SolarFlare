/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CFrustum.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#ifndef SF_CFRUSTUM_H_
#define SF_CFRUSTUM_H_

#include "Math/TVector3.h"
#include "Math/TVector4.h"
#include "Math/TMatrix4x4.h"

using namespace SF::Math;

namespace SF
{
  namespace Graphics
  {

    class CFrustum
    {
    public:
      CFrustum();

      void SetModelview( const Matrix4 & modelview );
      void SetProjection( const Matrix4 & projection );

      bool PointInside( const Vector3 & point ) const;
      bool SphereInside( const Vector3 & point, float radius ) const;

    private:
      void ExtractPlanes();

      Matrix4 m_modelview;
      Matrix4 m_projection;
      Vector4 m_frustum[6];
    };

  }
}

#endif
