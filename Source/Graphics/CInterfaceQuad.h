/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CInterfaceQuad.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/05/2009 @ 17:02:08 - nwilliam
**  \brief   
*******************************************************************************/

#ifndef _SF_CINTERFACE_QUAD_H_
#define _SF_CINTERFACE_QUAD_H_

#include "Engine/BaseComponent.h"
#include "Math/TVector3.h"
#include "Utilities/Color.h"
#include "Texture2D.h"
#include "CBaseInterfaceGraphic.h"
#include "Math/CTransform.h"
#include "TextEngine.h"
#include "ShaderLoader.h"

using namespace SF::Math;

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {
      class CInterfaceQuad : public CBaseInterfaceGraphic
      {
      public:
        static Engine::BaseComponent * CreateComponent( const ticpp::Element * element );
        virtual Engine::BaseComponent* Clone() const;
        virtual bool InitDependencies( Engine::Entity* );

        CInterfaceQuad();
        ~CInterfaceQuad();

        std::string& GetText();
        const std::string& GetText() const;
        
        void SetWidth( float w );
        void SetHeight( float h );
        void SetColor( const Color4f & c );
        void SetTransparency( float percent );
        float GetWidth() const;
        float GetHeight() const;
        Color GetColor() const;
        float GetTransparency() const;
        
        void SetText( const std::string & text );
        void SetTextColor( const Color4f & color );
        void SetTextColor( const Color4f & top, const Color4f & bottom );
        void SetTextSize( float text_size );
        void SetFont( const std::string & font_name );

        void SetTexture( int state, SharedTexture _texture );
        void SetTexture( int state, const std::string & _texture_name );
        void SetFragmentShader( const std::string & mappedName );
        void SetFragmentShader( SharedShader2 shader );

        void Draw();
        
        virtual void PushToLua( lua_State *ls );

      private:
        bool BindTexture();
        void UnbindTexture();
        void UpdateTextWidth();

        float width;
        float height;
        Color4f color;

        std::string text;
        SF::Color4f text_color;
        float text_height;
        float text_width;
        unsigned int text_font;

        SharedShader2 frag;
        SharedShaderParameter cg_grad_top;
        SharedShaderParameter cg_grad_bottom;
        Color4f text_grad_top;
        Color4f text_grad_bottom;

        std::vector<SharedTexture> texture;

        NO_COPY( CInterfaceQuad );

        LUA_DECLARE_CLASS( CInterfaceQuad )
          LUA_DECLARE_METHOD( LSetTexture )

          LUA_DECLARE_METHOD( LSetWidth )
          LUA_DECLARE_METHOD( LSetHeight )
          LUA_DECLARE_METHOD( LSetColor )
          LUA_DECLARE_METHOD( LSetTransparency )
          LUA_DECLARE_METHOD( LGetWidth )
          LUA_DECLARE_METHOD( LGetHeight )
          LUA_DECLARE_METHOD( LGetColor )
          LUA_DECLARE_METHOD( LGetTransparency )

          LUA_DECLARE_METHOD( LSetText )
          LUA_DECLARE_METHOD( LSetTextSize )
          LUA_DECLARE_METHOD( LSetTextColor )
          LUA_DECLARE_METHOD( LSetFont )

          LUA_DECLARE_METHOD( LGetZOrder )
          LUA_DECLARE_METHOD( LSetZOrder )

          LUA_DECLARE_METHOD( LGetState )
          LUA_DECLARE_METHOD( LSetState )

          LUA_DECLARE_METHOD( LBase )
      };

    }
  }
}

#endif
