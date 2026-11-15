/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RUDP.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/18/2009 @ 00:37:22 - nwilliam
**  \brief   RUDP implementation.
*******************************************************************************/

#include <algorithm>  //find

#include "RUDP.h"
#include "SolarFlare/Globals.h"
#include "Utilities/DevConsole.h"

namespace SF {
namespace Networking
{
  //void RUDP::DumpPacketToFile( std::ostream& os, const ByteStream& orig )
  //{
  //  using namespace SF;
  //  using namespace Networking;

  //  ByteStream bs = orig.Clone();

  //  Messages::PacketHeader ph;
  //  bs >> ph;

  //  os << "____START____\n";
  //  os << "  Marker: " << (ph.marker == m_marker) << " From: " << ph.peerID << " Type: " << (char)('0' + (char)ph.typeTag) << '\n';

  //  if( ph.typeTag == RUDP::RUDP_INFO )
  //  {
  //    RPeer* peer = Peer( ph.peerID );

  //    if( peer )
  //    {
  //      Messages::RUDPHeader rh;
  //      bs >> rh;

  //      os << "  RUDP Info:\n";
  //      os << "    SeqID: " << rh.sequenceID << " RemAck: " << rh.remoteAcked << '\n';

  //      os << "  Packet Data:\n";

  //      while( bs.Size() > 0 && bs.HadError() == false )
  //      {
  //        uint8 type;
  //        bs >> type;

  //        switch( type )
  //        {
  //          case Messages::PING:
  //          {
  //            Messages::Ping p;
  //            bs >> p;
  //            os << "    Ping\n";
  //          }
  //          break;

  //          case Messages::PONG:
  //          {
  //            Messages::Pong p;
  //            bs >> p;
  //            os << "    Pong\n";
  //          }
  //          break;

  //          case Messages::SERVER_BROADCAST:
  //          {
  //            Messages::ServerBroadcast p;
  //            bs >> p;
  //            os << "    ServerBroadcast\n";
  //          }
  //          break;

  //          case Messages::CONNECT_REQUEST:
  //          {
  //            Messages::ConnectRequest p;
  //            bs >> p;
  //            os << "    ConnectRequest\n";
  //          }
  //          break;

  //          case Messages::CONNECT_ACCEPTED:
  //          {
  //            Messages::ConnectAccepted p;
  //            bs >> p;
  //            os << "    ConnectAccepted\n";
  //          }
  //          break;

  //          case Messages::POSITION_UPDATE:
  //          {
  //            Messages::PositionUpdate p;
  //            bs >> p;
  //            os << "    PositionUpdate\n";
  //          }
  //          break;

  //          case Messages::UPDATE_GAME_PEER:
  //          {
  //            Messages::UpdateGamePeer p;
  //            bs >> p;
  //            os << "    UpdateGamePeer\n";
  //          }
  //          break;

  //          case Messages::GAME_MESSAGE:
  //          {
  //            os << "    GameMessage\n";
  //            Engine::Message msg;
  //            bs >> msg;

  //            os << "      MsgType: " << msg.type << '\n';

  //            lua_State* ls = luaL_newstate();
  //            luaL_openlibs( ls );
  //            msg.stack.PushToLua( ls );

  //            size_t tbllen = lua_objlen( ls, 1 );
  //            for( size_t i = 1; i <= tbllen; ++i )
  //            {
  //              lua_pushinteger( ls, i );
  //              lua_gettable( ls, -2 );
  //              
  //              os << "      " << i << ": " << lua_typename( ls, lua_type( ls, -1 ) ) << ": ";
  //              
  //              switch( lua_type( ls, -1 ) )
  //              {
  //                //case LUA_TNIL:
  //                case LUA_TNUMBER:
  //                  os << lua_tonumber( ls, -1 );
  //                break;

  //                case LUA_TBOOLEAN:
  //                  os << (lua_toboolean( ls, -1 ) ? "true" : "false");
  //                break;

  //                case LUA_TSTRING:
  //                  os << lua_tostring( ls, -1 );
  //                break;

  //                //case LUA_TTABLE:
  //                //case LUA_TFUNCTION:
  //                //case LUA_TUSERDATA:
  //                //case LUA_TTHREAD:
  //                //case LUA_TLIGHTUSERDATA:
  //              }
  //              
  //              os << '\n';
  //              lua_pop( ls, 1 );
  //            }

  //            lua_close( ls );
  //          }
  //          break;

  //          default:
  //            os << "    UNKNOWN_BAD!! PACKET SHOULD BE CONSIDERED JUNK\n";
  //        }
  //      }
  //    }
  //    else
  //    {
  //      os << "  Unknown peer\n";
  //    }
  //  }

  //  os << "____END____\n" << std::endl;
  //}

  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  RUDP::RUDP() : m_port( 0 ),
                 m_marker( 0 ),
                 m_udpSequence( 0 )
  {
    InitStreamLists();
  }

