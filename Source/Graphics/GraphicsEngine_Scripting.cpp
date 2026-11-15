/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    GraphicsEngine_Scripting.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/12/2009 @ 00:57 - CJT 
**  \brief   These are the GraphicsEngine scripted functions.
*******************************************************************************/

#include "GraphicsEngine.h"
#include "StateManager.h"
#include "Math/Ray.h"

namespace SF
{
  namespace Graphics
  {


    LUA_START_DEFINE_CLASS( GraphicsEngine )
      LUA_NAME_METHOD( GraphicsEngine, "GetFPS", LGetFPS )
      LUA_NAME_METHOD( GraphicsEngine, "DrawLine", LDrawLine )
      LUA_NAME_METHOD( GraphicsEngine, "Wireframe", LWireframe )
      LUA_NAME_METHOD( GraphicsEngine, "PostProcess", LPostProcess )
      LUA_NAME_METHOD( GraphicsEngine, "GetCamera", LGetCamera )
      LUA_NAME_METHOD( GraphicsEngine, "GetScreenSize", LGetScreenSize )
      LUA_NAME_METHOD( GraphicsEngine, "ScreenToWorld", LScreenToWorld )
      LUA_NAME_METHOD( GraphicsEngine, "ProjectVector", LProjectVector )
      LUA_NAME_METHOD( GraphicsEngine, "UnprojectVector", LUnprojectVector )
      LUA_NAME_METHOD( GraphicsEngine, "SetAspectRatio", LSetAspectRatio )
      LUA_NAME_METHOD( GraphicsEngine, "GetAspectRatio", LGetAspectRatio )
      LUA_NAME_METHOD( GraphicsEngine, "SetViewingAngle", LSetViewingAngle )
      LUA_NAME_METHOD( GraphicsEngine, "GetViewingAngle", LGetViewingAngle )
      LUA_NAME_METHOD( GraphicsEngine, "SetGraphicsQuality", LSetGraphicsQuality )
      LUA_NAME_METHOD( GraphicsEngine, "GetGraphicsQuality", LGetGraphicsQuality )
      LUA_NAME_METHOD( GraphicsEngine, "SetVSync", LSetVSync )
      LUA_NAME_METHOD( GraphicsEngine, "GetVSync", LGetVSync )
      LUA_NAME_METHOD( GraphicsEngine, "SetLightAmbient", LSetLightAmbient )
      LUA_NAME_METHOD( GraphicsEngine, "SetLightDiffuse", LSetLightDiffuse )
      LUA_NAME_METHOD( GraphicsEngine, "SetLightSpecular", LSetLightSpecular )
      LUA_NAME_METHOD( GraphicsEngine, "SetLightPercent", LSetLightPercent )
      LUA_NAME_METHOD( GraphicsEngine, "ResetLights", LResetLights )
      LUA_NAME_METHOD( GraphicsEngine, "ShakeCamera", LShakeCamera )
      LUA_NAME_METHOD( GraphicsEngine, "Action", LAction )
      LUA_NAME_METHOD( GraphicsEngine, "GetActionRegion", LGetActionRegion )
      LUA_NAME_METHOD( GraphicsEngine, "ClearActions", LClearActions )
      LUA_NAME_METHOD( GraphicsEngine, "SetCursor", LSetCursor )
      LUA_NAME_METHOD( GraphicsEngine, "ShowCursor", LShowCursor )
      LUA_END_DEFINE_CLASS

