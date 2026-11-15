/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    NetworkEngine.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/09/2009 @ 12:57:08 - nwilliam
**  \brief   Network engine.
*******************************************************************************/

#include "CNetwork.h"
#include "NetworkEngine.h"
#include "NetworkMessages.h"
#include "Utilities/Timer.h"
#include "SolarFlare/Globals.h"     // console, entityFactory, messageEngine, rRandom, timer
#include "Engine/EntityFactory.h"
#include "Utilities/DevConsole.h"
#include "Engine/MessageEngine.h"
#include "Utilities/Random/SNCG.h"

namespace
{
  // SHA-256 "Solare Flare" (no quotes)
  const uint32 MARKER_V1 = 0x11F48149;
  const uint32 MARKER_V2 = 0x0DEBFA46;
  const uint32 MARKER_V3 = 0xAC1BAFD3;
  const uint32 MARKER_V4 = 0x0AB29C44;
  const uint32 MARKER_V5 = 0x83225ED5;
  const uint32 MARKER_V6 = 0x98F9CBE1;
  const uint32 MARKER_V7 = 0x1479C5D0;
  const uint32 MARKER_V8 = 0x121A8694;

  const uint32 MARKER = MARKER_V1;

  const char* ModeNames[] = { "Idle",
                              "FindServer",
                              "Connecting",
                              "Client",
                              "Server",
                              0
                            };
}


namespace SF {
namespace Networking
{
  //////////////////////////////////////////////////////////////////////////////
  // ServerInfo
  //////////////////////////////////////////////////////////////////////////////
  ServerInfo::ServerInfo() : id( 0 ),
                             timeRecv( 0.0f )
  {
  }

  //////////////////////////////////////////////////////////////////////////////
  // Constructor / destructor
  //////////////////////////////////////////////////////////////////////////////
  NetworkEngine::NetworkEngine()
  {
    using namespace Engine;

    Globals::entityFactory->CreateNotifier( ComponentType::NETWORK,
                                            Notifier::Method< NetworkEngine, &NetworkEngine::AddNetworkEntity >( this ) );

    Globals::entityFactory->DeleteNotifier( ComponentType::NETWORK,
                                            Notifier::Method< NetworkEngine, &NetworkEngine::RemoveNetworkEntity >( this ) );

    Globals::entityFactory->Creator( "NETWORK", CCreator::Function< CNetwork::Creator >() );

    InitData();
    Socket::Initialize();
    Mode( NetworkEngine::IDLE );
  }

  NetworkEngine::~NetworkEngine()
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void NetworkEngine::Update( float elapsed )
  {
    switch( m_mode )
    {
      case CLIENT:      UpdateClient( elapsed );      break;
      case SERVER:      UpdateServer( elapsed );      break;
      case CONNECTING:  UpdateConnecting( elapsed );  break;
      case FIND_SERVER: UpdateFindServer( elapsed );  break;
    }
  }


  void NetworkEngine::HandleMessage( const Engine::Message* msg )
  {
    if( m_mode != NetworkEngine::IDLE )
    {
      m_scratchBuffer.Clear();
      m_scratchBuffer << Messages::GameMessage( msg );

      bool initial = false;
      Engine::Entity* entity = 0;

      if( msg->type.compare( "EntityCreated" ) == 0 )
      {
        initial = true;
        Engine::EntityID id = msg->stack.Get<int>(1);
        entity = Globals::entityFactory->GetEntity( id );
      }
      else if( msg->type.compare( "SyncEntity" ) == 0 )
      {
        initial = false;
        Engine::EntityID id = msg->stack.Get<int>(0);
        entity = Globals::entityFactory->GetEntity( id );
      }

      if( entity )
      {
        entity->InitOut( m_scratchBuffer, initial );
      }

      if( msg->route == Engine::Message::MSG_SINGLE_PEER )
      {
        uint16 peerID = static_cast<uint16>( msg->stack.Get<int>( 0 ) );
        RPeer* peer = m_rudp.Peer( peerID );

        if( peer )
        {
          peer->SendTo( m_scratchBuffer );
        }
      }
      else
      {
        m_rudp.StreamReliable( m_scratchBuffer );
      }
    }
  }