  RUDP::~RUDP()
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void RUDP::Update()
  {
    for( RPeerList::iterator it = m_peers.begin(); it != m_peers.end(); ++it )
    {
      for( ByteStreamList::iterator bit = m_unreliable.begin(); bit != m_unreliable.end(); ++bit )
      {
        if( bit->Size() > sizeof( Messages::PacketHeader ) )
        {
          it->stats.Track( PeerStats::U_SENT, bit->Size() );
          m_mainSocket.Send( it->addr, bit->Buffer(), bit->Size() );
        }
      }

      for( ByteStreamList::iterator bit = m_reliable.begin(); bit != m_reliable.end(); ++bit )
      {
        it->SendTo( *bit );
      }

      // Send all cached outgoing
      const RPacketList& sl = it->ToSendList();
      for( RPacketList::const_iterator p = sl.begin(); p != sl.end(); ++p )
      {
        it->stats.Track( PeerStats::R_SENT, p->Stream().Size() );
        m_mainSocket.Send( it->addr, p->Stream().Buffer(), p->Stream().Size() );
      }
    }

    InitStreamLists();

    if( m_mainSocket.HadError() )
    {
      Globals::console->Error() << "Socket Error: " 
                                << m_mainSocket.GetError() << std::endl;
      m_mainSocket.ClearError();
    }
  }

  int RUDP::SendTemp( const Address& to )
  {
    return m_mainSocket.Send( to, m_temp.Buffer(), m_temp.Size() );
  }

  RUDP::RecvType RUDP::Recieve( ByteStream& bs, Address& from, RPeer*& peer )
  {
    peer = 0;
    bs.Clear();

    int32 count = 0;

    do
    {
      count = m_mainSocket.Receive( from, bs.Buffer(), bs.MaxSize() );
      bs.Size( count );

      if( ParseReceived( bs, peer ) == RECV_UNKNOWN )
        return RECV_UNKNOWN;
    }
    while( count > 0 );
    
    // Look for a packet we can proces from a peer
    for( RPeerList::iterator it = m_peers.begin(); it != m_peers.end(); ++it )
    {
      if( it->IsRecvWaiting() )
      {
        peer = &(*it);
        bs = it->ProcessRecvFront().Stream();
        return RECV_PEER;
      }
    }
    
    return RECV_NONE;
  }
  
  ByteStream& RUDP::TempBuffer()
  {
    InitUnreliable( m_temp );
    return m_temp;
  }

  void RUDP::RemovePeer( uint16 peerID )
  {
    RPeerList::iterator it = std::find( m_peers.begin(), m_peers.end(), peerID );

    if( it != m_peers.end() )
      m_peers.erase( it );
  }

  void RUDP::UpdatePeer( uint16 peerID, const Address& addr )
  {
    RPeer* peer = Peer( peerID );

    if( peer )
    {
      peer->addr = addr;
    }
    else
    {
      m_peers.push_back( RPeer( peerID, addr ) );
    }
  }

  int RUDP::BroadcastState( bool enabled )
  {
    if( m_mainSocket.IsOpen() )
    {
      return m_mainSocket.BroadcastState( enabled );
    }
    
    return 0;
  }

  int RUDP::Broadcast( const ByteStream& bs )
  {
    return m_mainSocket.Broadcast( bs.Buffer(), bs.Size() );
  }

  void RUDP::Reset()
  {
    m_temp.Clear();
    m_peers.clear();
    m_udpSequence = 0;

    InitStreamLists();
  }

  void RUDP::SetInfo( uint16 port, uint32 marker )
  {
    if( m_mainSocket.IsOpen() )
    {
      m_mainSocket.Close();
    }

    m_port = port;
    m_marker = marker;

    // Non-zero marker indicates should open on port
    // If port is zero, will use any available port (behavior of socket bind())
    // This is perferable, and used, for clients
    if( marker )
    {
      m_mainSocket.Open( m_port );
    }
  }

  RPeerList& RUDP::Peers()
  {
    return m_peers;
  }

  RPeer* RUDP::Peer( uint16 id )
  {
    RPeer* peer = 0;
    RPeerList::iterator it = std::find( m_peers.begin(), m_peers.end(), id );
    
    if( it != m_peers.end() )
      peer = &( *it );
    
    return peer;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private Methods
  //////////////////////////////////////////////////////////////////////////////
  void RUDP::InitStreamLists()
  {
    m_reliable.clear();
    m_unreliable.clear();

    m_unreliable.push_back( ByteStream( ByteStream::DefaultMaxSize - sizeof(Messages::PacketHeader) ) );

    InitUnreliable( m_unreliable.back() );
  }

  void RUDP::InitUnreliable( ByteStream& bs )
  {
    bs.Clear();

    Messages::PacketHeader ph = RPeer::packetHeader;
    ph.typeTag = RUDP::RUDP_UNRELIABLE;

    bs << ph << Messages::UDPHeader( ++m_udpSequence );
  }

  RUDP::RecvType RUDP::ParseReceived( ByteStream& bs, RPeer*& peer )
  {
    RecvType status = RUDP::RECV_INVALID;

    ByteStream copy = bs.Clone();

    Messages::PacketHeader ph;

    bs >> ph;

    if( ph.marker == m_marker )
    {
      peer = Peer( ph.peerID );

      switch( ph.typeTag )
      {
        case RUDP::RUDP_INFO:
        {
          if( peer )
          {
            Messages::RUDPHeader hdr;
            bs >> hdr;

            peer->comAccum = 0.0f;
            peer->AddRecv( hdr.sequenceID, bs );
            peer->UpdateAcknowledged( hdr.remoteAcked );
            status = RUDP::RECV_PEER;
          }
        }
        break;
        
        case RUDP::RUDP_UNRELIABLE:
        {
          Messages::UDPHeader hdr;
          bs >> hdr;

          if( !peer || peer->AcceptUnreliable( hdr.sequenceID ) )
          {
            status = RUDP::RECV_UNKNOWN;
          }
        }
        break;
      }
    }

    return status;
  }

} // Networking namespace
} // SF namespace
