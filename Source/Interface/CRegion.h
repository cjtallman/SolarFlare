/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CRegion.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 09/16/2008 @ 23:14:07 - nwilliam
**  \brief   Graphical region, holds other elements.
*******************************************************************************/

#ifndef SF_MENU_H
#define SF_MENU_H

#include "CBasicElement.h"

namespace SF {
namespace UI
{


class CRegion : public CBasicElement
{
public:

  CRegion();
  ~CRegion();

  void SetAlign(HAlign::ID hAlign, VAlign::ID vAlign);
  void SetLayout(Layout::ID layout);

  virtual void OnLeftClick();
  virtual void OnLeftClickHold();
  virtual void OnRightClick();
  virtual void OnRightClickHold();
  virtual void OnKeypress();
  virtual void OnMouseOver();
  virtual void OnMouseOut();
  virtual void OnScroll();

  virtual void AutoResize();
  virtual void AutoResizeChildren();
  virtual void Format();
  virtual void FormatChildren();

  virtual BaseComponent* Clone() const;
  virtual void PushToLua( lua_State *ls );

  static Engine::BaseComponent *Creator(const ticpp::Element *element);

private:
  HAlign::ID  m_hAlign;
  VAlign::ID  m_vAlign;

  Layout::ID  m_layout;

  LUA_DECLARE_CLASS( CRegion )
  LUA_DECLARE_METHOD( LBase )

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


}

}

#endif