  void NetworkEngine::Mode( NetworkEngine::Modes mode )
  {
    if( mode != m_mode )
    {
      m_mode = mode;
      
      ResetPeerData();
      ResetTimingData();

      // Only way to get to client is to go from connecting, and we don't want
      // to get rid of RUDP peers, packet caches, etc
      if( mode != NetworkEngine::CLIENT )
      {
        m_rudp.Reset();
      }

      Globals::console->Debug() << "Switching to network mode: "
                                << ModeNames[m_mode] << std::endl;

      m_rudp.BroadcastState( false );

      switch( mode )
      {
        case IDLE:
          m_rudp.SetInfo( 0, 0 );
        break;

        case FIND_SERVER:
          m_rudp.BroadcastState( true );
          m_rudp.SetInfo( m_serverPort, MARKER_V1 );
          m_networkID = static_cast<uint16>( Globals::rRandom->RandInt() );
        break;

        case SERVER:
          m_rudp.SetInfo( m_serverPort, MARKER_V1 );
          m_networkID = static_cast<uint16>( Globals::rRandom->RandInt() );
        break;
      }
    }

    // TODO: Change static packet header so this isn't needed
    RPeer::packetHeader = Messages::PacketHeader( MARKER, RUDP::RUDP_INFO, m_networkID );
  }

  void NetworkEngine::Connect( uint16 id, const Address& addr )
  {
    Mode( CONNECTING );

    // Connect request sending based on keep alive, do one immediately
    m_keepAliveDelay[0] = m_keepAliveDelay[1];

    ServerInfo server;
    server.addr = addr;
    m_servers[id] = server;
  }

  void NetworkEngine::AddNetworkEntity( Engine::EntityID id )
  {
    m_entities.push_back( id );
  }

  void NetworkEngine::RemoveNetworkEntity( Engine::EntityID id )
  {
    m_entities.remove( id );
  }

  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  bool NetworkEngine::DoClientJoin()
  {
    return m_allowConnect && (m_rudp.Peers().size() < m_maxClients);
  }

  bool NetworkEngine::DoUpdate( float elapsed )
  {
    m_updateRate[0] += elapsed;

    if( m_updateRate[0] > m_updateRate[1] )
    {
      m_updateRate[0] = 0.0f;
      return true;
    }

    return false;
  }

  bool NetworkEngine::DoDisconnect( float elapsed )
  {
    if( m_timeoutDelay > 0.0f )
    {
      return elapsed > m_timeoutDelay;
    }

    return false;
  }

  bool NetworkEngine::DoKeepAlive( float elapsed )
  {
    if( m_keepAliveDelay[1] > 0.0f )
    {
      m_keepAliveDelay[0] += elapsed;

      if( m_keepAliveDelay[0] > m_keepAliveDelay[1] )
      {
        m_keepAliveDelay[0] = 0.0f;
        return true;
      }
    }

    return false;
  }

  bool NetworkEngine::DoServerBroadcast( float elapsed )
  {
    if( m_serverBroadcastDelay[1] > 0.0f )
    {
      m_serverBroadcastDelay[0] += elapsed;
      
      if( m_serverBroadcastDelay[0] >= m_serverBroadcastDelay[1] )
      {
        m_serverBroadcastDelay[0] = 0.0f;
        return true;
      }
    }

    return false;
  }

