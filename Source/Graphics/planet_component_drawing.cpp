/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    planet_component_drawing.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:31:58 CTallman
**  \brief   Handles drawing the planet component.
*******************************************************************************/

#include "CPlanetGraphic.h"
#include "StateManager.h"
#include "Math/Constants.h"


namespace SF
{
  namespace Graphics
  {
    namespace Components
    {


      //************************************
      // Method:    BindTexture
      // FullName:  SF::Graphics::Components::CPlanetGraphic::BindTexture
      // Access:    protected 
      // Returns:   void
      // Qualifier:
      //************************************
      void CPlanetGraphic::BindTexture()
      {
        if( cg_min_radius->param )
          cgSetParameter1f( cg_min_radius->param, min_radius );

        if( cg_max_radius->param )
          cgSetParameter1f( cg_max_radius->param, max_radius );

        if( cg_tex_width->param )
          cgSetParameter1f( cg_tex_width->param, (float)height_texture.Width() );

        if( cg_tex_height->param )
          cgSetParameter1f( cg_tex_height->param, (float)height_texture.Height() );

        height_texture.Bind(0);
        color_texture.Bind(1);
        deform_texture.Bind(2);
      }

      //************************************
      // Method:    UnbindTexture
      // FullName:  SF::Graphics::Components::CPlanetGraphic::UnbindTexture
      // Access:    protected 
      // Returns:   void
      // Qualifier:
      //************************************
      void CPlanetGraphic::UnbindTexture()
      {
        height_texture.Unbind();
        color_texture.Unbind();
        deform_texture.Unbind();
      }

      //************************************
      // Method:    BeginRender
      // FullName:  SF::Graphics::Components::CPlanetGraphic::BeginRender
      // Access:    public 
      // Returns:   void
      // Qualifier:
      //************************************
      void CPlanetGraphic::BeginRender()
      {
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_CULL_FACE );
      }

      //************************************
      // Method:    EndRender
      // FullName:  SF::Graphics::Components::CPlanetGraphic::EndRender
      // Access:    public 
      // Returns:   void
      // Qualifier:
      //************************************
      void CPlanetGraphic::EndRender()
      {
        Performance::StateManager::SetActiveVertexShader(0);
        Performance::StateManager::SetActiveFragmentShader(0);
        Performance::StateManager::SetActiveModel(0);
        glPopAttrib();    
      }

      void CPlanetGraphic::Update(float /*elapsed*/)
      {
        --deform_delay;
        if( deform_delay == 0 )
        {
          deform_delay = 10;
          Deform();
        }
      }

      void CPlanetGraphic::RenderModel()
      {
        Performance::StateManager::SetActiveVertexShader(vert.Get());
        Performance::StateManager::SetActiveFragmentShader(frag.Get());

        if( height_buffer->Count() != model->VertexCount(0) )
          height_buffer->Resize(model->VertexCount(0));

        height_buffer->Load();
        model->QuickDraw();
        height_buffer->Unload();

        Performance::StateManager::SetActiveVertexShader(0);
        Performance::StateManager::SetActiveFragmentShader(0);
      }


      //************************************
      // Method:    Draw
      // FullName:  SF::Graphics::Components::CPlanetGraphic::Draw
      // Access:    public 
      // Returns:   void
      // Qualifier:
      // Parameter: const QuatCamera *
      //************************************
      void CPlanetGraphic::Draw( const CFrustum & frustum )
      {
        if(!IsVisible(frustum))
          return;

        glMatrixMode( GL_MODELVIEW );
        PushMatrix();
        OrientModel();
        BindTexture();       
        RenderModel();
        UnbindTexture();
        PopMatrix();
        
        if( updated_heights == false )
        {  
          height_texture.Recreate();
          UpdateHeightBuffer(); 
        }

      }  

      




    }
  }
}