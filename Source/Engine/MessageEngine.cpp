/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    MessageEngine.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 01/29/2009 @ 15:19:30 - nwilliam
**  \brief   Implementation of messaging engine.
*******************************************************************************/

#include <algorithm> // stable_sort
#include "EntityFactory.h"
#include "MessageEngine.h"
#include "SolarFlare/Globals.h" // entityFactory, networkEngine
#include "Networking/NetworkEngine.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Engine
{

MessageEngine::MessageEngine()
{
  m_currentTime = 0;
}

Message *MessageEngine::AddMessage(const std::string& type, BaseEngine *engine, float delay )
{
  FullMessage message;

  message.type = FullMessage::ENGINE;
  message.destination.engine = engine;
  message.time = m_currentTime + std::max(0.01f, delay);
  message.msg = new Message;
  message.msg->type = type;

  m_messageQueue.push_back( message );
  //std::stable_sort( m_messageQueue.begin(), m_messageQueue.end() );

  return message.msg;
}

Message *MessageEngine::AddMessage(const std::string& type, float delay, Message::ROUTE route )
{
  FullMessage message;

  message.type = FullMessage::GLOBAL;
  message.time = m_currentTime + std::max(0.01f, delay);
  message.msg = new Message;
  message.msg->type = type;
  message.msg->route = route;

  m_messageQueue.push_back( message );
  //std::stable_sort( m_messageQueue.begin(), m_messageQueue.end() );

  return message.msg;
}

Message *MessageEngine::AddMessage(const std::string& type, EntityID entity, float delay, Message::ROUTE route )
{
  FullMessage message;

  message.type = FullMessage::ENTITY;
  message.destination.entity = entity;
  message.time = m_currentTime + std::max(0.01f, delay);
  message.msg = new Message;
  message.msg->type = type;
  message.msg->route = route;

  m_messageQueue.push_back( message );
  //std::stable_sort( m_messageQueue.begin(), m_messageQueue.end() );

  return message.msg;
}

void MessageEngine::AddSubscription(const std::string& type, EntityID entity)
{
  m_entitySubscribers.insert( StringEntityMap::value_type(type, entity) );
}

void MessageEngine::RemoveSubscription(const std::string& type, EntityID entity)
{
  std::pair< StringEntityMap::iterator, StringEntityMap::iterator > iterPair;

  iterPair = m_entitySubscribers.equal_range( type );

  while(iterPair.first != iterPair.second)
  {
    if(entity == iterPair.first->second)
    {
      m_entitySubscribers.erase( iterPair.first );
      break;
    }

    ++iterPair.first;
  }
}

void MessageEngine::AddSubscription(const std::string& type, BaseEngine *engine)
{
  m_engineSubscribers.insert( StringEngineMap::value_type(type, engine) );
}

void MessageEngine::RemoveSubscription(const std::string& type, BaseEngine *engine)
{
  std::pair< StringEngineMap::iterator, StringEngineMap::iterator > iterPair;

  iterPair = m_engineSubscribers.equal_range( type );

  while(iterPair.first != iterPair.second)
  {
    if(engine == iterPair.first->second)
    {
      m_engineSubscribers.erase( iterPair.first );
      break;
    }

    ++iterPair.first;
  }
}

void MessageEngine::Update(float elapsed)
{
  std::stable_sort( m_messageQueue.begin(), m_messageQueue.end() );

  m_currentTime += elapsed;

  size_t processed = 0;
  size_t startSize = m_messageQueue.size() ;

  while(!m_messageQueue.empty() && m_messageQueue.front().time <= m_currentTime)
  {
    FullMessage message = m_messageQueue.front();
    m_messageQueue.pop_front();

    DispatchMessage(message);

    ++processed;

    if( processed > startSize )
      break;
  }
}

void MessageEngine::DispatchMessage(const FullMessage& message)
{
  Message::ROUTE& route = message.msg->route;

  if( route != Message::MSG_LOCAL )
  {
    Globals::networkEngine->HandleMessage( message.msg );
  }

  if( route == Message::MSG_ALL || route == Message::MSG_LOCAL )
  {
    switch( message.type )
    {
      default:
      case FullMessage::GLOBAL:
        DispatchGlobalMessage( message );
      break;

      case FullMessage::ENGINE:
        message.destination.engine->HandleMessage( message.msg );
      break;

      case FullMessage::ENTITY:
        Entity *entity = Globals::entityFactory->GetEntity( message.destination.entity );
        if( entity ) entity->HandleMessage( message.msg );
      break;
    }
  }

  delete message.msg;
}

void MessageEngine::DispatchGlobalMessage(const FullMessage& message)
{
  // Engine subscribers
  std::pair< StringEngineMap::iterator, StringEngineMap::iterator > engIterPair;

  engIterPair = m_engineSubscribers.equal_range( message.msg->type );

  while(engIterPair.first != engIterPair.second)
  {
    BaseEngine* engine = engIterPair.first->second;
    engine->HandleMessage( message.msg );
    ++engIterPair.first;
  }

  // Entity subscribers
  std::pair< StringEntityMap::iterator, StringEntityMap::iterator > entIterPair;

  entIterPair = m_entitySubscribers.equal_range( message.msg->type );

  while(entIterPair.first != entIterPair.second)
  {
    Entity* entity = Globals::entityFactory->GetEntity( entIterPair.first->second );
    if( entity ) entity->HandleMessage( message.msg );
    ++entIterPair.first;
  }
}


}

}