  void NetworkEngine::InitData()
  {
    m_mode = NetworkEngine::IDLE;

    m_serverName = "Server";
    m_clientName = "Client";

    m_eidRange = 10000000;        // local entities always get first range
    m_eidOffset = m_eidRange * 2; // localally created network get 2nd (if they end up hosting)
    Globals::entityFactory->Reset( m_eidRange );

    m_maxClients = 7;
    m_allowConnect = true;
    m_serverPort = 8003;
    m_timeoutDelay = 10000.0f;
    m_updateRate[0] = 0.0f;
    m_updateRate[1] = 25.0f;
    m_keepAliveDelay[0] = 0.0f;
    m_keepAliveDelay[1] = 2500.0f;
    m_serverBroadcastDelay[0] = 0.0f;
    m_serverBroadcastDelay[1] = 1000.0f;

    m_networkID = 0;
  }

  void NetworkEngine::ResetPeerData()
  {
    m_servers.clear();
    m_peerNames.clear();
  }

  void NetworkEngine::ResetTimingData()
  {
    m_updateRate[0] = 0.0f;
    m_keepAliveDelay[0] = 0.0f;
    m_serverBroadcastDelay[0] = 0.0f;
  }

  void NetworkEngine::RemovePeer( uint16 peerID )
  {
    m_rudp.RemovePeer( peerID );
    m_peerNames.erase( peerID );
  }

  void NetworkEngine::AddPeer( uint16 peerID, const std::string& name, const Address& addr )
  {
    if( peerID == m_networkID )
      return;

    StringMap::iterator it = m_peerNames.find( peerID );

    if( it == m_peerNames.end() )
    {
      m_peerNames[peerID] = name;

      if( m_mode == NetworkEngine::SERVER )
        m_rudp.UpdatePeer( peerID, addr );
    }
    else
    {
      it->second = name;
    }
  }

  void NetworkEngine::UpdateClient( float elapsed )
  {
    if( DoUpdate( elapsed ) )
      m_rudp.Update();

    RPeer* peer;
    Address from;

    while( m_rudp.Recieve( m_recvBuffer, from, peer ) != RUDP::RECV_NONE )
    {
      while( m_recvBuffer.Readable() )
      {
        using namespace Messages;

        uint8 msgType;
        m_recvBuffer >> msgType;

        switch( msgType )
        {
          case PING:              HandlePing( *peer );            break;
          case PONG:              HandlePong( *peer );            break;
          case GAME_MESSAGE:      HandleGameMessage( *peer );     break;
          case POSITION_UPDATE:   HandlePositionUpdate( *peer );  break;
          case UPDATE_GAME_PEER:  HandleGamePeerUpdate();         break;

          //case SERVER_BROADCAST:
          //case CONNECT_REQUEST:
          //case CONNECT_ACCEPTED:
          default:
            if( peer )
            {
              peer->stats.Track( PeerStats::DROPPED_RECV, m_recvBuffer.Size() );
            }

            m_recvBuffer.Clear();
        }
      }
    }

    if( DoKeepAlive( elapsed ) )
    {
      Messages::Ping ping( Globals::timer->ElapsedSinceStart() );
      m_rudp.StreamReliable( ping );
    }

    m_rudp.Peers().back().comAccum += elapsed;

    if( DoDisconnect( m_rudp.Peers().back().comAccum ) )
    {
      std::string err( "Disconnecting from server, timed out." );
      Globals::console->Error( err );

      Engine::Message* msg = Globals::messageEngine->AddMessage( "ServerDisconnect", 0.0f, Engine::Message::MSG_LOCAL );
      msg->stack.Push( std::string("TIMEOUT") );

      Mode( NetworkEngine::IDLE );
    }
  }

