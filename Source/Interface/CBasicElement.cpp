/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CBasicElement.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 12/02/2008 @ 18:10:17 - nwilliam
**  \brief   Implementation of common UI functionality.
*******************************************************************************/

#include "CBasicElement.h"

#include <algorithm>

#include "MenuSystem.h"
#include "Engine/Entity.h"
#include "Math/TVector3.h"
#include "Math/TVector2.h"
#include "Utilities/Misc.h"
#include "Math/CTransform.h"
#include "SolarFlare/Globals.h"
#include "Engine/InputEngine.h"
#include "Script/ScriptEngine.h"
#include "Utilities/DevConsole.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace UI
{

Engine::BaseComponent *CBasicElement::Clone() const
{
  return new CBasicElement( *this );
}

void CBasicElement::PushToLua( lua_State *ls )
{
  Lunar<CBasicElement>::push(ls, this);
}

Engine::BaseComponent *CBasicElement::Creator(const ticpp::Element *)
{
  CBasicElement *element = new CBasicElement;

  return element;
}

CBasicElement::CBasicElement()
: Engine::BaseComponent( Engine::ComponentType::UI_ELEMENT )
{
  m_parent = 0;

  m_focus = false;
  m_mouseOver = false;

  m_transform = 0;

  m_transformTime = 1.0f;
  m_transformTimeScale = 1.0f;

  m_colorBegin = m_colorEnd = m_color = Color4f(1.0f,1.0f,1.0f,1.0f);
  m_colorTime = 0.0f;
  m_colorTimeScale = 1.0f;

  marginTop = 0.0f;
  marginRight = 0.0f;
  marginBottom = 0.0f;
  marginLeft = 0.0f;

  paddingTop = 0.0f;
  paddingRight = 0.0f;
  paddingBottom = 0.0f;
  paddingLeft = 0.0f;

  halfHeight = 0;
  halfWidth = 0;

  hPosition = HAlign::LEFT;
  vPosition = VAlign::TOP;

  alignment = Alignment::RELATIVE;

  m_hCollapse = HAlign::CENTER;
  m_vCollapse = VAlign::MIDDLE;

}

CBasicElement::~CBasicElement()
{
  Unfocus();

  if(m_parent)
    m_parent->RemoveChild(this);

  ElementList::iterator iter;

  while( !m_children.empty() )
  {
    m_children.back()->m_parent = 0;
    m_children.pop_back();
  }
}

void CBasicElement::AddChild(CBasicElement *element)
{
  element->m_parent = this;
  m_children.push_back(element);

  element->m_transform->SetParent( m_transform );
}

void CBasicElement::Orphan()
{
  if(m_parent)
  {
    m_parent->RemoveChild(this);
    
    m_parent = 0;
  }
}

void CBasicElement::PromoteChildren()
{
  if(m_parent)
  {
    for(unsigned int i = 0; i < m_children.size(); ++i)
      m_parent->AddChild(m_children[i]);
  }
  else
  {
    for(unsigned int i = 0; i < m_children.size(); ++i)
      m_children[i]->m_parent = 0;;
  }

  m_children.clear();
}

void CBasicElement::RemoveChild(CBasicElement *element)
{
  m_children.erase( std::remove(m_children.begin(), m_children.end(), element), m_children.end() );

  if( element->m_transform )
    element->m_transform->Orphan();
}

void CBasicElement::RemoveChildren()
{
  ElementList::iterator iter;

  for(iter = m_children.begin(); iter != m_children.end(); ++iter)
    (*iter)->m_parent = 0;

  m_children.clear();
}

void CBasicElement::Focus()
{
  Globals::menuSystem->Focus(this);

  m_focus = true;
}

void CBasicElement::Unfocus()
{
  Globals::menuSystem->Unfocus();

  m_focus = false;
}

Color4f CBasicElement::GetColor() const
{
  return m_color;
}

float CBasicElement::GetWidth() const
{
  return halfWidth * 2;
}

float CBasicElement::GetHeight() const
{
  return halfHeight * 2;
}

void CBasicElement::SetColorMod(const Color4f & color)
{
  m_colorMod = color;
}

void CBasicElement::SetOpacity(float opacity)
{
  m_colorMod.a = (unsigned char)(opacity * 255);
}

void CBasicElement::SetMargin(float top, float right, float bottom, float left)
{
  marginTop = top;
  marginRight = right;
  marginBottom = bottom;
  marginLeft = left;
}

void CBasicElement::SetAlpha( float alpha, float time )
{
  m_colorEnd.a = alpha;

  SetColorTime( time );

  for(unsigned int i = 0; i < m_children.size(); ++i)
    m_children[i]->SetAlpha( alpha, time );
}

void CBasicElement::SetColor( const Color4f & color, float time )
{
  m_colorEnd = color;

  SetColorTime( time );

  for(unsigned int i = 0; i < m_children.size(); ++i)
    m_children[i]->SetColor( color, time );
}

void CBasicElement::SetColorTime( float time )
{
  m_colorBegin = m_color;

  if( time > 0.001f )
  {
    m_colorTime = 0.0f;
    m_colorTimeScale = 1.0f / time;
  }
  else
  {
    m_color = m_colorEnd;
    m_colorTime = 1.0f;
    m_colorTimeScale = 1.0f;
  }
}

void CBasicElement::SetPadding( float top, float right, float bottom, float left )
{
  paddingTop = top;
  paddingRight = right;
  paddingBottom = bottom;
  paddingLeft = left;
}

Math::CTransform *CBasicElement::GetTransform()
{
  return m_transform;
}

const Math::CTransform *CBasicElement::GetTransform() const
{
  return m_transform;
}

void CBasicElement::SetPosition( const Math::Vector2& position, float time )
{
  //m_transform->localPosition = Math::Vector3( position.x, position.y, 0 );
  m_end.localPosition = Math::Vector3( position.x, position.y, 0 );

  SetTransformTime( time );
}

void CBasicElement::SetTransformTime( float time )
{
  m_begin.localPosition = m_transform->localPosition;

  if( time > 0.001f )
  {
    m_transformTime = 0.0f;
    m_transformTimeScale = 1.0f / time;
  }
  else
  {
    m_transformTime = 1.0f;
    m_transformTimeScale = 1.0f;
  }
}

void CBasicElement::Update( float elapsed )
{
  m_transformTime += elapsed;
  m_colorTime += elapsed;

  float t_xform = Utilities::Clamp( m_transformTime * m_transformTimeScale, 0.0f, 1.0f );

  // Interpolate position
  m_transform->localPosition = m_begin.localPosition + t_xform * (m_end.localPosition - m_begin.localPosition);

  float t_color = Utilities::Clamp( m_colorTime * m_colorTimeScale, 0.0f, 1.0f );

  // Interpolate color
  m_color = Color4f::Interpolate( m_colorBegin, m_colorEnd, t_color );

  for(unsigned int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->Update( elapsed );
  }
}

bool CBasicElement::InitDependencies( Engine::Entity *entity )
{
  m_entityID = entity->ID();

  if(entity->Has( Engine::ComponentType::TRANSFORM ))
  {
    Math::CTransform *transform = static_cast<Math::CTransform *>(entity->GetFirstComponent( Engine::ComponentType::TRANSFORM ) );

    if(transform)
    {
      m_transform = transform;
      return true;
    }
  }

  return false;
}

void CBasicElement::HandleEvent(EventType::ID eventType)
{
  Math::Ray ray(
    Math::Vector3((float)Globals::input->MouseX(), (float)Globals::input->MouseY(), -1.0f),
    Math::Vector3(0, 0, 1.0f) );

  Math::Quad boundingArea(halfWidth * 2, halfHeight * 2);
  boundingArea.SetPosition( m_transform->GetWorldPosition() );

  float time;

  switch(eventType)
  {
  case EventType::LEFTCLICK:
    if( boundingArea.Intersection(ray, time) || m_focus )
      OnLeftClick();
    break;

  case EventType::LEFTCLICK_HOLD:
    if( boundingArea.Intersection(ray, time) || m_focus )
      OnLeftClickHold();
    break;

  case EventType::LEFTMOUSE_UP:
      OnLeftMouseUp();
    break;

  case EventType::RIGHTCLICK:
    if( boundingArea.Intersection(ray, time) || m_focus )
      OnRightClick();
    break;

  case EventType::RIGHTCLICK_HOLD:
    if( boundingArea.Intersection(ray, time) || m_focus )
      OnRightClickHold();
    break;

  case EventType::RIGHTMOUSE_UP:
      OnRightMouseUp();
    break;

  case EventType::MOUSEMOVE:
    if( boundingArea.Intersection(ray, time) /*|| m_focus*/ )
      OnMouseOver();
    else
      OnMouseOut();
    break;

  case EventType::SCROLL:
    if(m_focus)
      OnScroll();
    break;

  case EventType::KEYPRESS:
    if(m_focus)
      OnKeypress();
    break;
  }
} 

void CBasicElement::PassEvent(EventType::ID eventType)
{
  // Don't iterate over elements that may be added from an event
  ElementList tmp_children = m_children;

  for(unsigned int i = 0; i < tmp_children.size(); ++i)
  {
    // Make sure the element wasn't removed
    //if( std::find( m_children.begin(), m_children.end(), tmp_children[i] ) != m_children.end() )
      tmp_children[i]->HandleEvent( eventType );
  }
}


void CBasicElement::HandleLuaError( lua_State* ls, const char* info, int regIndex )
{
  Globals::scriptEngine->HandleLuaError( ls, info, true, regIndex );
}




LUA_START_DEFINE_CLASS( CBasicElement )
  LUA_NAME_METHOD( CBasicElement, "Base", LBase )

  LUA_NAME_METHOD( CBasicElement, "GetAlpha", LGetAlpha )
  LUA_NAME_METHOD( CBasicElement, "GetColor", LGetColor )
  LUA_NAME_METHOD( CBasicElement, "GetDimensions", LGetDimensions )
  LUA_NAME_METHOD( CBasicElement, "GetMargin", LGetMargin )

  LUA_NAME_METHOD( CBasicElement, "SetAlpha", LSetAlpha )
  LUA_NAME_METHOD( CBasicElement, "SetColor", LSetColor )
  LUA_NAME_METHOD( CBasicElement, "SetDimensions", LSetDimensions )
  LUA_NAME_METHOD( CBasicElement, "SetMargin", LSetMargin )
  LUA_NAME_METHOD( CBasicElement, "SetPadding", LSetPadding )
  LUA_NAME_METHOD( CBasicElement, "SetPosition", LSetPosition )
  LUA_NAME_METHOD( CBasicElement, "SetPositionMode", LSetPositionMode )

  LUA_NAME_METHOD( CBasicElement, "Orphan", LOrphan )
  LUA_NAME_METHOD( CBasicElement, "OrphanTransform", LOrphanTransform )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( CBasicElement, LBase )
{
  PushToLua(ls);

  return 1;
}

LUA_DEFINE_METHOD( CBasicElement, LGetAlpha )
{
  lua_pushnumber(ls, m_color.a);

  return 1;
}

LUA_DEFINE_METHOD( CBasicElement, LGetColor )
{
  lua_pushnumber(ls, m_color.r);
  lua_pushnumber(ls, m_color.g);
  lua_pushnumber(ls, m_color.b);
  lua_pushnumber(ls, m_color.a);

  return 4;
}

LUA_DEFINE_METHOD( CBasicElement, LGetDimensions )
{
  lua_pushnumber(ls, halfWidth * 2);
  lua_pushnumber(ls, halfHeight * 2);

  return 2;
}

LUA_DEFINE_METHOD( CBasicElement, LGetMargin )
{
  lua_pushnumber(ls, marginTop);
  lua_pushnumber(ls, marginRight);
  lua_pushnumber(ls, marginBottom);
  lua_pushnumber(ls, marginLeft);

  return 4;
}

LUA_DEFINE_METHOD( CBasicElement, LSetAlpha )
{
  float a = luaL_checkfloat(ls, 1);
  float time = 0;

  if( lua_gettop(ls) > 1 )
    time = luaL_checkfloat(ls, 2);

  SetAlpha( a, time);

  return 0;
}

LUA_DEFINE_METHOD( CBasicElement, LSetColor )
{
  float r = luaL_checkfloat(ls, 1);
  float g = luaL_checkfloat(ls, 2);
  float b = luaL_checkfloat(ls, 3);
  float a = luaL_checkfloat(ls, 4);
  float time = 0;

  if( lua_gettop(ls) > 4 )
    time = luaL_checkfloat(ls, 5);

  SetColor( Color4f( r, g, b, a ), time);

  return 0;
}

LUA_DEFINE_METHOD( CBasicElement, LSetDimensions )
{
  float width = luaL_checkfloat(ls, 1);
  float height = luaL_checkfloat(ls, 2);

  halfWidth = width * 0.5f;
  halfHeight = height * 0.5f;

  return 0;
}

LUA_DEFINE_METHOD( CBasicElement, LSetMargin )
{
  float top = luaL_checkfloat( ls, 1 );
  float right = luaL_checkfloat( ls, 2 );
  float bottom = luaL_checkfloat( ls, 3 );
  float left = luaL_checkfloat( ls, 4 );

  SetMargin(top,right,bottom,left);

  return 0;
}

LUA_DEFINE_METHOD( CBasicElement, LSetPadding )
{
  SetPadding(
    luaL_checkfloat(ls,1),
    luaL_checkfloat(ls,2),
    luaL_checkfloat(ls,3),
    luaL_checkfloat(ls,4) );

  return 0;
}

LUA_DEFINE_METHOD( CBasicElement, LSetPosition )
{
  // x, y positioning
  if( lua_isnumber(ls, 1) && lua_isnumber(ls, 2) )
  {
    float x = (float)lua_tonumber(ls, 1);
    float y = (float)lua_tonumber(ls, 2);

    SetPosition( Math::Vector2(x,y) );

    return 0;
  }
  // Left/Center/Right, Top/Middle/Bottom positioning
  else if( lua_isstring(ls, 1) && lua_isstring(ls, 2) )
  {
    std::string hAlignString = lua_tostring( ls, 1 );
    std::string vAlignString = lua_tostring( ls, 2 );

    if(hAlignString == "LEFT")
      hPosition = HAlign::LEFT;
    else if(hAlignString == "CENTER")
      hPosition = HAlign::CENTER;
    else if(hAlignString == "RIGHT")
      hPosition = HAlign::RIGHT;

    if(vAlignString == "TOP")
      vPosition = VAlign::TOP;
    else if(vAlignString == "MIDDLE")
      vPosition = VAlign::MIDDLE;
    else if(vAlignString == "BOTTOM")
      vPosition = VAlign::BOTTOM;

    return 0;
  }

  return 0;
}

LUA_DEFINE_METHOD( CBasicElement, LSetPositionMode )
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

LUA_DEFINE_METHOD_NOLS( CBasicElement, LOrphan )
{
  Orphan();

  return 0;
}

LUA_DEFINE_METHOD_NOLS( CBasicElement, LOrphanTransform )
{
  m_transform->localPosition = m_transform->GetWorldPosition();
  m_transform->Orphan();

  SetPosition( Math::Vector2(m_transform->localPosition.x, m_transform->localPosition.y) );

  return 0;
}


}

}
