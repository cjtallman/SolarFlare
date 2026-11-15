/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    MessageEngine.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 01/29/2009 @ 15:19:25 - nwilliam
**  \brief   Message manager, dispatcher, router, etc.
*******************************************************************************/

#ifndef SF_MESSAGEENGINE_H
#define SF_MESSAGEENGINE_H

#include <map>
#include <queue>
#include <vector>

#include "BaseEngine.h"
#include "Entity.h"
#include "Message.h"

namespace SF {
namespace Engine
{

class MessageEngine : public BaseEngine
{
public:
  MessageEngine();

  Message *AddMessage(const std::string& type, BaseEngine *engine, float delay = 0.0f );
  Message *AddMessage(const std::string& type, float delay = 0.0f, Message::ROUTE route = Message::MSG_LOCAL );
  Message *AddMessage(const std::string& type, EntityID entity, float delay = 0.0f, Message::ROUTE route = Message::MSG_LOCAL );
  
  void AddSubscription(const std::string& type, EntityID entity);
  void RemoveSubscription(const std::string& type, EntityID entity);
  
  void AddSubscription(const std::string& type, BaseEngine *engine);
  void RemoveSubscription(const std::string& type, BaseEngine *engine);

  void Update(float elapsed);

private:
  // Includes message delivery information
  struct FullMessage
  {
    enum Type { GLOBAL, ENGINE, ENTITY };

    bool operator < (const FullMessage& rhs) const { return time < rhs.time; }

    Type type;
    double time;
    Message *msg;

    union
    {
      BaseEngine *engine;
      EntityID entity;
    } destination;
  };

  typedef std::priority_queue<FullMessage> MessageQueue;
  typedef std::deque<FullMessage> MessageList;
  typedef std::multimap<std::string, BaseEngine *> StringEngineMap;
  typedef std::multimap<std::string, EntityID> StringEntityMap;

  void DispatchMessage(const FullMessage& message);
  void DispatchGlobalMessage(const FullMessage& message);

  MessageList     m_messageQueue;
  double          m_currentTime;

  StringEntityMap m_entitySubscribers;
  StringEngineMap m_engineSubscribers;
};

}

}

#endif
