/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CButton.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 17:47:13 - nwilliam
**  \brief   Basic graphical button.
*******************************************************************************/

#include "CButton.h"
#include "MenuSystem.h"
#include "SolarFlare/Globals.h"
#include "Engine/MessageEngine.h"
#include "External/ticpp/include/ticpp.h"
#include "External/Lua/include/LunarHelper.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace UI
{

CButton::CButton()
{
  m_onClick = LUA_NOREF;
}

CButton::~CButton()
{
  luaL_unref(m_onClickLuaState, LUA_REGISTRYINDEX, m_onClick);
}

Engine::BaseComponent* CButton::Clone() const
{
  CButton *button = new CButton;

  return button;
}

void CButton::PushToLua( lua_State *ls )
{
  Lunar<CButton>::push(ls, this);
}

void CButton::OnLeftClick()
{
  Globals::menuSystem->ProcessedClick(true);

  // Send onClick message
  Engine::Message *msg = Globals::messageEngine->AddMessage("onClick", m_entityID);
  msg->stack.Push( m_entityID );

  if( m_onClick != LUA_NOREF )
  {
    lua_rawgeti(m_onClickLuaState, LUA_REGISTRYINDEX, m_onClick);
    
    if( lua_pcall(m_onClickLuaState, 0, 0, 0) )
      HandleLuaError( m_onClickLuaState, "Button", m_onClick );
  }
}

void CButton::OnMouseOver()
{
  Globals::menuSystem->Hover(true);

  if( !m_mouseOver )
  {
    // Send onMouseOver message to entity
    Engine::Message *msg = Globals::messageEngine->AddMessage("onMouseOver", m_entityID);
    msg->stack.Push( m_entityID );

    m_mouseOver = true;
  }
}

void CButton::OnMouseOut()
{
  if( m_mouseOver )
  {
    // Send onMouseOut message to entity
    Engine::Message *msg = Globals::messageEngine->AddMessage("onMouseOut", m_entityID);
    msg->stack.Push( m_entityID );

    m_mouseOver = false;
  }
}

Engine::BaseComponent *CButton::Creator(const ticpp::Element *)
{
  CButton *button = new CButton;

  return button;
}

LUA_START_DEFINE_CLASS( CButton )
  LUA_NAME_METHOD( CButton, "Base", LBase )

  LUA_NAME_METHOD( CButton, "GetAlpha", LGetAlpha )
  LUA_NAME_METHOD( CButton, "GetColor", LGetColor )
  LUA_NAME_METHOD( CButton, "GetMargin", LGetMargin )

  LUA_NAME_METHOD( CButton, "OnClick", LOnClick )
  LUA_NAME_METHOD( CButton, "Orphan", LOrphan )

  LUA_NAME_METHOD( CButton, "SetAlignment", LSetAlignment )
  LUA_NAME_METHOD( CButton, "SetAlpha", LSetAlpha )
  LUA_NAME_METHOD( CButton, "SetColor", LSetColor )
  LUA_NAME_METHOD( CButton, "SetDimensions", LSetDimensions )
  LUA_NAME_METHOD( CButton, "SetMargin", LSetMargin )
  LUA_NAME_METHOD( CButton, "SetPosition", LSetPosition )

LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( CButton, LBase )
{
  CBasicElement::PushToLua(ls);
  return 1;
}

LUA_DEFINE_METHOD( CButton, LGetAlpha )
{
  return CBasicElement::LGetAlpha(ls);
}

LUA_DEFINE_METHOD( CButton, LGetColor )
{
  return CBasicElement::LGetColor(ls);
}

LUA_DEFINE_METHOD( CButton, LGetMargin )
{
  return CBasicElement::LGetMargin(ls);
}

LUA_DEFINE_METHOD( CButton, LOnClick )
{
  int stackSize = lua_gettop( ls );

  if( stackSize > 0 )
  {
    // set onClick
    if( lua_isfunction(ls, 1) )
    {
      m_onClickLuaState = ls;
      m_onClick = luaL_ref(ls, LUA_REGISTRYINDEX);
    }

    return 0;
  }
  else
  {
    lua_rawgeti(ls, LUA_REGISTRYINDEX, m_onClick);
    return 1;
  }
}

LUA_DEFINE_METHOD( CButton, LOrphan )
{
  return CBasicElement::LOrphan(ls);
}

LUA_DEFINE_METHOD( CButton, LSetColor )
{
  return CBasicElement::LSetColor(ls);
}

LUA_DEFINE_METHOD( CButton, LSetAlignment )
{
  std::string alignmentStr = luaL_checkstring( ls, 1 );

  if( alignmentStr == "RELATIVE" )
    alignment = Alignment::RELATIVE;
  else if( alignmentStr == "ABSOLUTE" )
    alignment = Alignment::ABSOLUTE;
  else
    alignment = Alignment::RELATIVE;

  return 0;
}

LUA_DEFINE_METHOD( CButton, LSetAlpha )
{
  return CBasicElement::LSetAlpha(ls);
}

LUA_DEFINE_METHOD( CButton, LSetDimensions )
{
  return CBasicElement::LSetDimensions(ls);
}

LUA_DEFINE_METHOD( CButton, LSetMargin )
{
  return CBasicElement::LSetMargin(ls);
}

LUA_DEFINE_METHOD( CButton, LSetPosition )
{
  return CBasicElement::LSetPosition(ls);
}


}

}
