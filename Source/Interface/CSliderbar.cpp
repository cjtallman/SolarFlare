/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CSliderbar.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/16/2008 @ 07:07:50 - pmera
**  \brief   
*******************************************************************************/

#include "CSliderbar.h"
#include "MenuSystem.h"
#include "Utilities/Misc.h"
#include "Engine/InputEngine.h"
#include "Script/ScriptHelper.h"
#include "Engine/MessageEngine.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace UI
{

CSliderbar::CSliderbar()
{
  m_onClick = LUA_NOREF;

  m_rangeLower = 0.0f;
  m_rangeUpper = 1.0f;
  m_value = 0.0f;
}  

CSliderbar::~CSliderbar()
{
  luaL_unref(m_onClickLuaState, LUA_REGISTRYINDEX, m_onClick);
}

Engine::BaseComponent* CSliderbar::Clone() const
{
  CSliderbar *button = new CSliderbar;

  return button;
}

void CSliderbar::PushToLua( lua_State *ls )
{
  Lunar<CSliderbar>::push(ls, this);
}

void CSliderbar::OnLeftClickHold()
{
  Focus();

  Globals::menuSystem->ProcessedClick(true);

  float x = (float)Globals::input->MouseX();

  float dx = Utilities::Clamp( x - m_transform->GetWorldPosition().x, -halfWidth, halfWidth );

  x = (dx + halfWidth) / (halfWidth * 2);
  m_value = m_rangeLower + (m_rangeUpper - m_rangeLower) * x;

  m_sliderTransform->localPosition = Math::Vector3(dx, 0, 0);

  if( m_onClick != LUA_NOREF )
  {
    lua_rawgeti(m_onClickLuaState, LUA_REGISTRYINDEX, m_onClick);
    lua_pushnumber(m_onClickLuaState, m_value);
    
    if( lua_pcall(m_onClickLuaState, 1, 0, 0) )
      HandleLuaError( m_onClickLuaState, "Sliderbar", m_onClick );
  }
}

void CSliderbar::OnLeftMouseUp()
{
  Unfocus();
}

void CSliderbar::OnMouseOver()
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

void CSliderbar::OnMouseOut()
{
  if( m_mouseOver )
  {
    // Send onMouseOut message to entity
    Engine::Message *msg = Globals::messageEngine->AddMessage("onMouseOut", m_entityID);
    msg->stack.Push( m_entityID );

    m_mouseOver = false;
  }
}

Engine::BaseComponent *CSliderbar::Creator(const ticpp::Element *)
{
  CSliderbar *button = new CSliderbar;

  return button;
}

bool CSliderbar::InitDependencies( Engine::Entity *entity )
{
  m_entityID = entity->ID();

  m_transform = static_cast<Math::CTransform *>(
    entity->GetComponentByName( Engine::ComponentType::TRANSFORM, "barTransform" ));

  m_sliderTransform = static_cast<Math::CTransform *>(
    entity->GetComponentByName( Engine::ComponentType::TRANSFORM, "sliderTransform" ));

  if( !m_transform || !m_sliderTransform )
    return false;

  m_sliderTransform->SetParent( m_transform );

  return true;
}

LUA_START_DEFINE_CLASS( CSliderbar )
  LUA_NAME_METHOD( CSliderbar, "Base", LBase )

  LUA_NAME_METHOD( CSliderbar, "EntityID", LEntityID )

  LUA_NAME_METHOD( CSliderbar, "GetAlpha", LGetAlpha )
  LUA_NAME_METHOD( CSliderbar, "GetColor", LGetColor )
  LUA_NAME_METHOD( CSliderbar, "GetMargin", LGetMargin )

  LUA_NAME_METHOD( CSliderbar, "OnClick", LOnClick )
  LUA_NAME_METHOD( CSliderbar, "Orphan", LOrphan )

  LUA_NAME_METHOD( CSliderbar, "SetAlpha", LSetAlpha )
  LUA_NAME_METHOD( CSliderbar, "SetAlignment", LSetAlignment )
  LUA_NAME_METHOD( CSliderbar, "SetColor", LSetColor )
  LUA_NAME_METHOD( CSliderbar, "SetDimensions", LSetDimensions )
  LUA_NAME_METHOD( CSliderbar, "SetMargin", LSetMargin )
  LUA_NAME_METHOD( CSliderbar, "SetPosition", LSetPosition )
  LUA_NAME_METHOD( CSliderbar, "SetRange", LSetRange )
  LUA_NAME_METHOD( CSliderbar, "SetValue", LSetValue )

LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( CSliderbar, LBase )
{
  CBasicElement::PushToLua(ls);

  return 1;
}

LUA_DEFINE_METHOD( CSliderbar, LEntityID )
{
  lua_pushinteger(ls, m_entityID);

  return 1;
}

LUA_DEFINE_METHOD( CSliderbar, LOnClick )
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


LUA_DEFINE_METHOD( CSliderbar, LOrphan )
{
  return CBasicElement::LOrphan(ls);
}

LUA_DEFINE_METHOD( CSliderbar, LGetAlpha )
{
  return CBasicElement::LGetAlpha(ls);
}

LUA_DEFINE_METHOD( CSliderbar, LGetColor )
{
  return CBasicElement::LGetColor(ls);
}

LUA_DEFINE_METHOD( CSliderbar, LGetMargin )
{
  return CBasicElement::LGetMargin(ls);
}

LUA_DEFINE_METHOD( CSliderbar, LSetAlignment )
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

LUA_DEFINE_METHOD( CSliderbar, LSetAlpha )
{
  return CBasicElement::LSetAlpha(ls);
}

LUA_DEFINE_METHOD( CSliderbar, LSetColor )
{
  return CBasicElement::LSetColor(ls);
}

LUA_DEFINE_METHOD( CSliderbar, LSetDimensions )
{
  float width = luaL_checkfloat(ls, 1);
  float height = luaL_checkfloat(ls, 2);

  halfWidth = width * 0.5f;
  halfHeight = height * 0.5f;

  return 0;
}

LUA_DEFINE_METHOD( CSliderbar, LSetMargin )
{
  return CBasicElement::LSetMargin(ls);
}

LUA_DEFINE_METHOD( CSliderbar, LSetPosition )
{
  return CBasicElement::LSetPosition(ls);
}

LUA_DEFINE_METHOD( CSliderbar, LSetRange )
{
  m_rangeLower = luaL_checkfloat(ls, 1);
  m_rangeUpper = luaL_checkfloat(ls, 2);

  return 0;
}

LUA_DEFINE_METHOD( CSliderbar, LSetValue )
{
  m_value = luaL_checkfloat(ls, 1);

  m_sliderTransform->localPosition = Math::Vector3(
    ((m_value - m_rangeLower) / (m_rangeUpper - m_rangeLower) - 0.5f) * halfWidth * 2, 0, 0);

  return 0;
}

}

}
