/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CText.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:28:19 CTallman
**  \brief   
*******************************************************************************/

#ifndef SF_CTEXT_H
#define SF_CTEXT_H

#include "TextEngine.h"
#include "Utilities/Color.h"
#include "Engine/BaseComponent.h"
#include "Math/CTransform.h"
#include "ShaderLoader.h"
#include "External/Lua/include/LunarHelper.h"
#include "Interface/CBasicElement.h"

namespace ticpp { class Element; }

namespace SF {
namespace Graphics {
namespace Components
{
  
  Engine::BaseComponent * CreateTextComponent( const ticpp::Element * element );

  class CText : public Engine::BaseComponent
  {
    enum HORIZONTAL { LEFT, CENTER, RIGHT };
    enum VERTICAL { TOP, MIDDLE, BOTTOM };
  public:
    CText(void);

    virtual Engine::BaseComponent * Clone() const;
    virtual bool InitDependencies( Engine::Entity * parent );
    static Engine::BaseComponent * CreateComponent( const ticpp::Element * element );

    virtual void Draw() const;
   
    void SetText( const std::vector<std::string> & text );
    void SetText( const std::string & _text );
    const std::string & GetText(int index = 0) const;

    float GetRegionWidth() const;
    float GetRegionHeight() const;

    void SetColor( const SF::Color4f & _color );
    void SetColor( const Color4f & top, const Color4f & bottom );
    const SF::Color4f & GetColor() const;

    void SetTransparency( float percent );
    float GetTransparency() const;

    void SetFont( const std::string & font_name );
    
    void SetFontSize( float size );
    float GetFontSize() const;

    void SetHorizontalAlignment( HORIZONTAL align );
    void SetVerticalAlignment( VERTICAL align );
    int GetHorizontalAlignment() const;
    int GetVerticalAlignment() const;

    void SetFragmentShader( const std::string & mappedName );
    void SetFragmentShader( SharedShader2 shader );

    virtual void PushToLua( lua_State * ls );

  private:
    void UpdateTextRegion();
    
    UI::CBasicElement * interface_element;
    std::vector<std::string> text;
    unsigned int font_id;
    Math::CTransform * transform;
    
    float font_height;

    float text_width;
    float text_height;

    Color4f text_grad_top;
    Color4f text_grad_bottom;

    HORIZONTAL horizontal_alignment;
    VERTICAL vertical_alignment;

    SharedShader2 frag;
    SharedShaderParameter cg_grad_top;
    SharedShaderParameter cg_grad_bottom;

    static std::string empty_string;

    LUA_DECLARE_CLASS( CText )
      LUA_DECLARE_METHOD( LGetRegionWidth )
      LUA_DECLARE_METHOD( LGetRegionHeight )
      LUA_DECLARE_METHOD( LGetFontSize )
      LUA_DECLARE_METHOD( LGetHorizontalAlignment )
      LUA_DECLARE_METHOD( LGetVerticalAlignment )

      LUA_DECLARE_METHOD( LSetText )
      LUA_DECLARE_METHOD( LSetTextColor )
      LUA_DECLARE_METHOD( LSetFont )
      LUA_DECLARE_METHOD( LSetTransparency )
      LUA_DECLARE_METHOD( LGetTransparency )
      LUA_DECLARE_METHOD( LSetFontSize )
      LUA_DECLARE_METHOD( LSetHorizontalAlignment )
      LUA_DECLARE_METHOD( LSetVerticalAlignment )
  };

}
}
}
#endif
