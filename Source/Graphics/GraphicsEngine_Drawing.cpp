/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    GraphicsEngine_Drawing.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#include "GraphicsEngine.h"
#include "Engine/EntityFactory.h"
#include "GraphicsComponents.h"
#include "SolarFlare/Globals.h"
#include "Engine/InputEngine.h"
#include "ParticleEngine.h"

#include <sstream>
#include <string>

namespace SF
{
  namespace Graphics
  {
    void GraphicsEngine::RenderComponents( float elapsed )
    {
      DrawSkybox( elapsed );
      DrawSun( elapsed );
      DrawPlanets( elapsed );
      DrawModels( elapsed );
      DrawWorldQuads( elapsed );
      DrawParticles( elapsed );
      DrawEllipses( elapsed );
    }

    void GraphicsEngine::DrawConsole()
    {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      Globals::console->DrawAllMessages();
      glPopAttrib();
    }
    void GraphicsEngine::DrawEllipses( float /*elapsed*/ )
    {
      glPushAttrib( GL_ALL_ATTRIB_BITS );
      glEnable(GL_BLEND);
      glEnable( GL_DEPTH_TEST );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glEnable(GL_LINE_SMOOTH);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      glLineWidth(1);
      for( EntitySet::iterator itr = ellipse_components.begin(); itr != ellipse_components.end(); ++itr )
      {
        Engine::Entity * current_entity = entity_factory->GetEntity(*itr);

        SF::Engine::ComponentList & comlist = current_entity->GetComponentsByType(Engine::ComponentType::ELLIPSE);
        for( Engine::ComponentList::iterator com = comlist.begin(); com != comlist.end(); ++com )
          ((Components::CEllipse*)(*com))->Draw();
      }
      glPopAttrib();
    }

    void GraphicsEngine::DrawFPS( float time_elapsed )
    {
      static int frames = 0;
      static float times = 0;
      //static std::stringstream frame_count_display;
      times += time_elapsed;
      if( times > 1000 )
      {
        //frame_count_display.str("");
        //frame_count_display << "FPS: " << frames;
        m_fps = frames;
        frames = 0;
        times = 0;
      }
      else
        frames++;

      //DrawString( 0, WindowHeight() - 20, frame_count_display.str() );
    }

    void GraphicsEngine::DrawLines()
    {
      
      glBegin( GL_LINES );
      glColor4f(1,1,1,1);
      for(unsigned int i = 0; i < m_lines.size(); ++i)
      {
        glVertex3f(m_lines[i].start.x, m_lines[i].start.y, m_lines[i].start.z);
        glVertex3f(m_lines[i].end.x, m_lines[i].end.y, m_lines[i].end.z);
      }

      glEnd();

      m_lines.clear();
    }

    void GraphicsEngine::DrawModels( float /*elapsed*/ )
    {
      std::vector<Components::CModel*> opaque_models;
      std::vector<Components::CModel*> transp_models;

      Math::Vector3 view = CameraView();

      for( EntitySet::iterator itr = model_components.begin(); itr != model_components.end(); ++itr )
      {
        Engine::Entity * current_entity = entity_factory->GetEntity(*itr);
        SF::Engine::ComponentList & comlist = current_entity->GetComponentsByType(Engine::ComponentType::MODEL);
        for( Engine::ComponentList::iterator com = comlist.begin(); com != comlist.end(); ++com )
        {
          Components::CModel * model = (Components::CModel*)(*com);
          Math::Vector3 cameraToModel = model->WorldPosition() - CameraPosition();
          float depth = cameraToModel * view;
          model->Depth(depth);

          if( ((Components::CModel*)(*com))->IsOpaque() )
          {
            opaque_models.push_back(model);
          }
          else
          {
            transp_models.push_back(model);
          }
        }
      }

      Components::CModel::BeginRender();
      glEnable( GL_DEPTH_TEST );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ZERO );
      for( size_t i = 0; i < opaque_models.size(); ++i )
        opaque_models[i]->Draw(m_frustum);

      glDepthMask(false);
      glDisable(GL_CULL_FACE);
      glBlendFunc( GL_SRC_ALPHA, GL_ONE );
      if( !transp_models.empty() )
      {
        std::sort(transp_models.begin(), transp_models.end(),Components::CModel::SorterPtr);
        for( size_t i = 0; i < transp_models.size(); ++i )
          transp_models[i]->Draw(m_frustum);
      }
      Components::CModel::EndRender();
    }



    void GraphicsEngine::DrawParticles( float /*elapsed*/ )
    {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      Globals::particleEngine->Draw();
      glPopAttrib();      
    }

