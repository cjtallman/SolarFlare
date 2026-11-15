/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CToolTip.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 03/13/2009 @ 12:01 - CJT 
**  \brief 
*******************************************************************************/

#ifndef SF_CTOOLTIP_H
#define SF_CTOOLTIP_H

#include "Engine/BaseComponent.h"
#include "CBaseInterfaceGraphic.h"
#include "Interface/CBasicElement.h"

#include "SolarFlare/Globals.h"
#include "ShaderLoader.h"
#include "Math/CTransform.h"
#include "Utilities/Color.h"
#include "Texture2D.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {
      namespace ANCHOR
      {
        enum ANCHORPOS { TOP_LEFT, TOP, TOP_RIGHT, RIGHT, BOTTOM_RIGHT, BOTTOM, BOTTOM_LEFT, LEFT, CENTER };
      }

      namespace ALIGNMENT
      {
        enum HORIZONTAL { LEFT, CENTER, RIGHT };
        enum VERTICAL { TOP, MIDDLE, BOTTOM };
      }

      class CToolTip : public CBaseInterfaceGraphic
      {
      public:
        static Engine::BaseComponent * CreateComponent( const ticpp::Element * element );
        virtual Engine::BaseComponent* Clone() const;
        virtual bool InitDependencies( Engine::Entity* );

        CToolTip();
        ~CToolTip(){}

        virtual void Draw();

        void SetTexture( SharedTexture _texture );
        void SetTexture( const std::string & mappedName );

        void SetText( const std::vector<std::string> & text );
        void SetText( const std::string & _text );
        const std::string & GetText(int index = 0) const;

        float GetRegionWidth() const;
        float GetRegionHeight() const;

        void SetTextColor( const Color4f & _color );
        void SetTextColor( const Color4f & top, const Color4f & bottom );
        const Color4f & GetTextColor() const;

        void SetColor( const Color4f & _color );
        const Color4f & GetColor() const;

        void SetTransparency( float percent );
        float GetTransparency() const;

        void SetPadding(float top, float right, float bottom, float left);

        void SetFont( const std::string & font_name );

        void SetFontSize( float size );
        float GetFontSize() const;

        void SetHorizontalAlignment( ALIGNMENT::HORIZONTAL align );
        void SetVerticalAlignment( ALIGNMENT::VERTICAL align );
        int GetHorizontalAlignment() const;
        int GetVerticalAlignment() const;

        void SetFragmentShader( const std::string & mappedName );
        void SetFragmentShader( SharedShader2 shader );

        virtual void PushToLua( lua_State *ls );

      private:

        void DrawPanel() const;
        void DrawText() const;

        void UpdateTextRegion();

        SharedTexture panel_texture;

        UI::CBasicElement * interface_element;
        std::vector<std::string> text;
        unsigned int font_id;
        Math::CTransform * transform;

        float font_height;

        float text_width;
        float text_height;

        struct
        {
          float top;
          float right;
          float bottom;
          float left;
        }padding;

        Color4f text_grad_top;
        Color4f text_grad_bottom;
        Color4f panel_color;

        ALIGNMENT::HORIZONTAL horizontal_alignment;
        ALIGNMENT::VERTICAL vertical_alignment;
        ANCHOR::ANCHORPOS anchor;


        SharedShader2 frag;
        SharedShaderParameter cg_grad_top;
        SharedShaderParameter cg_grad_bottom;

        static std::string empty_string;

        LUA_DECLARE_CLASS( CToolTip )
          LUA_DECLARE_METHOD( LGetRegionWidth )
          LUA_DECLARE_METHOD( LGetRegionHeight )
          LUA_DECLARE_METHOD( LGetFontSize )
          LUA_DECLARE_METHOD( LGetHorizontalAlignment )
          LUA_DECLARE_METHOD( LGetVerticalAlignment )
          LUA_DECLARE_METHOD( LGetTransparency )

          LUA_DECLARE_METHOD( LSetColor )
          LUA_DECLARE_METHOD( LSetText )
          LUA_DECLARE_METHOD( LSetTextColor )
          LUA_DECLARE_METHOD( LSetFont )
          LUA_DECLARE_METHOD( LSetFontSize )
          LUA_DECLARE_METHOD( LSetHorizontalAlignment )
          LUA_DECLARE_METHOD( LSetVerticalAlignment )
          LUA_DECLARE_METHOD( LSetTransparency )
          LUA_DECLARE_METHOD( LSetPadding )
          LUA_DECLARE_METHOD( LSetAnchor )


      };

    }
  }
}

#endif

