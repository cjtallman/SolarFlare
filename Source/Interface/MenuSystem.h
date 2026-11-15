/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    MenuSystem.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 09/16/2008 @ 23:15:47 - nwilliam
**  \brief   Engine for controlling, updating, and maintining the UI.
*******************************************************************************/

#ifndef SF_MENU_MANAGER_H
#define SF_MENU_MANAGER_H

#include <map>
#include <stack>

#include "CBasicElement.h"
#include "CRegion.h"

#include "Engine/BaseEngine.h"
#include "Engine/Entity.h"
#include "External/Lua/include/LunarHelper.h"

namespace SF {
namespace UI
{

class MenuSystem : public Engine::BaseEngine
{
  friend class CBasicElement;
  friend class CRegion;

public:
  MenuSystem();
  ~MenuSystem();

  void AddElement(Engine::EntityID entityID);
  void RemoveElement(Engine::EntityID entityID);

  void HandleInput(CBasicElement *element);

  void Format();

  void Focus(CBasicElement *element);
  void Unfocus();

  void Hover(bool hover);
  bool Hover();

  void ProcessedClick(bool processed);
  bool ProcessedClick();

  void SetScreenSize(int sw, int sh);

  virtual void HandleMessage( const Engine::Message *msg );

  virtual void Update(float elapsed);

private:
  typedef std::vector<CBasicElement *> ElementList;

  CBasicElement * m_menuRoot;
  ElementList     m_menuRoots;

  CBasicElement * m_focus;

  CBasicElement * m_activeElement;

  ElementList     m_updateList;

  Math::TVector2<int> m_screenSize;

  std::vector<Engine::EntityID> m_entities;

  bool m_processedClick;
  bool m_hover;

  bool m_leftMouseDown;
  bool m_rightMouseDown;

  void            AddToUpdateList(CBasicElement *element);

  CBasicElement * GetUIComponent(int entityID) const;

  void            ResizeMenuRoot();

  LUA_DECLARE_CLASS( MenuSystem )

  LUA_DECLARE_METHOD( LAddToUpdateList )
  LUA_DECLARE_METHOD( LFormat )
  LUA_DECLARE_METHOD( LHandleInput )
  LUA_DECLARE_METHOD( LHover )
  LUA_DECLARE_METHOD( LProcessedClick )
  LUA_DECLARE_METHOD( LResetActiveElement )
  LUA_DECLARE_METHOD( LSetActiveElement )
};

}

}

#endif
