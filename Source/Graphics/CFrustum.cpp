/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CFrustum.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#include "CFrustum.h"

namespace SF
{
  namespace Graphics
  {
    Vector4 Normalized( const Vector4 & v )
    {
      return v / sqrt( v.x*v.x + v.y*v.y + v.z*v.z );
    }

    CFrustum::CFrustum()
    {
      ExtractPlanes();
    }

    void CFrustum::SetModelview( const Matrix4 & modelview )
    {
      m_modelview = modelview;
      ExtractPlanes();
    }

    void CFrustum::SetProjection( const Matrix4 & projection )
    {
      m_projection = projection;
    }

    bool CFrustum::PointInside( const Vector3 & p ) const
    {
      return SphereInside( p, 0.0f );
    }

    bool CFrustum::SphereInside( const Vector3 & p, float r ) const
    {
      for( int i = 0; i < 6; ++i )
        if( m_frustum[i].x*p.x + m_frustum[i].y*p.y + m_frustum[i].z*p.z + m_frustum[i].w + r < 0 )
          return false;
      return true;
    }

    void CFrustum::ExtractPlanes()
    {
      Matrix4 mvp =  m_projection * m_modelview;

      m_frustum[0] = Normalized(mvp.Col4(3) + mvp.Col4(0));
      m_frustum[1] = Normalized(mvp.Col4(3) - mvp.Col4(0));
      m_frustum[2] = Normalized(mvp.Col4(3) - mvp.Col4(1));
      m_frustum[3] = Normalized(mvp.Col4(3) + mvp.Col4(1));
      m_frustum[4] = Normalized(mvp.Col4(3) + mvp.Col4(2));
      m_frustum[5] = Normalized(mvp.Col4(3) - mvp.Col4(2));
    }

  }
}