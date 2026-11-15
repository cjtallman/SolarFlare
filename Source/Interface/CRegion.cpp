/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CRegion.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/17/2009 @ 17:47:24 - nwilliam
**  \brief   Graphical region, holds other elements.
*******************************************************************************/

#include "CRegion.h"

#include <cmath> // abs
#include "MenuSystem.h"
#include "Math/CTransform.h"
#include "Math/TVector2.h"
#include "SolarFlare/Globals.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace UI
{

CRegion::CRegion()
{
  m_vAlign = VAlign::TOP;
  m_hAlign = HAlign::LEFT;
  m_layout = Layout::LEFT_RIGHT;
}

CRegion::~CRegion()
{
}


void CRegion::SetAlign(HAlign::ID hAlign, VAlign::ID vAlign)
{
  m_hAlign = hAlign;
  m_vAlign = vAlign;
}

void CRegion::SetLayout(Layout::ID layout)
{
  m_layout = layout;
}

void CRegion::OnLeftClick()
{
  PassEvent(EventType::LEFTCLICK);
}

void CRegion::OnLeftClickHold()
{
  PassEvent(EventType::LEFTCLICK_HOLD);
}

void CRegion::OnRightClick()
{
  PassEvent(EventType::RIGHTCLICK);
}

void CRegion::OnRightClickHold()
{
  PassEvent(EventType::RIGHTCLICK_HOLD);
}

void CRegion::OnKeypress()
{
  PassEvent(EventType::KEYPRESS);
}

void CRegion::OnMouseOver()
{
  m_mouseOver = true;
  PassEvent(EventType::MOUSEMOVE);
}

void CRegion::OnMouseOut()
{
  m_mouseOver = false;
  PassEvent(EventType::MOUSEMOVE);
}

void CRegion::OnScroll()
{
  PassEvent(EventType::SCROLL);
}

void CRegion::AutoResize()
{
  AutoResizeChildren();

  float horizExtent = 0;
  float vertExtent = 0;

  for(unsigned int i = 0; i < m_children.size(); ++i)
  {
    CBasicElement *element = m_children[i];

    int horizMargin = (int)(element->marginLeft + element->marginRight);
    int vertMargin = (int)(element->marginTop + element->marginBottom);

    if(m_layout == Layout::LEFT_RIGHT || m_layout == Layout::RIGHT_LEFT)
    {
      horizExtent += element->halfWidth + horizMargin/2;
    }
    else
    {
      horizExtent = std::max(horizExtent, element->halfWidth + horizMargin/2);
    }

    if(m_layout == Layout::TOP_BOTTOM || m_layout == Layout::BOTTOM_TOP)
    {
      vertExtent += element->halfHeight + vertMargin/2;
    }
    else
    {
      vertExtent = std::max(vertExtent, element->halfHeight + vertMargin/2);
    }
  }

  halfWidth = std::max(horizExtent + (paddingLeft + paddingRight)/2, halfWidth);
  halfHeight = std::max(vertExtent + (paddingTop + paddingBottom)/2, halfHeight);
}

void CRegion::AutoResizeChildren()
{
  for(unsigned int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->AutoResize();
  }
}

void CRegion::Format()
{
  float leftExtent = 0;
  float rightExtent = 0;

  float topExtent = 0;
  float bottomExtent = 0;

  // Get the extents of the all the child elements
  for(unsigned int i = 0; i < m_children.size(); ++i)
  {
    CBasicElement *element = m_children[i];

    if( element->alignment == Alignment::ABSOLUTE)
      continue;

    if(m_layout == Layout::LEFT_RIGHT || m_layout == Layout::RIGHT_LEFT)
    {
      leftExtent -= element->halfWidth + (element->marginLeft + element->marginRight)/2;
      rightExtent += element->halfWidth + (element->marginLeft + element->marginRight)/2;
    }
    else
    {
      leftExtent = std::min(leftExtent, -element->halfWidth);
      rightExtent = std::max(rightExtent, element->halfWidth);
    }

    if(m_layout == Layout::TOP_BOTTOM || m_layout == Layout::BOTTOM_TOP)
    {
      topExtent -= element->halfHeight + (element->marginBottom + element->marginTop)/2;
      bottomExtent += element->halfHeight + (element->marginBottom + element->marginTop)/2;
    }
    else
    {
      topExtent = std::max(topExtent, -element->halfHeight);
      bottomExtent = std::min(bottomExtent, element->halfHeight);
    }
  }

  // Horizontal alignment
  switch(m_hAlign)
  {
  default:
  case HAlign::CENTER:
    break;
  case HAlign::LEFT:
    rightExtent += -halfWidth - leftExtent + paddingLeft;
    leftExtent += -halfWidth - leftExtent + paddingLeft;
    break;
  case HAlign::RIGHT:
    leftExtent += halfWidth - rightExtent - paddingRight;
    rightExtent += halfWidth - rightExtent - paddingRight;
    break;
  }

  // Vertical alignment
  switch(m_vAlign)
  {
  default:
  case VAlign::MIDDLE:
    break;
  case VAlign::TOP:
    bottomExtent += -halfHeight - topExtent + paddingTop;
    topExtent += -halfHeight - topExtent + paddingTop;
    break;
  case VAlign::BOTTOM:
    topExtent += halfHeight - bottomExtent - paddingBottom;
    bottomExtent += halfHeight - bottomExtent - paddingBottom;
    break;
  }

  // If the user caused the extents to go inside out, we set both extents to their average
  if(leftExtent > rightExtent)
    leftExtent = rightExtent = (leftExtent + rightExtent) / 2;

  if(topExtent > bottomExtent)
    bottomExtent = topExtent = (bottomExtent + topExtent) / 2;

  float leftOffset = 0.0f;
  float rightOffset = 0.0f;
  float topOffset = 0.0f;
  float bottomOffset = 0.0f;

  for(unsigned int i = 0; i < m_children.size(); ++i)
  {
    CBasicElement *element = m_children[i];

    Math::Vector2 position;

    // Absolute Positioning
    if(element->alignment == Alignment::ABSOLUTE)
    {
      switch(element->hPosition)
      {
      default:
      case HAlign::CENTER:
        position.x = 0;
        break;
      case HAlign::LEFT:
        position.x = -halfWidth + element->halfWidth;
        break;
      case HAlign::RIGHT:
        position.x = halfWidth - element->halfWidth;
        break;
      }

      switch(element->vPosition)
      {
      default:
      case VAlign::MIDDLE:
        position.y = 0;
        break;
      case VAlign::TOP:
        position.y = -halfHeight + element->halfHeight;
        break;
      case VAlign::BOTTOM:
        position.y = halfHeight - element->halfHeight;
        break;
      }

      position.y += element->marginTop - element->marginBottom;
      position.x += element->marginLeft - element->marginRight;

      element->SetPosition( position );
    }
    // Relative Positioning
    else
    {
      // Horizontal alignment
      switch(m_hAlign)
      {
      default:
      case HAlign::CENTER:
        position.x = (leftExtent + rightExtent) / 2;
        break;
      case HAlign::LEFT:
        position.x = leftExtent + element->halfWidth;
        break;
      case HAlign::RIGHT:
        position.x = rightExtent - element->halfWidth;
        break;
      }

      // Vertical alignment
      switch(m_vAlign)
      {
      default:
      case VAlign::MIDDLE:
        position.y = (topExtent + bottomExtent) / 2;
        break;
      case VAlign::TOP:
        position.y = bottomExtent + element->halfHeight;
        break;
      case VAlign::BOTTOM:
        position.y = topExtent - element->halfHeight;
        break;
      }

      switch(m_layout)
      {
      case Layout::LEFT_RIGHT:
        leftOffset += element->marginLeft;
        position.x = leftExtent + leftOffset + element->halfWidth;
        leftOffset += element->marginRight;
        leftOffset += 2 * element->halfWidth;
        break;
      case Layout::RIGHT_LEFT:
        rightOffset += element->marginRight;
        position.x = rightExtent - rightOffset - element->halfWidth;
        rightOffset += element->marginLeft;
        rightOffset += 2 * element->halfWidth;
        break;
      case Layout::TOP_BOTTOM:
        topOffset += element->marginTop;
        position.y = topExtent + topOffset + element->halfHeight;
        topOffset += element->marginBottom;
        topOffset += 2 * element->halfHeight;
        break;
      case Layout::BOTTOM_TOP:
        bottomOffset += element->marginBottom;
        position.y = bottomExtent - bottomOffset - element->halfHeight;
        bottomOffset += element->marginTop;
        bottomOffset += 2 * element->halfHeight;
        break;
      }

      element->SetPosition( position );
    }
  }

  // Format the children
  FormatChildren();
}

void CRegion::FormatChildren()
{
  for(unsigned int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->Format();
  }
}

Engine::BaseComponent* CRegion::Clone() const
{
  return new CRegion( *this );
}

void CRegion::PushToLua( lua_State *ls )
{
  Lunar<CRegion>::push(ls, this);
}

Engine::BaseComponent *CRegion::Creator(const ticpp::Element *)
{
  CRegion *region = new CRegion;

  return region;
}

LUA_START_DEFINE_CLASS( CRegion )
  LUA_NAME_METHOD( CRegion, "Base", LBase )

  LUA_NAME_METHOD( CRegion, "Orphan", LOrphan )
  LUA_NAME_METHOD( CRegion, "OrphanTransform", LOrphanTransform )

  LUA_NAME_METHOD( CRegion, "SetColor", LSetColor )
  LUA_NAME_METHOD( CRegion, "GetColor", LGetColor )

  LUA_NAME_METHOD( CRegion, "SetAlpha", LSetAlpha )
  LUA_NAME_METHOD( CRegion, "GetAlpha", LGetAlpha )

  LUA_NAME_METHOD( CRegion, "SetAlignment", LSetAlignment )
  LUA_NAME_METHOD( CRegion, "SetLayout", LSetLayout )
  LUA_NAME_METHOD( CRegion, "SetMargin", LSetMargin )
  LUA_NAME_METHOD( CRegion, "SetPadding", LSetPadding )
  LUA_NAME_METHOD( CRegion, "SetPosition", LSetPosition )
  LUA_NAME_METHOD( CRegion, "SetPositionMode", LSetPositionMode )

LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( CRegion, LBase )
{
  CBasicElement::PushToLua(ls);
  return 1;
}

LUA_DEFINE_METHOD( CRegion, LOrphan )
{
  return CBasicElement::LOrphan(ls);
}

LUA_DEFINE_METHOD( CRegion, LOrphanTransform )
{
  return CBasicElement::LOrphanTransform(ls);
}

LUA_DEFINE_METHOD( CRegion, LSetColor )
{
  return CBasicElement::LSetColor(ls);
}

LUA_DEFINE_METHOD( CRegion, LGetColor )
{
  return CBasicElement::LGetColor(ls);
}

LUA_DEFINE_METHOD( CRegion, LSetAlpha )
{
  return CBasicElement::LSetAlpha(ls);
}

LUA_DEFINE_METHOD( CRegion, LGetAlpha )
{
  return CBasicElement::LGetAlpha(ls);
}

LUA_DEFINE_METHOD( CRegion, LSetAlignment )
{
  std::string hAlignString = luaL_checkstring( ls, 1 );
  std::string vAlignString = luaL_checkstring( ls, 2 );

  HAlign::ID hAlign = HAlign::LEFT;
  VAlign::ID vAlign = VAlign::TOP;

  if(hAlignString == "LEFT")
    hAlign = HAlign::LEFT;
  else if(hAlignString == "CENTER")
    hAlign = HAlign::CENTER;
  else if(hAlignString == "RIGHT")
    hAlign = HAlign::RIGHT;

  if(vAlignString == "TOP")
    vAlign = VAlign::TOP;
  else if(vAlignString == "MIDDLE")
    vAlign = VAlign::MIDDLE;
  else if(vAlignString == "BOTTOM")
    vAlign = VAlign::BOTTOM;

  SetAlign(hAlign, vAlign);

  return 0;
}

LUA_DEFINE_METHOD( CRegion, LSetLayout )
{
  std::string layoutString = luaL_checkstring( ls, 1 );

  Layout::ID layout = Layout::LEFT_RIGHT;

  if(layoutString == "LeftToRight")
    layout = Layout::LEFT_RIGHT;
  else if(layoutString == "RightToLeft")
    layout = Layout::RIGHT_LEFT;
  else if(layoutString == "TopToBottom")
    layout = Layout::TOP_BOTTOM;
  else if(layoutString == "BottomToTop")
    layout = Layout::BOTTOM_TOP;

  SetLayout(layout);

  return 0;
}

LUA_DEFINE_METHOD( CRegion, LSetMargin )
{
  return CBasicElement::LSetMargin(ls);
}

LUA_DEFINE_METHOD( CRegion, LSetPadding )
{
  CBasicElement::LSetPadding(ls);

  return 0;
}

LUA_DEFINE_METHOD( CRegion, LSetPosition )
{
  return CBasicElement::LSetPosition(ls);
}

LUA_DEFINE_METHOD( CRegion, LSetPositionMode )
{
  return CBasicElement::LSetPositionMode(ls);
}

}

}
