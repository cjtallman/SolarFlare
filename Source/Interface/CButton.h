/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CButton.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 09/16/2008 @ 23:14:07 - nwilliam
**  \brief   Basic graphical button.
*******************************************************************************/

#ifndef SF_BUTTON_H
#define SF_BUTTON_H

#include <string>

#include "CBasicElement.h"

#include "SolarFlare/Globals.h"
#include "Math/TVector2.h"
#include "Math/TVector3.h"
#include "Math/TVector2.h"
#include "Math/TVector3.h"
#include "Math/Quad.h"

#include "External/Lua/include/LunarHelper.h"

namespace SF {
namespace UI
{

class CButton : public CBasicElement
{
public:
  CButton();
  ~CButton();

  virtual BaseComponent* Clone() const;
  virtual void PushToLua( lua_State *ls );

  virtual void OnLeftClick();
  virtual void OnMouseOver();
  virtual void OnMouseOut();

  static Engine::BaseComponent *Creator(const ticpp::Element *element);

private:
  lua_State *       m_onClickLuaState;
  int               m_onClick;

  LUA_DECLARE_CLASS( CButton )
  LUA_DECLARE_METHOD( LBase )

  LUA_DECLARE_METHOD( LGetAlpha )
  LUA_DECLARE_METHOD( LGetColor )
  LUA_DECLARE_METHOD( LGetMargin )

  LUA_DECLARE_METHOD( LOnClick )
  LUA_DECLARE_METHOD( LOrphan )

  LUA_DECLARE_METHOD( LSetAlignment )
  LUA_DECLARE_METHOD( LSetAlpha )
  LUA_DECLARE_METHOD( LSetColor )
  LUA_DECLARE_METHOD( LSetDimensions )
  LUA_DECLARE_METHOD( LSetMargin )
  LUA_DECLARE_METHOD( LSetPosition )
};


}

}

#endif
