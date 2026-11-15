/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RPeer.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/18/2009 @ 00:39:41 - nwilliam
**  \brief   Implementation of reliable peer related helper functions.
*******************************************************************************/

#include "RPeer.h"
#include "Engine/Message.h"
#include "Utilities/Misc.h"

namespace SF {
namespace Networking
{
  Messages::PacketHeader RPeer::packetHeader;

  //////////////////////////////////////////////////////////////////////////////
  // Constructors
  //////////////////////////////////////////////////////////////////////////////
  RPeer::RPeer() : id( 0 ),
                   m_sentDirty( false ),
                   comAccum( 0.0f ),
                   m_remoteAcked( 0 ),
                   m_localSequence( 0 ),
                   m_recvProcessSeq( 0 ),
                   m_udpSequence( 0 )
  {
  }

  RPeer::RPeer( uint16 _id, const Address& _addr ) : id( _id ),
                                                     m_sentDirty( false ),
                                                     comAccum( 0.0f ),
                                                     addr( _addr ),
                                                     m_remoteAcked( 0 ),
                                                     m_localSequence( 0 ),
                                                     m_recvProcessSeq( 0 ),
                                                     m_udpSequence( 0 )
  {
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  bool RPeer::operator==( uint16 _id ) const
  {
    return id == _id;
  }

  bool RPeer::operator==( const RPeer& rhs ) const
  {
    return id == rhs.id;
  }

  bool RPeer::IsSendWaiting() const
  {
    return !m_send.empty();
  }

  bool RPeer::IsRecvWaiting() const
  {
    if( m_recv.size() && m_recv.front().Seq() == (m_recvProcessSeq + 1) )
      return true;
    
    return false;
  }

  RPacket RPeer::ProcessRecvFront()
  {
    RPacket rp = m_recv.front();
    m_recv.pop_front();
    m_recvProcessSeq = rp.Seq();

    return rp;
  }

  const RPacketList& RPeer::ToSendList()
  {
    m_sentDirty = true;
    return m_send;
  }

  void RPeer::SendTo( ByteStream& bs )
  {
    AddSend( bs.Size() );
    m_send.back().Stream() << bs;
  }

  void RPeer::SendTo( const Messages::Pong& msg )
  {
    AddSend( sizeof(Messages::Pong) );
    m_send.back().Stream() << msg;
  }

  void RPeer::SendTo( const Messages::GameMessage& msg )
  {
    int32 size = static_cast<int32>( sizeof(msg.type) + msg.msg->StreamOutByteSize() );
    AddSend( size );

    m_send.back().Stream() << msg;
  }

  void RPeer::UpdateAcknowledged( uint16 ack )
  {
    m_remoteAcked = Utilities::Max( m_remoteAcked, ack );

    while( !m_send.empty() && m_send.front().Seq() <= m_remoteAcked )
      m_send.pop_front();
  }



  void RPeer::AddRecv( uint16 packetSeq, ByteStream& stream )
  {
    if( packetSeq <= m_recvProcessSeq ) // Trivial reject, received and processed
    {
      stats.Track( PeerStats::DUP_RECV, stream.Size() );
      return;
    }

    for( RPacketList::iterator it = m_recv.begin(); it != m_recv.end(); ++it )
    {
      if( it->Seq() == packetSeq )
      {
        stats.Track( PeerStats::DUP_RECV, stream.Size() );
        return;  // Already received
      }
    }

    stats.Track( PeerStats::UNIQUE_RECV, stream.Size() );

    RPacket rp( packetSeq, stream );
    m_recv.push_back( rp );
    m_recv.sort();
  }

  bool RPeer::AcceptUnreliable( uint16 sequenceID )
  {
    if( AcceptSequence( m_udpSequence, sequenceID ) )
    {
      m_udpSequence = sequenceID;
      return true;
    }

    return false;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private
  //////////////////////////////////////////////////////////////////////////////
  void RPeer::AddSend( int32 size )
  {
    if( m_sentDirty || m_send.empty() || m_send.back().WillOverflow( size ) )
    {
      RPacket rp( ++m_localSequence );
      rp.Stream() << packetHeader << Messages::RUDPHeader( rp.Seq(), m_recvProcessSeq );
      m_send.push_back( rp );

      m_sentDirty = false;
    }
  }

  bool RPeer::AcceptSequence( uint16 oldSeq, uint16 newSeq )
  {
    return (newSeq > oldSeq) || (newSeq - oldSeq >= newSeq);
  }

} // Networking namespace
} // SF namespace
