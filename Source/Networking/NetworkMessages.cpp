/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    NetworkMessages.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/13/2009 @ 18:55:58 - nwilliam
**  \brief   Network specific messages
*******************************************************************************/

#include "NetworkMessages.h"

namespace
{
  uint8   UINT8_MAX = 1u << (sizeof(uint8) * 8 - 1);
  uint16 UINT16_MAX = 1u << (sizeof(uint16) * 8 - 1);
  uint32 UINT32_MAX = 1u << (sizeof(uint32) * 8 - 1);
}

namespace SF {
namespace Networking {
namespace Messages
{
  //////////////////////////////////////////////////////////////////////////////
  // PacketHeader
  //////////////////////////////////////////////////////////////////////////////
  PacketHeader::PacketHeader() : marker( UINT32_MAX ),
                                 typeTag( UINT8_MAX ),
                                 peerID( UINT16_MAX )
  {
  }

  PacketHeader::PacketHeader( uint32 _marker, uint8 _typeTag, uint16 _peerID ) : marker( _marker ),
                                                                                 typeTag( _typeTag ),
                                                                                 peerID( _peerID )
  {
  }

  ByteStream& operator>>( ByteStream& bs, PacketHeader& msg )
  {
    bs >> msg.marker >> msg.typeTag >> msg.peerID;
    return bs;
  }

  ByteStream& operator<<( ByteStream& bs, const PacketHeader& msg )
  {
    bs << msg.marker << msg.typeTag << msg.peerID;
    return bs;
  }


  //////////////////////////////////////////////////////////////////////////////
  // UDPHeader
  //////////////////////////////////////////////////////////////////////////////
  UDPHeader::UDPHeader() : sequenceID( UINT16_MAX )
  {
  }

  UDPHeader::UDPHeader( uint16 _sequenceID) : sequenceID( _sequenceID )
  {
  }

  ByteStream& operator>>( ByteStream& bs, UDPHeader& hdr )
  {
    bs >> hdr.sequenceID;
    return bs;
  }

  ByteStream& operator<<( ByteStream& bs, const UDPHeader& hdr )
  {
    bs << hdr.sequenceID;
    return bs;
  }


  //////////////////////////////////////////////////////////////////////////////
  // RUDPHeader
  //////////////////////////////////////////////////////////////////////////////
  RUDPHeader::RUDPHeader() : sequenceID( UINT16_MAX ),
                             remoteAcked( UINT16_MAX )
  {
  }

  RUDPHeader::RUDPHeader( uint16 _sequenceID, uint16 _remoteAcked ) : sequenceID( _sequenceID ),
                                                                      remoteAcked( _remoteAcked )
  {
  }

  ByteStream& operator>>( ByteStream& bs, RUDPHeader& hdr )
  {
    bs >> hdr.sequenceID >> hdr.remoteAcked;
    return bs;
  }