    void GraphicsEngine::DrawPlanets( float elapsed )
    {
      
      for( EntitySet::iterator itr = planet_components.begin(); itr != planet_components.end(); ++itr )
      {
        Engine::Entity * current_entity = entity_factory->GetEntity(*itr);

        Components::CPlanetGraphic::BeginRender();

        SF::Engine::ComponentList & comlist = current_entity->GetComponentsByType(Engine::ComponentType::PLANET);
        for( Engine::ComponentList::iterator com = comlist.begin(); com != comlist.end(); ++com )
        {
          ((Components::CPlanetGraphic*)(*com))->Update( elapsed );
          ((Components::CPlanetGraphic*)(*com))->Draw( m_frustum );
        }
        Components::CPlanetGraphic::EndRender();
      }
      
    }

    void GraphicsEngine::DrawSkybox( float /*elapsed*/ )
    {
      for( EntitySet::iterator itr = skybox_components.begin(); itr != skybox_components.end(); ++itr )
      {
        Engine::Entity * current_entity = entity_factory->GetEntity(*itr);

        SF::Engine::ComponentList & comlist = current_entity->GetComponentsByType(Engine::ComponentType::SKYBOX);
        for( Engine::ComponentList::iterator com = comlist.begin(); com != comlist.end(); ++com )
          ((Components::CSkybox*)(*com))->Draw();
      }
    }

    void GraphicsEngine::DrawString( int x, int y, const std::string & text )
    {
      TextEngine::Instance()->PrintText(float(x), float(y),text);
    }

    void GraphicsEngine::DrawSun(float /*elapsed*/ )
    {
      Components::CSunGraphic::BeginRender();
      for( EntitySet::iterator itr = sun_components.begin(); itr != sun_components.end(); ++itr )
      {
        Engine::Entity * current_entity = entity_factory->GetEntity(*itr);

        SF::Engine::ComponentList & comlist = current_entity->GetComponentsByType(Engine::ComponentType::SUN);
        for( Engine::ComponentList::iterator com = comlist.begin(); com != comlist.end(); ++com )
          ((Components::CSunGraphic*)(*com))->Draw( m_frustum );
      }
      Components::CSunGraphic::EndRender();
    }

    void GraphicsEngine::DrawUI()
    {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      for( EntitySet::iterator itr = ui_components.begin(); itr != ui_components.end(); ++itr )
      {
        Engine::Entity * current_entity = entity_factory->GetEntity(*itr);
        SF::Engine::ComponentList & comlist = current_entity->GetComponentsByType(Engine::ComponentType::UI_GRAPHIC);
        for( Engine::ComponentList::iterator com = comlist.begin(); com != comlist.end(); ++com )
          ((Components::CBaseInterfaceGraphic*)(*com))->Draw();
      }

      for( EntitySet::iterator itr = text_components.begin(); itr != text_components.end(); ++itr )
      {
        Engine::Entity * current_entity = entity_factory->GetEntity(*itr);
        SF::Engine::ComponentList & comlist = current_entity->GetComponentsByType(Engine::ComponentType::TEXT);
        for( Engine::ComponentList::iterator com = comlist.begin(); com != comlist.end(); ++com )
          ((Components::CText*)(*com))->Draw();
      }

      

      glPopAttrib();
    }

    void GraphicsEngine::DrawCursor( float /*elapsed*/ )
    {
      if( mShowCursor )
      {
        int X = Globals::input->MouseX();
        int Y = Globals::input->MouseY();

        const int size = 32;

        if( cursorTexture )
        {
          Enable2D();
          cursorTexture->Bind();
          glBegin( GL_QUADS );
          glTexCoord2f( 0, 0 );
          glVertex2i( X, Y ); 
          glTexCoord2f( 1, 0 );
          glVertex2i( X+size, Y ); 
          glTexCoord2f( 1, 1 );
          glVertex2i( X+size, Y+size ); 
          glTexCoord2f( 0, 1 );
          glVertex2i( X, Y+size ); 
          glEnd();
          cursorTexture->Unbind();
          Disable2D();
        }
      }
    }



    void GraphicsEngine::DrawWorldQuads( float /*elapsed*/ )
    {
      glEnable( GL_DEPTH_TEST );
      glEnable( GL_BLEND );
      glDepthMask(false);
      glBlendFunc( GL_SRC_ALPHA, GL_ONE );
      for( EntitySet::iterator itr = worldquad_components.begin(); itr != worldquad_components.end(); ++itr )
      {
        Engine::Entity * current_entity = entity_factory->GetEntity(*itr);

        SF::Engine::ComponentList & comlist = current_entity->GetComponentsByType(Engine::ComponentType::WORLD_QUAD);
        for( Engine::ComponentList::iterator com = comlist.begin(); com != comlist.end(); ++com )
          ((Components::CWorldQuad*)(*com))->Draw();
      }
    }

  }
}