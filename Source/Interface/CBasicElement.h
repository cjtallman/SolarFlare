/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CBasicElement.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/05/2009 @ 16:59:27 - nwilliam
**  \brief   Common base class to all UI elements.
*******************************************************************************/

#ifndef SF_BASE_ELEMENT_H
#define SF_BASE_ELEMENT_H

#include <vector>

#include "Engine/BaseComponent.h"
#include "Math/BaseGeometry.h"
#include "Math/CTransform.h"
#include "Math/Plane.h"
#include "Math/Quad.h"
#include "Math/Quaternion.h"
#include "Math/TVector2.h"
#include "Math/TVector3.h"
#include "Utilities/Color.h"
#include "External/Lua/include/lua.hpp"

namespace SF {
namespace Math { class CTransform; }
namespace UI
{

namespace Alignment
{
  #undef RELATIVE
  #undef ABSOLUTE

  enum ID
  {
    RELATIVE,
    ABSOLUTE
  };
}

namespace HAlign
{
  enum ID
  {
    LEFT,
    CENTER,
    RIGHT
  };
}

namespace VAlign
{
  enum ID
  {
    TOP,
    MIDDLE,
    BOTTOM
  };
}

namespace Layout
{
  enum ID
  {
    LEFT_RIGHT,
    RIGHT_LEFT,
    TOP_BOTTOM,
    BOTTOM_TOP
  };
}

namespace EventType
{
  enum ID
  {
    LEFTCLICK,
    LEFTCLICK_HOLD,
    LEFTMOUSE_UP,
    RIGHTCLICK,
    RIGHTCLICK_HOLD,
    RIGHTMOUSE_UP,
    KEYPRESS,
    MOUSEMOVE,
    SCROLL
  };
}

namespace ElementType
{
  enum ID
  {
    BUTTON,
    CHECKBOX,
    DROPDOWN,
    PANEL,
    RADIOBUTTON,
    SLIDER,
    TEXTBOX,
    WINDOW
  };
}

class CRegion;

class CBasicElement : public Engine::BaseComponent
{
public:
  friend class MenuSystem;

  typedef std::vector<CBasicElement *> ElementList;

  virtual BaseComponent* Clone() const;
  virtual void PushToLua( lua_State *ls );

  static Engine::BaseComponent *Creator(const ticpp::Element *element);

  CBasicElement();
  virtual ~CBasicElement();

  void AddChild(CBasicElement *element);
  void Orphan();
  void PromoteChildren();
  void RemoveChild(CBasicElement *element);
  void RemoveChildren();

  void HandleEvent(EventType::ID eventType);
  void PassEvent(EventType::ID eventType);

  virtual void Focus();
  virtual void Unfocus();

  Color4f GetColor() const;
  float GetWidth() const;
  float GetHeight() const;

  void SetColorMod(const Color4f & color);
  void SetOpacity(float opacity);

  Math::CTransform *GetTransform();
  const Math::CTransform *GetTransform() const;

  void SetMargin(float top, float right, float bottom, float left);

  void SetAlpha( float alpha, float time = 0.0f );
  void SetColor( const Color4f & color, float time = 0.0f );
  void SetColorTime( float time );
  void SetPadding( float top, float right, float bottom, float left );

  void SetPosition( const Math::Vector2& position, float time = 0.0f );
  void SetTransformTime( float time );

  void Update( float elapsed );
  void UpdateChildrenPositions();

  virtual bool InitDependencies( Engine::Entity * );

  virtual void OnLeftClick() {};
  virtual void OnLeftClickHold() {};
  virtual void OnLeftMouseUp() {};
  virtual void OnRightClick() {};
  virtual void OnRightClickHold() {};
  virtual void OnRightMouseUp() {};
  virtual void OnKeypress() {};
  virtual void OnMouseOver() {};
  virtual void OnMouseOut() {};
  virtual void OnScroll() {};

  virtual void AutoResize() {};
  virtual void AutoResizeChildren() {};
  virtual void Format() {};
  virtual void FormatChildren() {};

  float         halfWidth;
  float         halfHeight;
  Alignment::ID alignment;
  HAlign::ID    hPosition;
  VAlign::ID    vPosition;


  float         marginTop;
  float         marginRight;
  float         marginBottom;
  float         marginLeft;

  float         paddingTop;
  float         paddingRight;
  float         paddingBottom;
  float         paddingLeft;

protected:
  bool  m_mouseOver;
  bool  m_focus;

  int   m_entityID;

  Color4f m_colorMod;
  
  HAlign::ID  m_hCollapse;
  VAlign::ID  m_vCollapse;

  Math::CTransform *m_transform;
  Math::CTransform  m_begin;
  Math::CTransform  m_end;

  Color4f m_color;
  Color4f m_colorBegin;
  Color4f m_colorEnd;

  float m_colorTime;
  float m_colorTimeScale;

  float m_transformTime;
  float m_transformTimeScale;

  CBasicElement * m_parent;
  ElementList   m_children;
  

  void HandleLuaError( lua_State* ls, const char* info, int regIndex = LUA_NOREF );


  LUA_DECLARE_CLASS( CBasicElement )

  LUA_DECLARE_METHOD( LBase )

  LUA_DECLARE_METHOD( LSetAlpha )
  LUA_DECLARE_METHOD( LSetColor )
  LUA_DECLARE_METHOD( LSetDimensions )
  LUA_DECLARE_METHOD( LSetMargin )
  LUA_DECLARE_METHOD( LSetPadding )
  LUA_DECLARE_METHOD( LSetPosition )
  LUA_DECLARE_METHOD( LSetPositionMode )

  LUA_DECLARE_METHOD( LGetAlpha )
  LUA_DECLARE_METHOD( LGetColor )
  LUA_DECLARE_METHOD( LGetDimensions )
  LUA_DECLARE_METHOD( LGetMargin )

  LUA_DECLARE_METHOD( LOrphan )
  LUA_DECLARE_METHOD( LOrphanTransform )
};

} // UI namespace
} // SF namespace

#endif
