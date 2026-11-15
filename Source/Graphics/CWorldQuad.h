/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CWorldQuad.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 15:41:28 - nwilliam
**  \brief   Graphical component of quad in world space.
*******************************************************************************/

#ifndef SF_CTEXTURED_QUAD_H
#define SF_CTEXTURED_QUAD_H

#include "Texture2D.h"
#include "ShaderLoader.h"
#include "Math/TVector2.h"
#include "Math/CTransform.h"

#include "Engine/BaseComponent.h"

#include "Script/ScriptHelper.h"

namespace SF {
namespace Graphics {
namespace Components
{
  class CWorldQuad : Engine::BaseComponent
  {
    public:
      CWorldQuad();
      ~CWorldQuad();

      static Engine::BaseComponent * CreateComponent( const ticpp::Element * element );
      bool InitDependencies( Engine::Entity * parent );
      Engine::BaseComponent * Clone() const;

      void SetScale( float x_scale, float y_scale );
      void SetScale( float uniform_scale );
      void SetColor( Color _color );

      void SetVertexShader( const char * str );
      void SetFragmentShader( const char * str );
      void SetTexture( const std::string & _texture_name );

      void Draw();

      virtual void PushToLua( lua_State * ls );

    private:

      void BindTexture() const;
      void UnbindTexture() const;

      Math::CTransform * transform;
      Math::Vector2 scale;
      Color color;

      SharedTexture texture;
      SharedShader2 vert;
      SharedShader2 frag;



      LUA_DECLARE_CLASS( CWorldQuad )
        LUA_DECLARE_METHOD( LSetTexture )
        LUA_DECLARE_METHOD( LSetColor )
        LUA_DECLARE_METHOD( LSetTransparency )
        LUA_DECLARE_METHOD( LSetScale )
  };

} // Components namespace
} // Graphics namespace
} // SF namespace

#endif // SF_CTEXTURED_QUAD_H
