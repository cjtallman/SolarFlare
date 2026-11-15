/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CInputRegion.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/13/2009 @ 13:02:42 - nwilliam
**  \brief   Text box UI component.
*******************************************************************************/

#ifndef SF_TEXT_INPUT_H
#define SF_TEXT_INPUT_H

#include <string>
#include "CRegion.h"
#include "Graphics/CText.h"
#include "Script/ScriptHelper.h"

namespace SF {
namespace UI
{
  class CInputRegion : public CRegion
  {
    public:
      CInputRegion();
      ~CInputRegion();

      virtual BaseComponent* Clone() const;
      virtual void PushToLua( lua_State *ls );

      virtual void Focus();
      virtual void Unfocus();
      virtual void OnKeypress();
      virtual void OnMouseOut();
      virtual void OnMouseOver();
      virtual void OnLeftClick();

      virtual bool InitDependencies( Engine::Entity* e );
      static Engine::BaseComponent* Creator( const ticpp::Element* element );

    private:
      void InsertCursor();
      void RemoveCursor();
      void DoCallback( const std::string& content );

      size_t m_maxLength;
      bool m_cursorActive;
      size_t m_cursorOffset;
      Script::LuaFunction m_keyPressCallback;
      Graphics::Components::CText* m_textComp;


    LUA_DECLARE_CLASS( CInputRegion )
    LUA_DECLARE_METHOD( LBase )
    LUA_DECLARE_METHOD( LRegion )
    LUA_DECLARE_METHOD( LContent )
    LUA_DECLARE_METHOD( LMaxLength )
    LUA_DECLARE_METHOD( LOnKeyPress )
    LUA_DECLARE_METHOD( LTextComponent )

    // From CRegion
    LUA_DECLARE_METHOD( LOrphan )
    LUA_DECLARE_METHOD( LOrphanTransform )
    LUA_DECLARE_METHOD( LSetColor )
    LUA_DECLARE_METHOD( LGetColor )
    LUA_DECLARE_METHOD( LSetAlpha )
    LUA_DECLARE_METHOD( LGetAlpha )
    LUA_DECLARE_METHOD( LSetAlignment )
    LUA_DECLARE_METHOD( LSetLayout )
    LUA_DECLARE_METHOD( LSetMargin )
    LUA_DECLARE_METHOD( LSetPadding )
    LUA_DECLARE_METHOD( LSetPosition )
    LUA_DECLARE_METHOD( LSetPositionMode )
  };

} // UI namespace
} // SF namespace

#endif // SF_TEXT_INPUT_H
