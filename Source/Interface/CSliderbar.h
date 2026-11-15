/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CSliderbar.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/16/2008 @ 07:07:55 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_CSLIDERBAR_H
#define SF_CSLIDERBAR_H

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

class CSliderbar : public CBasicElement
{
public:
  CSliderbar();
  ~CSliderbar();

  virtual bool InitDependencies( Engine::Entity * );
  virtual BaseComponent* Clone() const;
  virtual void PushToLua( lua_State *ls );

  virtual void OnLeftClickHold();
  virtual void OnLeftMouseUp();
  virtual void OnMouseOver();
  virtual void OnMouseOut();

  static Engine::BaseComponent *Creator(const ticpp::Element *element);

private:
  Math::CTransform *m_sliderTransform;
  lua_State *       m_onClickLuaState;
  int               m_onClick;

  float             m_rangeLower;
  float             m_rangeUpper;

  float             m_value;

  LUA_DECLARE_CLASS( CSliderbar )
  LUA_DECLARE_METHOD( LBase )

  LUA_DECLARE_METHOD( LEntityID )

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
  LUA_DECLARE_METHOD( LSetRange )
  LUA_DECLARE_METHOD( LSetValue )
};


}

}

#endif
