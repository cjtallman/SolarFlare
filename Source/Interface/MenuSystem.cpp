/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    MenuSystem.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 02/08/2009 @ 19:26:14 - pmera
**  \brief   
*******************************************************************************/

#include "MenuSystem.h"

#include <algorithm> // remove
#include "CRegion.h"
#include "CButton.h"
#include "CInputRegion.h"
#include "CSliderbar.h"

#include "SolarFlare/Globals.h"
#include "Engine/InputEngine.h"
#include "Engine/EntityFactory.h"
#include "Engine/MessageEngine.h"

namespace SF {
namespace UI
{

MenuSystem::MenuSystem()
{
  Globals::entityFactory->Creator( "UI_BASICELEMENT", Engine::CCreator::Function<CBasicElement::Creator>() );
  Globals::entityFactory->Creator( "UI_BUTTON", Engine::CCreator::Function<CButton::Creator>() );
  Globals::entityFactory->Creator( "UI_REGION", Engine::CCreator::Function<CRegion::Creator>() );
  Globals::entityFactory->Creator( "UI_INPUTREGION", Engine::CCreator::Function<CInputRegion::Creator>() );
  Globals::entityFactory->Creator( "UI_SLIDER", Engine::CCreator::Function<CSliderbar::Creator>() );

  Globals::entityFactory->DeleteNotifier(
    Engine::ComponentType::UI_ELEMENT, Engine::Notifier::Method<MenuSystem, &MenuSystem::RemoveElement>(this) );

  Globals::entityFactory->CreateNotifier(
    Engine::ComponentType::UI_ELEMENT, Engine::Notifier::Method<MenuSystem, &MenuSystem::AddElement>(this) );

  Globals::messageEngine->AddSubscription( "WindowResize", this );

  m_focus = 0;
  m_activeElement = 0;

  m_leftMouseDown = false;
  m_rightMouseDown = false;
}

MenuSystem::~MenuSystem()
{
}

void MenuSystem::AddElement(Engine::EntityID entityID)
{
  CBasicElement *element = GetUIComponent(entityID);

  // Add the element to the active element
  if(m_activeElement)
  {
    m_activeElement->AddChild(element);
  }
  else
  {
    m_menuRoots.push_back( element );

    ResizeMenuRoot();
  }
}

void MenuSystem::RemoveElement(Engine::EntityID entityID)
{
  CBasicElement *element = GetUIComponent(entityID);

  // If its a root, remove it from the menu root list
  if( element->m_parent == 0 )
    m_menuRoots.erase( std::remove( m_menuRoots.begin(), m_menuRoots.end(), element ), m_menuRoots.end() );

  // Promote the children
  element->PromoteChildren();

  // Remove relationship with parent
  element->Orphan();

  // Remove from update list
  m_updateList.erase(std::remove(m_updateList.begin(), m_updateList.end(), element), m_updateList.end() );
}

void MenuSystem::SetScreenSize(int sw, int sh)
{
  m_screenSize.x = sw;
  m_screenSize.y = sh;

  ResizeMenuRoot();
}

void MenuSystem::HandleMessage( const Engine::Message *msg )
{
  if( msg->type == "WindowResize" )
  {
    SetScreenSize( msg->stack.Get<int>(0), msg->stack.Get<int>(1) );
  }
}

void MenuSystem::HandleInput(CBasicElement *element)
{
  if( Globals::input->IsAnyPressed() )
  {
    element->HandleEvent( EventType::KEYPRESS );
  }

  if( Globals::input->IsTriggered( Engine::InputMap::MOUSE_LEFT ) )
    element->HandleEvent( EventType::LEFTCLICK );

  if( Globals::input->IsPressed( Engine::InputMap::MOUSE_LEFT ) )
  {
    m_leftMouseDown = true;
    element->HandleEvent( EventType::LEFTCLICK_HOLD );
  }
  else if( m_leftMouseDown )
  {
    element->HandleEvent( EventType::LEFTMOUSE_UP );
    m_leftMouseDown = false;
  }

  if( Globals::input->IsTriggered( Engine::InputMap::MOUSE_RIGHT ) )
    element->HandleEvent( EventType::RIGHTCLICK );

  if( Globals::input->IsPressed( Engine::InputMap::MOUSE_RIGHT ) )
  {
    m_rightMouseDown = true;
    element->HandleEvent( EventType::RIGHTCLICK_HOLD );
  }
  else if( m_rightMouseDown )
  {
    element->HandleEvent( EventType::RIGHTMOUSE_UP );
    m_rightMouseDown = false;
  }

  if( Globals::input->WheelDelta() )
    element->HandleEvent( EventType::SCROLL );

  //if( Globals::input->MouseDeltaX() || Globals::input->MouseDeltaY() )
    element->HandleEvent( EventType::MOUSEMOVE );
}

void MenuSystem::Format()
{
  if( m_menuRoots.front() )
  {
    m_menuRoots.front()->AutoResizeChildren();
    m_menuRoots.front()->Format();
  }
}

void MenuSystem::Focus(CBasicElement *element)
{
  m_focus = element;
}

void MenuSystem::Unfocus()
{
  m_focus = 0;
}

void MenuSystem::Hover(bool hover)
{
  m_hover = hover;
}

bool MenuSystem::Hover()
{
  return m_hover;
}

void MenuSystem::ProcessedClick(bool processed)
{
  m_processedClick = processed;
}

bool MenuSystem::ProcessedClick()
{
  return m_processedClick;
}

void MenuSystem::Update(float elapsed)
{
  for(size_t i = 0; i < m_menuRoots.size(); ++i)
  {
    m_menuRoots[i]->Update( elapsed );

    m_menuRoots[i]->AutoResizeChildren();
    m_menuRoots[i]->Format();
  }

  for(unsigned int i = 0; i < m_updateList.size(); ++i)
    m_updateList[i]->Update( elapsed );
}

void MenuSystem::AddToUpdateList(CBasicElement *element)
{
  m_updateList.push_back(element);
}

CBasicElement *MenuSystem::GetUIComponent(int entityID) const
{
  Engine::Entity *entity = Globals::entityFactory->GetEntity(entityID);
  CBasicElement *element = static_cast<CBasicElement *>(entity->GetFirstComponent( Engine::ComponentType::UI_ELEMENT ));

  return element;
}

void MenuSystem::ResizeMenuRoot()
{
  if( m_menuRoots.front() )
  {
    m_menuRoots.front()->halfWidth = m_screenSize.x * 0.5f;
    m_menuRoots.front()->halfHeight = m_screenSize.y * 0.5f;

    m_menuRoots.front()->SetPosition(
      Math::Vector2(m_menuRoots.front()->halfWidth, m_menuRoots.front()->halfHeight) );
  }
}

LUA_START_DEFINE_CLASS( MenuSystem )
  LUA_NAME_METHOD( MenuSystem, "AddToUpdateList", LAddToUpdateList )
  LUA_NAME_METHOD( MenuSystem, "Format", LFormat)
  LUA_NAME_METHOD( MenuSystem, "HandleInput", LHandleInput )
  LUA_NAME_METHOD( MenuSystem, "Hover", LHover )
  LUA_NAME_METHOD( MenuSystem, "ProcessedClick", LProcessedClick )
  LUA_NAME_METHOD( MenuSystem, "ResetActiveElement", LResetActiveElement )
  LUA_NAME_METHOD( MenuSystem, "SetActiveElement", LSetActiveElement )

LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( MenuSystem, LAddToUpdateList )
{
  int entityID = luaL_checkint(ls, 1);

  CBasicElement *element = GetUIComponent(entityID);

  AddToUpdateList(element);

  return 0;
}

LUA_DEFINE_METHOD_NOLS( MenuSystem, LFormat)
{
  Format();

  return 0;
}

LUA_DEFINE_METHOD_NOLS( MenuSystem, LHandleInput )
{
  m_processedClick = false;
  m_hover = false;

  if(m_focus)
    HandleInput( m_focus );

  for(size_t i = 0; i < m_menuRoots.size(); ++i)
    HandleInput( m_menuRoots[i] );

  return 0;
}

LUA_DEFINE_METHOD( MenuSystem, LHover )
{
  lua_pushboolean(ls, m_hover);
  return 1;
}

LUA_DEFINE_METHOD( MenuSystem, LProcessedClick )
{
  lua_pushboolean(ls, m_processedClick);
  return 1;
}

LUA_DEFINE_METHOD_NOLS( MenuSystem, LResetActiveElement )
{
  m_activeElement = 0;

  return 0;
}

LUA_DEFINE_METHOD( MenuSystem, LSetActiveElement )
{
  int entityID = luaL_checkint(ls,1);

  CBasicElement *element = GetUIComponent(entityID);

  if(element)
  {
    m_activeElement = element;
  }

  return 0;
}

}
}
