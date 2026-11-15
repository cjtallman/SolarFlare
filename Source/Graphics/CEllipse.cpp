/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CEllipse.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:32:44 CTallman
**  \brief   Draws an oriented ellipse using a line loop.
*******************************************************************************/

#include "CEllipse.h"
#include "Engine/Entity.h"
#include "OpenGLInclude.h"
#include "Math/Constants.h"
#include "Math/CTransform.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {

      Engine::BaseComponent * CEllipse::CreateComponent( const ticpp::Element * element )
      {
        if( element )
        {
          CEllipse * out = new CEllipse();
          try
          {
            ticpp::Element * child = 0;

            child = element->FirstChildElement( "segments", false );
            if( child )
            {
              int segments = 100;
              child->GetAttributeOrDefault( "value", &segments, 100 );
              out->SetSegments(segments);
            }
          }
          catch( ticpp::Exception & )
          {
            delete out;
            return 0;
          }

          return out;
        }
        return 0;
      }

      Engine::BaseComponent * CEllipse::Clone() const
      {
        return new CEllipse();
      }

      bool CEllipse::InitDependencies( Engine::Entity * )
      {
        return true;
      }

      CEllipse::CEllipse() : Engine::BaseComponent( Engine::ComponentType::ELLIPSE )
      {
        segments = 200;
        major_axis = 5000;
        minor_axis = 5000;
        brightness = 1.0f;
        visible = false;
        RecalculateEllipsePoints();
      }

      void CEllipse::RecalculateEllipsePoints()
      {
        ellipse_points.resize(segments);
        float dt = Math::Constants::p_2_pi / segments;
        float t = -Math::Constants::pi;
        for( int i = 0 ; i < segments; ++i )
        {
          ellipse_points[i].x = major_axis * std::cos(t);
          ellipse_points[i].z = minor_axis * std::sin(t);
          t += dt;
        }
      }

      void CEllipse::Draw() const
      {
        //if( !visible )
        //  return;

        PushMatrix();
        float rotAngle;
        Math::Vector3 rotAxis;
        Math::Vector3 pos = center;
        
        orientation.AxisAngle(rotAxis, rotAngle);

        glTranslatef( pos.x, pos.y, pos.z );
        glRotatef(
          Math::Constants::RadToDeg * rotAngle,
          rotAxis.x,
          rotAxis.y,
          rotAxis.z);

        glColor4f(1, 1, 1, brightness * .25f);
        glBegin( GL_LINE_LOOP );
        for( int i = 0; i < segments; ++i )
          glVertex3f( ellipse_points[i].x, ellipse_points[i].y, ellipse_points[i].z );
        glEnd();

        PopMatrix();
      }

      // Networking
      void CEllipse::InitIn( Networking::ByteStream & bs )
      {
        bs >> center >> orientation >> major_axis >> minor_axis >> segments;
        RecalculateEllipsePoints();
      }

      void CEllipse::InitOut( Networking::ByteStream & bs ) const
      {
        bs << center << orientation << major_axis << minor_axis << segments;
      }

      void CEllipse::PushToLua( lua_State *ls )
      {
        Lunar<CEllipse>::push( ls, this );
      }

      LUA_START_DEFINE_CLASS( CEllipse )
        LUA_NAME_METHOD( CEllipse, "GetMajorAxis",    LGetMajorAxis )
        LUA_NAME_METHOD( CEllipse, "GetMinorAxis",    LGetMinorAxis )
        LUA_NAME_METHOD( CEllipse, "SetMajorAxis",    LSetMajorAxis )
        LUA_NAME_METHOD( CEllipse, "SetMinorAxis",    LSetMinorAxis )
        LUA_NAME_METHOD( CEllipse, "SetCenter",       LSetCenter )
        LUA_NAME_METHOD( CEllipse, "SetOrientation",  LSetOrientation )
        LUA_NAME_METHOD( CEllipse, "SetBrightness",   LSetBrightness )
        LUA_NAME_METHOD( CEllipse, "Visible", LVisible )

      LUA_END_DEFINE_CLASS

      LUA_DEFINE_METHOD( CEllipse, LGetMajorAxis )
      {
        lua_pushnumber( ls, GetMajorAxis() );
        return 1;
      }

      LUA_DEFINE_METHOD( CEllipse, LGetMinorAxis )
      {
        lua_pushnumber( ls, GetMinorAxis() );
        return 1;
      }

      LUA_DEFINE_METHOD( CEllipse, LSetMajorAxis )
      {
        float axis = luaL_checkfloat(ls, 1);
        SetMajorAxis( axis );
        return 0;
      }

      LUA_DEFINE_METHOD( CEllipse, LSetMinorAxis )
      {
        float axis = luaL_checkfloat(ls, 1);
        SetMinorAxis( axis );
        return 0;
      }

      LUA_DEFINE_METHOD( CEllipse, LSetCenter )
      {
        float x = luaL_checkfloat(ls, 3);
        float y = luaL_checkfloat(ls, 2);
        float z = luaL_checkfloat(ls, 1);

        SetCenter( Math::Vector3( x,y,z ) );

        return 0;
      }

      LUA_DEFINE_METHOD( CEllipse, LSetOrientation )
      {
        float yaw = luaL_checkfloat(ls, 1);
        float pitch = luaL_checkfloat(ls, 2);
        float roll = luaL_checkfloat(ls, 3);

        orientation = Math::Quaternion::EulerAngles(yaw, pitch, roll);

        return 0;
      }

      LUA_DEFINE_METHOD( CEllipse, LSetBrightness )
      {
        brightness = luaL_checkfloat(ls, 1);
        return 0;
      }

      LUA_DEFINE_METHOD( CEllipse, LVisible )
      {
        if( lua_gettop(ls) > 0)
        {
          if(lua_type(ls, 1) == LUA_TBOOLEAN)
            visible = (lua_toboolean(ls, 1) == 1);
          return 0;
        }
        else
        {
          lua_pushboolean( ls, visible );
          return 1;
        }
      }

    }
  }
}

