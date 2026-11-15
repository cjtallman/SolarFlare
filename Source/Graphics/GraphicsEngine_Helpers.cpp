/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    GraphicsEngine_Helpers.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/12/2009 @ 00:58 - CJT 
**  \brief   Some of the simple helper functions for the GraphicsEngine.
*******************************************************************************/

#include "GraphicsEngine.h"
#include "SolarFlare/Globals.h" 
#include "Engine/AssetManager.h"
#include "Math/Ray.h"

namespace SF
{
  namespace Graphics
  {

    void GraphicsEngine::AddLine( const Math::Vector3& start, const Math::Vector3& end )
    {
      m_lines.push_back( Math::LineSegment(start, end) );
    }

    Math::Ray GraphicsEngine::ScreenToWorld(int x, int y)
    {
      double ModelView[16];
      double Projection[16];
      int Viewport[4];

      glGetDoublev(GL_MODELVIEW_MATRIX, ModelView);
      glGetDoublev(GL_PROJECTION_MATRIX, Projection);
      glGetIntegerv(GL_VIEWPORT, Viewport);

      y = Viewport[3] - y;

      double wx, wy, wz;
      gluUnProject(x, y, 0, ModelView, Projection, Viewport, &wx, &wy, &wz);

      Math::Vector3 eye = Globals::graphicsEngine->CameraPosition();
      Math::Vector3 p = Math::Vector3( (float)wx, (float)wy, (float)wz);
      return Math::Ray( eye, p-eye);
    }

    Math::Vector3 GraphicsEngine::ScreenToWorldPoint(int x, int y)
    {
      double ModelView[16];
      double Projection[16];
      int Viewport[4];

      glGetDoublev(GL_MODELVIEW_MATRIX, ModelView);
      glGetDoublev(GL_PROJECTION_MATRIX, Projection);
      glGetIntegerv(GL_VIEWPORT, Viewport);

      y = Viewport[3] - y;

      double wx, wy, wz;
      gluUnProject(x, y, 0, ModelView, Projection, Viewport, &wx, &wy, &wz);

      return Math::Vector3( (float)wx, (float)wy, (float)wz);
    }

    Math::Vector3 GraphicsEngine::WorldToScreenPoint(const Math::Vector3& worldPoint)
    {
      double ModelView[16];
      double Projection[16];
      int Viewport[4];

      glGetDoublev(GL_MODELVIEW_MATRIX, ModelView);
      glGetDoublev(GL_PROJECTION_MATRIX, Projection);
      glGetIntegerv(GL_VIEWPORT, Viewport);

      double sx, sy, sz;
      gluProject(worldPoint.x, worldPoint.y, worldPoint.z, ModelView, Projection, Viewport, &sx, &sy, &sz);

      sy = Viewport[3] - sy;

      return Math::Vector3( (float)sx, (float)sy, (float)sz );
    }

    void GraphicsEngine::SetCursor( const std::string & mappedName )
    {

      static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
      SF::Engine::Assets::Texture * asset = (SF::Engine::Assets::Texture*)asset_manager->GetAsset( mappedName );
      if( asset )
        cursorTexture = asset->image;
      else
      {
        Globals::console->Warning("Invalid Texture: \"" + mappedName + "\" is not loaded.");
      }
    }

    void GraphicsEngine::ShowCursor(bool show)
    {
      mShowCursor = show;
    }

  }
}