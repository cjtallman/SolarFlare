/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    sun_component_drawing.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#include "CSunGraphic.h"
#include "StateManager.h"
#include "Math/Constants.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {
      void CSunGraphic::BeginRender()
      {
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_CULL_FACE );
      }

      void CSunGraphic::EndRender()
      {
        Performance::StateManager::SetActiveVertexShader(0);
        Performance::StateManager::SetActiveFragmentShader(0);
        Performance::StateManager::SetActiveModel(0);
        glPopAttrib();
      }


      void CSunGraphic::BindTexture()
      {
        if( cg_min_radius->param )
          cgSetParameter1f( cg_min_radius->param, min_radius );

        if( cg_max_radius->param )
          cgSetParameter1f( cg_max_radius->param, max_radius );

        height_texture.Bind(0);
        color_texture.Bind(1);
      }

      void CSunGraphic::UnbindTexture()
      {
        height_texture.Unbind();
        color_texture.Unbind();        
      }

      bool CSunGraphic::IsVisible( const CFrustum & frustum )
      {
        return frustum.SphereInside( transform->GetWorldPosition(), max_radius );
      }

      void CSunGraphic::OrientModel()
      {
        if( !transform )
          return;

        Vector3 pos = transform->GetWorldPosition();
        Quaternion rot = transform->GetWorldOrientation();
        Vector3 rotAxis;
        float rotAngle;
        rot.AxisAngle(rotAxis, rotAngle);
        glTranslatef( pos.x, pos.y, pos.z );
        glRotatef(
          Math::Constants::RadToDeg * rotAngle,
          rotAxis.x,
          rotAxis.y,
          rotAxis.z);
      }

      void CSunGraphic::RenderModel()
      {
        Performance::StateManager::SetActiveVertexShader(vert.Get());
        Performance::StateManager::SetActiveFragmentShader(frag.Get());
        model->QuickDraw();
        Performance::StateManager::SetActiveVertexShader(0);
        Performance::StateManager::SetActiveFragmentShader(0);
      }


      void CSunGraphic::Draw( const CFrustum & frustum )
      {
        if( !IsVisible(frustum) )
          return;

        glMatrixMode( GL_MODELVIEW );
        PushMatrix();
        OrientModel();
        BindTexture();
        RenderModel();
        UnbindTexture();
        PopMatrix();
      }


    }//Components
  }//Graphics
}//SF