  void NetworkEngine::UpdateServer( float elapsed )
  {
    if( DoClientJoin() && DoServerBroadcast( elapsed ) )
    {
      BroadcastServer();
    }

    if( DoUpdate( elapsed ) )
    {
      SendPositionUpdates();
      m_rudp.Update();
    }

    // Receive data
    RPeer* peer;
    Address from;

    while( m_rudp.Recieve( m_recvBuffer, from, peer ) != RUDP::RECV_NONE )
    {
      while( m_recvBuffer.Readable() )
      {
        using namespace Messages;

        uint8 msgType;
        m_recvBuffer >> msgType;

        switch( msgType )
        {
          case PING:            HandlePing( *peer );          break;
          case PONG:            HandlePong( *peer );          break;
          case GAME_MESSAGE:    HandleGameMessage( *peer );   break;
          case CONNECT_REQUEST: HandleConnectRequest( from ); break;

          case REQUEST_ID:
          {
            RequestID req;  m_recvBuffer >> req;
            m_rudp.TempBuffer() << RequestID( m_networkID );
            m_rudp.SendTemp( from );
          }
          break;

          //SERVER_BROADCAST
          //CONNECT_ACCEPTED
          //POSITION_UPDATE
          //UPDATE_GAME_PEER
          default:
            if( peer )
            {
              peer->stats.Track( PeerStats::DROPPED_RECV, m_recvBuffer.Size() );
            }

            m_recvBuffer.Clear();
        }
      }
    }

    if( DoKeepAlive( elapsed ) )
    {
      Messages::Ping ping( Globals::timer->ElapsedSinceStart() );
      m_rudp.StreamReliable( ping );
    }

    // Check for clients to drop
    if( m_timeoutDelay > 0.0f )
    {
      RPeerList& peers = m_rudp.Peers();

      for( RPeerList::iterator it = peers.begin(); it != peers.end(); ++it )
      {
        it->comAccum += elapsed;

        if( DoDisconnect( it->comAccum ) )
        {
          Globals::console->Debug() << "Removing timed out peer: "
                                    << it->addr.ToString() << std::endl;

          Engine::Message* msg = Globals::messageEngine->AddMessage( "PeerRemoved" );
          msg->stack.Push( static_cast<int>( it->id ) );
          msg->stack.Push( std::string("TIMEOUT") );

          Messages::UpdateGamePeer ugp( Messages::UpdateGamePeer::REMOVE, it->id, std::string() );
          m_rudp.StreamReliable( ugp );

          RPeerList::iterator temp = it--;
          RemovePeer( temp->id );
        }
      }
    }
  }

  void NetworkEngine::UpdateFindServer( float elapsed )
  {
    RPeer* peer;
    Address from;

    while( m_rudp.Recieve( m_recvBuffer, from, peer ) != RUDP::RECV_NONE )
    {
      uint8 msgType;
      m_recvBuffer >> msgType;

      if( msgType == Messages::SERVER_BROADCAST )
      {
        Messages::ServerBroadcast sb;

        m_recvBuffer >> sb;

        m_servers[sb.id].id = sb.id;
        m_servers[sb.id].addr = from;
        m_servers[sb.id].name = sb.serverName;
        m_servers[sb.id].timeRecv = 0.0f;
      }
    }

    for( ServerMap::iterator it = m_servers.begin(); it != m_servers.end(); ++it )
    {
      it->second.timeRecv += elapsed;

      if( it->second.timeRecv > 5000.0f )
      {
        m_servers.erase( it );
        break;
      }
    }
  }