    LUA_DEFINE_METHOD( GraphicsEngine, LGetFPS )
    {
      lua_pushinteger( ls, m_fps );
      return 1;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LDrawLine )
    {
      Math::Vector3 start( luaL_checkfloat(ls, 1), luaL_checkfloat(ls, 2), luaL_checkfloat(ls, 3) );
      Math::Vector3 end( luaL_checkfloat(ls, 4), luaL_checkfloat(ls, 5), luaL_checkfloat(ls, 6) );

      AddLine(start, end);

      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LWireframe )
    {
      int top = lua_gettop(ls);
      if( top == 0 )
      {
        lua_pushboolean(ls, renderer.Wireframe() );
        return 1;
      }
      else
      {
        bool enable = lua_toboolean(ls, 1) != 0;
        renderer.Wireframe(enable);
        return 0;
      }
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LPostProcess )
    {
      int top = lua_gettop(ls);
      if( top == 0 )
      {
        lua_pushboolean(ls, RenderPostProcess() );
        return 1;
      }
      else
      {
        bool enable = lua_toboolean(ls, 1) != 0;
        RenderPostProcess(enable);
        return 0;
      }
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LGetCamera )
    {
      Lunar<Graphics::QuatCamera>::push( ls, &m_camera );
      return 1;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LGetScreenSize )
    {
      lua_pushinteger( ls, renderer.WindowWidth() );
      lua_pushinteger( ls, renderer.WindowHeight() );

      return 2;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LScreenToWorld )
    {
      int sx = luaL_checkint(ls, 1);
      int sy = luaL_checkint(ls, 2);

      Math::Ray ray = GraphicsEngine::ScreenToWorld(sx, sy);

      lua_pushnumber(ls, ray.start.x);
      lua_pushnumber(ls, ray.start.y);
      lua_pushnumber(ls, ray.start.z);

      lua_pushnumber(ls, ray.direction.x);
      lua_pushnumber(ls, ray.direction.y);
      lua_pushnumber(ls, ray.direction.z);

      return 6;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LProjectVector )
    {
      float x = luaL_checkfloat(ls, 1);
      float y = luaL_checkfloat(ls, 2);
      float z = luaL_checkfloat(ls, 3);

      Math::Vector3 s = GraphicsEngine::WorldToScreenPoint( Math::Vector3(x, y, z) );

      lua_pushnumber(ls, s.x);
      lua_pushnumber(ls, s.y);
      lua_pushnumber(ls, s.z);

      return 3;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LUnprojectVector )
    {
      int dx = luaL_checkint(ls, 1);
      int dy = luaL_checkint(ls, 2);

      Math::Vector3 v = GraphicsEngine::ScreenToWorldPoint(dx,dy);

      lua_pushnumber(ls, v.x);
      lua_pushnumber(ls, v.y);
      lua_pushnumber(ls, v.z);

      return 3;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LSetAspectRatio )
    {
      double r = luaL_checknumber( ls, 1 );
      renderer.AspectRatio( r );
      return 0;
    }
    LUA_DEFINE_METHOD( GraphicsEngine, LGetAspectRatio )
    {
      lua_pushnumber( ls, renderer.AspectRatio() );
      return 1;
    }
    LUA_DEFINE_METHOD( GraphicsEngine, LSetViewingAngle )
    {
      double r = luaL_checknumber( ls, 1 );
      renderer.FOV( r );
      return 0;
    }
    LUA_DEFINE_METHOD( GraphicsEngine, LGetViewingAngle )
    {
      lua_pushnumber( ls, renderer.FOV() );
      return 1;
    }
    LUA_DEFINE_METHOD( GraphicsEngine, LSetGraphicsQuality )
    {
      int quality = luaL_checkint( ls, 1 );
      if( quality < QUALITY::LAST && quality >= 0 )
        Performance::StateManager::SetGraphicsQuality( quality );
      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LGetGraphicsQuality )
    {
      int quality = (int)Performance::StateManager::GetGraphicsQuality();
      lua_pushinteger( ls, quality );
      return 1;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LSetVSync )
    {
      bool enable = lua_toboolean(ls, 1) != 0;
      renderer.VSync(enable);
      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LGetVSync )
    {
      lua_pushboolean( ls, renderer.VSync() );
      return 1;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LSetLightAmbient )
    {
      int r = 0;
      int g = 0;
      int b = 0;
      int a = 255;
      const float den = 1.0f/255.0f;

      int top = lua_gettop(ls);
      switch( top )
      {
      case 4:
        a = luaL_checkint(ls, 4);
      case 3:
        b = luaL_checkint(ls, 3);
      case 2:
        g = luaL_checkint(ls, 2);
      case 1:
        r = luaL_checkint(ls, 1);
      default:
        renderer.SetLightAmbient( Color4f(r*den,g*den,b*den,a*den) );
      }

      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LSetLightDiffuse )
    { 
      int r = 0;
      int g = 0;
      int b = 0;
      int a = 255;
      const float den = 1.0f/255.0f;

      int top = lua_gettop(ls);
      switch( top )
      {
      case 4:
        a = luaL_checkint(ls, 4);
      case 3:
        b = luaL_checkint(ls, 3);
      case 2:
        g = luaL_checkint(ls, 2);
      case 1:
        r = luaL_checkint(ls, 1);
      default:
        renderer.SetLightDiffuse( Color4f(r*den,g*den,b*den,a*den) );
      }

      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LSetLightSpecular )
    {
      int r = 0;
      int g = 0;
      int b = 0;
      int a = 255;
      const float den = 1.0f/255.0f;

      int top = lua_gettop(ls);
      switch( top )
      {
      case 4:
        a = luaL_checkint(ls, 4);
      case 3:
        b = luaL_checkint(ls, 3);
      case 2:
        g = luaL_checkint(ls, 2);
      case 1:
        r = luaL_checkint(ls, 1);
      default:
        renderer.SetLightSpecular( Color4f(r*den,g*den,b*den,a*den) );
      }

      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LSetLightPercent )
    {
      float percent = luaL_checkfloat(ls, 1);
      renderer.SetLightPercent(percent);
      return 0;
    }

    LUA_DEFINE_METHOD_NOLS( GraphicsEngine, LResetLights )
    {
      ResetLights();
      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LShakeCamera )
    {
      float intensity = luaL_checkfloat(ls, 1);
      float time = luaL_checkfloat(ls, 2);

      camera_shake_intensity = intensity;
      camera_shake_current_time = time;
      camera_shake_total_time = time;

      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LAction )
    {
      const char * text1 = 0;
      const char * text2 = 0;
      int action = luaL_checkint(ls, 1);

      text1 = luaL_checkstring(ls, 2);
      int r1 = luaL_checkint(ls, 3);
      int g1 = luaL_checkint(ls, 4);
      int b1 = luaL_checkint(ls, 5);

      text2 = luaL_checkstring(ls, 6);
      int r2 = luaL_checkint(ls, 7);
      int g2 = luaL_checkint(ls, 8);
      int b2 = luaL_checkint(ls, 9);
      
      actionRegion.AddAction(text1, Color4f(1,1,1,1), Color4i(r1,g1,b1,1), text2, Color4f(1,1,1,1), Color4i(r2,g2,b2,1), action);
      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LGetActionRegion )
    {
      Lunar<CActionRegion>::push( ls, &actionRegion );
      return 1;
    }

    LUA_DEFINE_METHOD_NOLS( GraphicsEngine, LClearActions )
    {
      actionRegion.Clear();
      return 0;
    }


    LUA_DEFINE_METHOD( GraphicsEngine, LSetCursor )
    {
      const char * mappedName = luaL_checkstring(ls, 1);

      if( mappedName )
      {
        SetCursor(mappedName);
      }

      return 0;
    }

    LUA_DEFINE_METHOD( GraphicsEngine, LShowCursor )
    {
      bool enable = lua_toboolean(ls, 1) != 0;
      ShowCursor(enable);
      return 0;
    }



  }
}