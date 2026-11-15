/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    NetworkEngine.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/20/2009 @ 14:28:33 - nwilliam
**  \brief   Network engine.
*******************************************************************************/

#ifndef SF_NETWORK_ENGINE_H
#define SF_NETWORK_ENGINE_H

#include <map>
#include "RUDP.h"
#include "Engine/Entity.h"
#include "Engine/BaseEngine.h"
#include "External/Lua/include/LunarHelper.h"

namespace SF {
namespace Networking
{
  struct ServerInfo
  {
    ServerInfo();
    
    uint16 id;
    Address addr;
    float timeRecv;
    std::string name;
  };


  class NetworkEngine : public Engine::BaseEngine
  {
    public:
      enum Modes { IDLE, FIND_SERVER, CONNECTING, CLIENT, SERVER };

      NetworkEngine();
      virtual ~NetworkEngine();

      virtual void Update( float elapsed );
      virtual void HandleMessage( const Engine::Message* msg );

      void Mode( Modes mode );
      void Connect( uint16 id, const Address& addr );

      void AddNetworkEntity( Engine::EntityID id );
      void RemoveNetworkEntity( Engine::EntityID id );

    private:
      typedef std::map< uint16, ServerInfo > ServerMap;
      typedef std::map< uint16, std::string > StringMap;

      bool DoClientJoin();
      bool DoUpdate( float elapsed );
      bool DoDisconnect( float elapsed );
      bool DoKeepAlive( float elapsed );
      bool DoServerBroadcast( float elasped );

      void UpdateClient( float elapsed );
      void UpdateServer( float elapsed );
      void UpdateFindServer( float elapsed );
      void UpdateConnecting( float elapsed );

      void InitData();
      void ResetPeerData();
      void ResetTimingData();
      void RemovePeer( uint16 peerID );
      void AddPeer( uint16 peerID, const std::string& name, const Address& addr );

      void BroadcastServer();
      void SendPositionUpdates();
      void SyncGameToPeer( RPeer& peer );

      void HandleGamePeerUpdate();
      void HandlePing( RPeer& peer );
      void HandlePong( RPeer& peer );
      void HandleGameMessage( RPeer& peer );
      void HandlePositionUpdate( RPeer& peer );
      void HandleConnectRequest( const Address& from );

      // Always used
      uint16 m_networkID;
      StringMap m_peerNames;
      std::string m_clientName;

      // Find Server
      ServerMap m_servers;

      // Server mode
      std::string m_serverName;

      // Engine related
      RUDP m_rudp;
      Modes m_mode;
      ByteStream m_recvBuffer;
      ByteStream m_scratchBuffer;
      Engine::EntityIDList m_entities;

      // Settings
      Engine::EntityID m_eidRange;
      Engine::EntityID m_eidOffset;

      size_t m_maxClients;
      bool m_allowConnect;
      uint16 m_serverPort;
      float m_timeoutDelay;
      float m_updateRate[2];
      float m_keepAliveDelay[2];
      float m_serverBroadcastDelay[2];



      LUA_DECLARE_CLASS( NetworkEngine )
      LUA_DECLARE_METHOD( LID )
      LUA_DECLARE_METHOD( LMode )
      LUA_DECLARE_METHOD( LSettings )
      LUA_DECLARE_METHOD( LPeerName )
      LUA_DECLARE_METHOD( LPeerInfo )
      LUA_DECLARE_METHOD( LConnectIP )
      LUA_DECLARE_METHOD( LConnectID )
      LUA_DECLARE_METHOD( LServerName )
      LUA_DECLARE_METHOD( LClientName )
      LUA_DECLARE_METHOD( LServerInfo )
      LUA_DECLARE_METHOD( LServerListLAN )
  };
  
} // Networking namespace
} // SF namespace

#endif // SF_NETWORK_ENGINE_H