  void NetworkEngine::UpdateConnecting( float elapsed )
  {
    RPeer* peer;
    Address from;

    while( m_rudp.Recieve( m_recvBuffer, from, peer ) != RUDP::RECV_NONE )
    {
      using namespace Messages;

      uint8 msgType;
      m_recvBuffer >> msgType;

      if( msgType == CONNECT_ACCEPTED )
      {
        ConnectAccepted ca;
        m_recvBuffer >> ca;

        ServerMap::iterator it = m_servers.find( ca.serverID );

        if( it == m_servers.end() )
          break;  // Unknown server

        Globals::entityFactory->Reset( ca.eidLower );

        Mode( NetworkEngine::CLIENT );

        Globals::console->Debug() << "ConnectAccepted received from " 
                                  << from.ToString() << std::endl;

        // Finish processing recvBuffer
        while( m_recvBuffer.Readable() )
        {
          m_recvBuffer >> msgType;

          switch( msgType )
          {
            case PING:              HandlePing( *peer );        break;
            case PONG:              HandlePong( *peer );        break;
            case GAME_MESSAGE:      HandleGameMessage( *peer ); break;
            case UPDATE_GAME_PEER:  HandleGamePeerUpdate();     break;

            //SERVER_BROADCAST
            //CONNECT_REQUEST
            //CONNECT_ACCEPTED
            //POSITION_UPDATE
            default:
              if( peer )
              {
                peer->stats.Track( PeerStats::DROPPED_RECV, m_recvBuffer.Size() );
              }

              m_recvBuffer.Clear();
          }
        }

        return;
      }
      else if( msgType == REQUEST_ID )
      {
        if( m_servers[0].addr == from )
        {
          Messages::RequestID req;
          m_recvBuffer >> req;

          m_rudp.UpdatePeer( req.id, from );
        }
      }
    }

    // Send connect request
    if( DoKeepAlive( elapsed ) )
    {
      ByteStream& bs = m_rudp.TempBuffer();

      if( m_rudp.Peers().size() == 0 )
      {
        bs << Messages::RequestID();
        m_rudp.SendTemp( m_servers[0].addr );
      }
      else
      {
        bs << Messages::ConnectRequest( m_networkID, m_clientName );
        m_rudp.SendTemp( m_rudp.Peers().back().addr );
      }
    }
  }

  void NetworkEngine::BroadcastServer()
  {
    m_rudp.BroadcastState( true );

    ByteStream& temp = m_rudp.TempBuffer();
    temp << Messages::ServerBroadcast( m_networkID, m_serverName );
    m_rudp.Broadcast( temp );

    m_rudp.BroadcastState( false );
  }

  void NetworkEngine::SendPositionUpdates()
  {
    for( Engine::EntityIDList::iterator it = m_entities.begin(); it != m_entities.end(); ++it )
    {
      Engine::Entity* e = Globals::entityFactory->GetEntity( *it );

      if( e )
      {
        m_scratchBuffer.Clear();

        m_scratchBuffer << Messages::PositionUpdate( e );
        e->DeltaOut( m_scratchBuffer );

        // TODO: Consider precomputing an entities stream out size

        m_rudp.StreamUnreliable( m_scratchBuffer );
      }
    }
  }

  void NetworkEngine::SyncGameToPeer( RPeer& peer )
  {
    // Send all internal network info (peerIDs, peerNames)
    // DOES involve adding 'self' to name table and then removing (simplifies loop)
    m_peerNames[m_networkID] = m_clientName;
    m_scratchBuffer.Clear();

    for( StringMap::iterator it = m_peerNames.begin(); it != m_peerNames.end(); ++it )
    {
      Messages::UpdateGamePeer ugp( Messages::UpdateGamePeer::UPDATE, it->first, it->second );
      m_scratchBuffer << ugp;
    }
    peer.SendTo( m_scratchBuffer );
    m_peerNames.erase( m_networkID );

    // Sync out all entities as new 'EntityCreated' messages
    Engine::Message msg;
    msg.type = "EntityCreated";

    for( Engine::EntityIDList::iterator it = m_entities.begin(); it != m_entities.end(); ++it )
    {
      Engine::Entity* entity = Globals::entityFactory->GetEntity( *it );

      if( entity )
      {
        m_scratchBuffer.Clear();
        msg.stack.Push( entity->DefType() );
        msg.stack.Push( *it );

        m_scratchBuffer << Messages::GameMessage( &msg );
        entity->InitOut( m_scratchBuffer, true );

        peer.SendTo( m_scratchBuffer );

        msg.stack.Pop();
        msg.stack.Pop();
      }
    }
  }

  void NetworkEngine::HandleGamePeerUpdate()
  {
    Messages::UpdateGamePeer ugp;
    m_recvBuffer >> ugp;

    if( ugp.action == Messages::UpdateGamePeer::UPDATE )
      AddPeer( ugp.peerID, ugp.name, Address() );
    else if( ugp.action == Messages::UpdateGamePeer::REMOVE )
      RemovePeer( ugp.peerID );
  }

