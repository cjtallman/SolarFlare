/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    NetworkMessages.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/18/2009 @ 00:34:10 - nwilliam
**  \brief   Network specific messages
*******************************************************************************/

#ifndef SF_NETWORK_MESSAGES_H
#define SF_NETWORK_MESSAGES_H

#include <string>
#include "ByteStream.h"
#include "Engine/Entity.h"

// Forward declaration
namespace SF { namespace Engine { struct Message; } }

namespace SF {
namespace Networking {
namespace Messages
{
  enum Types { PING,
               PONG,
               REQUEST_ID,
               SERVER_BROADCAST,
               CONNECT_REQUEST,
               CONNECT_ACCEPTED,
               POSITION_UPDATE,
               UPDATE_GAME_PEER,
               GAME_MESSAGE
             };

  struct PacketHeader
  {
    PacketHeader();
    PacketHeader( uint32 _marker, uint8 _typeTag, uint16 _peerID );

    uint32 marker;
    uint8 typeTag;
    uint16 peerID;
  };

  ByteStream& operator>>( ByteStream& bs, PacketHeader& ph );
  ByteStream& operator<<( ByteStream& bs, const PacketHeader& ph );


  struct UDPHeader
  {
    UDPHeader();
    UDPHeader( uint16 _sequenceID );

    uint16 sequenceID;
  };

  ByteStream& operator>>( ByteStream& bs, UDPHeader& hdr );
  ByteStream& operator<<( ByteStream& bs, const UDPHeader& hdr );


  struct RUDPHeader
  {
    RUDPHeader();
    RUDPHeader( uint16 _sequenceID, uint16 _remoteAcked );

    uint16 sequenceID;
    uint16 remoteAcked;
  };

  ByteStream& operator>>( ByteStream& bs, RUDPHeader& hdr );
  ByteStream& operator<<( ByteStream& bs, const RUDPHeader& hdr );


  struct Ping
  {
    Ping();
    Ping( float _sendTime );

    float sendTime;
    static const uint8 type = Messages::PING;
  };

  ByteStream& operator>>( ByteStream& bs, Ping& msg );
  ByteStream& operator<<( ByteStream& bs, const Ping& msg );


  struct Pong
  {
    Pong();
    Pong( const Ping& ping );

    float sentTime;
    static const uint8 type = Messages::PONG;
  };

  ByteStream& operator>>( ByteStream& bs, Pong& msg );
  ByteStream& operator<<( ByteStream& bs, const Pong& msg );


  struct RequestID
  {
    RequestID();
    RequestID( uint16 _id );

    uint16 id;
    static const uint8 type = Messages::REQUEST_ID;
  };

  ByteStream& operator>>( ByteStream& bs, RequestID& msg );
  ByteStream& operator<<( ByteStream& bs, const RequestID& msg );


  struct ServerBroadcast
  {
    ServerBroadcast();
    ServerBroadcast( uint16 _id, std::string _serverName );

    uint16 id;
    std::string serverName;
    static const uint8 type = Messages::SERVER_BROADCAST;
  };

  ByteStream& operator>>( ByteStream& bs, ServerBroadcast& msg );
  ByteStream& operator<<( ByteStream& bs, const ServerBroadcast& msg );


  struct ConnectRequest
  {
    ConnectRequest();
    ConnectRequest( uint16 _peerID, const std::string& _peerName );

    uint16 peerID;
    std::string peerName;
    static const uint8 type = Messages::CONNECT_REQUEST;
  };

  ByteStream& operator>>( ByteStream& bs, ConnectRequest& msg );
  ByteStream& operator<<( ByteStream& bs, const ConnectRequest& msg );


  struct ConnectAccepted
  {
    ConnectAccepted();
    ConnectAccepted( int32 _eidLower, uint16 _serverID );

    int32 eidLower;
    uint16 serverID;
    static const uint8 type = Messages::CONNECT_ACCEPTED;
  };

  ByteStream& operator>>( ByteStream& bs, ConnectAccepted& msg );
  ByteStream& operator<<( ByteStream& bs, const ConnectAccepted& msg );


  struct PositionUpdate
  {
    PositionUpdate();
    PositionUpdate( Engine::Entity* e );

    Engine::EntityID eid;
    static const uint8 type = Messages::POSITION_UPDATE;
  };

  ByteStream& operator>>( ByteStream& bs, PositionUpdate& msg );
  ByteStream& operator<<( ByteStream& bs, const PositionUpdate& msg );


  struct UpdateGamePeer
  {
    static const uint8 UPDATE = 0;
    static const uint8 REMOVE = 1;

    UpdateGamePeer();
    UpdateGamePeer( uint8 _action, uint16 _peerID, const std::string& _name );

    uint8 action;
    uint16 peerID;
    std::string name;
    static const uint8 type = Messages::UPDATE_GAME_PEER;
  };

  ByteStream& operator>>( ByteStream& bs, UpdateGamePeer& msg );
  ByteStream& operator<<( ByteStream& bs, const UpdateGamePeer& msg );


  struct GameMessage
  {
    GameMessage( const Engine::Message* _msg );

    const Engine::Message* msg;
    static const uint8 type = Messages::GAME_MESSAGE;
  };

  ByteStream& operator>>( ByteStream& bs, GameMessage& msg );
  ByteStream& operator<<( ByteStream& bs, const GameMessage& msg );

} // Messages namespace
} // Networking namespace
} // SF namespace

#endif // SF_NETWORK_MESSAGES_H