  ByteStream& operator<<( ByteStream& bs, const RUDPHeader& hdr )
  {
    bs << hdr.sequenceID << hdr.remoteAcked;
    return bs;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Ping Message
  //////////////////////////////////////////////////////////////////////////////
  Ping::Ping() : sendTime( 0.0f )
  {
  }

  Ping::Ping( float _sendTime ) : sendTime( _sendTime )
  {
  }

  ByteStream& operator>>( ByteStream& bs, Ping& msg )
  {
    bs >> msg.sendTime;
    return bs;
  }

  ByteStream& operator<<( ByteStream& bs, const Ping& msg )
  {
    bs << Ping::type << msg.sendTime;
    return bs;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Pong Message
  //////////////////////////////////////////////////////////////////////////////
  Pong::Pong() : sentTime( 0.0f )
  {
  }

  Pong::Pong( const Ping& ping ) : sentTime( ping.sendTime )
  {
  }

  ByteStream& operator>>( ByteStream& bs, Pong& msg )
  {
    bs >> msg.sentTime;
    return bs;
  }

  ByteStream& operator<<( ByteStream& bs, const Pong& msg )
  {
    bs << Pong::type << msg.sentTime;
    return bs;
  }

  //////////////////////////////////////////////////////////////////////////////
  // RequestID Message
  //////////////////////////////////////////////////////////////////////////////
  RequestID::RequestID() : id( UINT16_MAX )
  {
  }

  RequestID::RequestID( uint16 _id ) : id( _id )
  {
  }

  ByteStream& operator>>( ByteStream& bs, RequestID& msg )
  {
    bs >> msg.id;
    return bs;
  }

  ByteStream& operator<<( ByteStream& bs, const RequestID& msg )
  {
    bs << RequestID::type << msg.id;
    return bs;
  }


  //////////////////////////////////////////////////////////////////////////////
  // ServerBroadcast Message
  //////////////////////////////////////////////////////////////////////////////
  ServerBroadcast::ServerBroadcast() : id( UINT16_MAX )
  {
  }

  ServerBroadcast::ServerBroadcast( uint16 _id, std::string _serverName ) : id( _id ),
                                                                            serverName( _serverName )
  {
  }

  ByteStream& operator>>( ByteStream& bs, ServerBroadcast& msg )
  {
    bs >> msg.id >> msg.serverName;
    return bs;
  }

  ByteStream& operator<<( ByteStream& bs, const ServerBroadcast& msg )
  {
    bs << ServerBroadcast::type << msg.id << msg.serverName;
    return bs;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // ConnectRequest Message
  //////////////////////////////////////////////////////////////////////////////
  ConnectRequest::ConnectRequest() : peerID( UINT16_MAX )
  {
  }
  
  ConnectRequest::ConnectRequest(uint16 _peerID, const std::string &_peerName) : peerID( _peerID ),
                                                                                 peerName( _peerName )
  {
  }
  
  ByteStream& operator>>( ByteStream& bs, ConnectRequest& msg )
  {
    bs >> msg.peerID >> msg.peerName;
    return bs;
  }
  
  ByteStream& operator<<( ByteStream& bs, const ConnectRequest& msg )
  {
    bs << ConnectRequest::type << msg.peerID << msg.peerName;
    return bs;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // ConnectAccepted Message
  //////////////////////////////////////////////////////////////////////////////
  ConnectAccepted::ConnectAccepted() : eidLower( -1 ),
                                       serverID( UINT16_MAX )
  {
  }
  
  ConnectAccepted::ConnectAccepted( int32 _eidLower, uint16 _serverID ) : eidLower( _eidLower ),
                                                                          serverID( _serverID )
  {
  }
  
  ByteStream& operator>>( ByteStream& bs, ConnectAccepted& msg )
  {
    bs >> msg.eidLower >> msg.serverID;
    return bs;
  }
  
  ByteStream& operator<<( ByteStream& bs, const ConnectAccepted& msg )
  {
    bs << ConnectAccepted::type << msg.eidLower << msg.serverID;
    return bs;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // PositionUpdate Message
  //////////////////////////////////////////////////////////////////////////////
  PositionUpdate::PositionUpdate() : eid( -1 )
  {
  }
  
  PositionUpdate::PositionUpdate( Engine::Entity* e )
  {
    eid = e->ID();
  }
  
  ByteStream& operator>>( ByteStream& bs, PositionUpdate& msg )
  {
    bs >> msg.eid;
    return bs;
  }
  
  ByteStream& operator<<( ByteStream& bs, const PositionUpdate& msg )
  {
    bs << PositionUpdate::type << msg.eid;
    return bs;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // UpdateGamePeer Message
  //////////////////////////////////////////////////////////////////////////////
  UpdateGamePeer::UpdateGamePeer() : action( UINT8_MAX ),
                                     peerID( UINT16_MAX )
  {
  }
  
  UpdateGamePeer::UpdateGamePeer( uint8 _action, uint16 _peerID, const std::string& _name ) : action( _action ),
                                                                                              peerID( _peerID ),
                                                                                              name( _name )
  {
  }
  
  ByteStream& operator>>( ByteStream& bs, UpdateGamePeer& msg )
  {
    bs >> msg.action >> msg.peerID >> msg.name;
    return bs;
  }
  
  ByteStream& operator<<( ByteStream& bs, const UpdateGamePeer& msg )
  {
    bs << UpdateGamePeer::type << msg.action << msg.peerID << msg.name;
    return bs;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // GameMessage Message
  //////////////////////////////////////////////////////////////////////////////
  GameMessage::GameMessage( const Engine::Message* _msg ) : msg( _msg )
  {
  }

  ByteStream& operator>>( ByteStream& bs, GameMessage& )
  {
    return bs;
  }

  ByteStream& operator<<( ByteStream& bs, const GameMessage& msg )
  {
    bs << GameMessage::type << *(msg.msg);
    return bs;
  }

} // Messages namespace
} // Networking namespace
} // SF namespace