  void NetworkEngine::HandlePing( RPeer& peer )
  {
    Messages::Ping ping;
    m_recvBuffer >> ping;
    peer.SendTo( Messages::Pong( ping ) );
  }

  void NetworkEngine::HandlePong( RPeer& peer )
  {
    Messages::Pong pong;
    m_recvBuffer >> pong;

    // Round trip latency, enough for now
    float halfDiff = Globals::timer->ElapsedSinceStart() - pong.sentTime;
    halfDiff /= 2;

    peer.stats.latencyTo = halfDiff;
    peer.stats.latencyFrom = halfDiff;
  }

  void NetworkEngine::HandleGameMessage( RPeer& peer )
  {
    Engine::Message* msg = Globals::messageEngine->AddMessage( std::string() );
    msg->route = Engine::Message::MSG_LOCAL;

    m_recvBuffer >> *msg;

    bool initial = false;
    bool doEntity = false;
    Engine::Entity* entity = 0;

    if( msg->type.compare( "EntityCreated" ) == 0 )
    {
      initial = true;
      doEntity = true;

      if( msg->stack.Size() == 2 )
      {
        std::string defName = msg->stack.Get<std::string>( 0 );
        Engine::EntityID id = msg->stack.Get<int>( 1 );
        entity = Globals::entityFactory->CreateID( defName, id, false );
      }
    }
    else if( msg->type.compare( "SyncEntity" ) == 0 )
    {
      initial = false;
      doEntity = true;

      if( msg->stack.Size() == 1 )
      {
        Engine::EntityID id = msg->stack.Get<int>( 0 );
        entity = Globals::entityFactory->GetEntity( id );
      }
    }

    if( doEntity )
    {
      if( entity )
      {
        entity->InitIn( m_recvBuffer, initial );
      }
      else
      {
        // Bad / unknown
        peer.stats.Track( PeerStats::DROPPED_RECV, m_recvBuffer.Size() );
        m_recvBuffer.Clear();
        return;
      }
    }

    // Propogate to peers
    if( m_mode == NetworkEngine::SERVER )
    {
      m_scratchBuffer.Clear();
      m_scratchBuffer << Messages::GameMessage( msg );

      if( entity )
      {
        entity->InitOut( m_scratchBuffer, initial );
      }

      RPeerList& peers = m_rudp.Peers();

      for( RPeerList::iterator it = peers.begin(); it != peers.end(); ++it )
      {
        if( it->id != peer.id )
        {
          it->SendTo( m_scratchBuffer );
        }
      }
    }
  }

  void NetworkEngine::HandlePositionUpdate( RPeer& peer )
  {
    Messages::PositionUpdate pu;
    m_recvBuffer >> pu;

    Engine::Entity* e = Globals::entityFactory->GetEntity( pu.eid );

    if( e )
    {
      e->DeltaIn( m_recvBuffer );
    }
    else
    {
      peer.stats.Track( PeerStats::DROPPED_RECV, m_recvBuffer.Size() );
      m_recvBuffer.Clear();
    }
  }

  void NetworkEngine::HandleConnectRequest( const Address& from )
  {
    using namespace Messages;

    Messages::ConnectRequest cr;
    m_recvBuffer >> cr;

    RPeer* peer = m_rudp.Peer( cr.peerID );

    if( peer == 0 )
    {
      if( DoClientJoin() )
      {
        ConnectAccepted ca( m_eidOffset, m_networkID );
        m_eidOffset += m_eidRange;

        m_rudp.UpdatePeer( cr.peerID, from );

        m_scratchBuffer.Clear();
        m_scratchBuffer << ca;
        m_rudp.Peers().back().SendTo( m_scratchBuffer );

        SyncGameToPeer( m_rudp.Peers().back() );
        m_peerNames[cr.peerID] = cr.peerName;

        UpdateGamePeer ugp( UpdateGamePeer::UPDATE, cr.peerID, cr.peerName );
        m_rudp.StreamReliable( ugp );

        Engine::Message* msg = Globals::messageEngine->AddMessage( "PeerAdded" );
        msg->stack.Push( static_cast<int>(cr.peerID) );

        Globals::console->Debug() << "ConnectAccepted sent to " 
                                  << from.ToString() << std::endl;
      }
    }
    else
    {
      Globals::console->Warning() << "Connect request received from duplicate peer ID: "
                                  << cr.peerID << std::endl;
      
    }
  }


  //////////////////////////////////////////////////////////////////////////////
  // Lua methods
  //////////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( NetworkEngine )
    LUA_NAME_METHOD( NetworkEngine, "ID",            LID )
    LUA_NAME_METHOD( NetworkEngine, "Mode",          LMode )
    LUA_NAME_METHOD( NetworkEngine, "Settings",      LSettings )
    LUA_NAME_METHOD( NetworkEngine, "PeerName",      LPeerName )
    LUA_NAME_METHOD( NetworkEngine, "PeerInfo",      LPeerInfo )
    LUA_NAME_METHOD( NetworkEngine, "ConnectID",     LConnectID )
    LUA_NAME_METHOD( NetworkEngine, "ConnectIP",     LConnectIP )
    LUA_NAME_METHOD( NetworkEngine, "ServerName",    LServerName )
    LUA_NAME_METHOD( NetworkEngine, "ClientName",    LClientName )
    LUA_NAME_METHOD( NetworkEngine, "ServerInfo",    LServerInfo )
    LUA_NAME_METHOD( NetworkEngine, "ServerListLAN", LServerListLAN )
  LUA_END_DEFINE_CLASS


  LUA_DEFINE_METHOD( NetworkEngine, LID )
  {
    lua_pushnumber( ls, m_networkID );
    return 1;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LMode )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushstring( ls, ModeNames[ m_mode ] );
      return 1;
    }

    int opt = luaL_checkoption( ls, 1, 0, ModeNames );

    Mode( static_cast<NetworkEngine::Modes>( opt ) );
    return 0;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LSettings )
  {
    static const char* names[] = { "TimeoutDelay", "UpdateRate",
                                   "KeepAliveDelay", "ServerBroadcastDelay",
                                   "AllowConnect", "MaxClients", 0 };

    float* vals[] = { &m_timeoutDelay, &m_updateRate[1],
                      &m_keepAliveDelay[1], &m_serverBroadcastDelay[1] };


    int top = lua_gettop( ls );

    if( top == 0 )
    {
      lua_newtable( ls );

      for( int i = 0; names[i]; ++i )
      {
        lua_pushinteger( ls, i+1 );
        lua_pushstring( ls, names[i] );
        lua_settable( ls, -3 );
      }

      return 1;
    }
    else if( top == 1 )
    {
      int opt = luaL_checkoption( ls, 1, 0, names );

      if( opt <= 3 )
        lua_pushnumber( ls, *vals[opt] );
      else if( opt == 4 )
        lua_pushboolean( ls, m_allowConnect );
      else
        lua_pushinteger( ls, m_maxClients );

      return 1;
    }
    else if( top == 2 )
    {
      int opt = luaL_checkoption( ls, 1, 0, names );
      
      if( opt <= 3 )
        *vals[opt] = luaL_checkfloat( ls, 2 );
      else if( opt == 4 )
        m_allowConnect = (lua_toboolean( ls, 2 ) == 1);
      else
        m_maxClients = lua_tointeger( ls, 2 );
    }

    return 0;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LPeerName )
  {
    uint16 peerID = static_cast<uint16>( luaL_checkint( ls, 1 ) );

    if( peerID == m_networkID )
    {
      lua_pushlstring( ls, m_clientName.c_str(), m_clientName.length() );
    }
    else
    {
      StringMap::iterator it = m_peerNames.find( peerID );

      if( it != m_peerNames.end() )
        lua_pushlstring( ls, it->second.c_str(), it->second.length() );
      else
        lua_pushnil( ls );
    }

    return 1;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LPeerInfo )
  {
    lua_createtable( ls, static_cast<int>( m_peerNames.size() ), 0 );

    int index = 1;

    for( StringMap::iterator it = m_peerNames.begin(); it != m_peerNames.end(); ++it )
    {
      lua_pushnumber( ls, index++ );
      lua_createtable( ls, 0, 2 );

      lua_pushstring( ls, "ID" );
      lua_pushnumber( ls, it->first );
      lua_settable( ls, -3 );

      lua_pushstring( ls, "Name" );
      lua_pushlstring( ls, it->second.c_str(), it->second.length() );
      lua_settable( ls, -3 );

      lua_settable( ls, -3 );
    }

    // Table of tables left on stack:
    //  table = { { "ID" = id_1, "Name" = name_1 }, ... }

    return 1;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LConnectID )
  {
    uint16 id = static_cast<uint16>( luaL_checkint( ls, 1 ) );
    ServerMap::iterator it = m_servers.find( id );

    bool goodServer = it != m_servers.end();
    lua_pushboolean( ls, goodServer );

    if( goodServer )
    {
      Address addr = it->second.addr;
      Connect( id, addr );
      m_rudp.UpdatePeer( id, addr );
    }

    return 1;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LConnectIP )
  {
    uint16 port = 8003;
    const char* str = luaL_checkstring( ls, 1 );

    if( lua_gettop( ls ) == 2 )
    {
      port = static_cast<uint16>( luaL_checkint( ls , 2 ) );
    }

    Address addr( str, port );
    Connect( 0, addr );

    return 0;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LServerName )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushlstring( ls, m_serverName.c_str(), m_serverName.length() );
      return 1;
    }

    const char* str = luaL_checkstring( ls, -1 );
    m_serverName = str;

    return 0;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LClientName )
  {
    if( lua_gettop( ls ) == 0 )
    {
      lua_pushlstring( ls, m_clientName.c_str(), m_clientName.length() );
      return 1;
    }

    const char* str = luaL_checkstring( ls, -1 );
    m_clientName = str;

    return 0;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LServerInfo )
  {
    if( m_mode == NetworkEngine::CLIENT )
    {
      RPeer& peer = m_rudp.Peers().front();

      lua_createtable( ls, 0, 3 );

      lua_pushliteral( ls, "ID" );
      lua_pushinteger( ls, peer.id );
      lua_settable( ls, -3 );

      lua_pushliteral( ls, "Address" );
      lua_pushstring( ls, peer.addr.ToString() );
      lua_settable( ls, -3 );
    }
    else
    {
      lua_pushnil( ls );
    }

    return 1;
  }

  LUA_DEFINE_METHOD( NetworkEngine, LServerListLAN )
  {
    lua_createtable( ls, static_cast<int>( m_servers.size() ), 0 );

    int index = 1;
    ServerMap::iterator it = m_servers.begin();

    lua_createtable( ls, static_cast<int>( m_servers.size() ), 0 );

    while( it != m_servers.end() )
    {
      lua_pushinteger( ls, index++ );

      lua_createtable( ls, 0, 3 );

      lua_pushliteral( ls, "ID" );
      lua_pushinteger( ls, it->second.id );
      lua_settable( ls, -3 );

      lua_pushliteral( ls, "Name" );
      lua_pushstring( ls, it->second.name.c_str() );
      lua_settable( ls, -3 );

      lua_pushliteral( ls, "Address" );
      lua_pushstring( ls, it->second.addr.ToString() );
      lua_settable( ls, -3 );

      lua_settable( ls, -3 );

      ++it;
    }

    // Table of tables (containing name, address)

    return 1;
  }

} // Networking namespace
} // SF namespace
