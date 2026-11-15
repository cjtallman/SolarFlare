/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RUDP.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/18/2009 @ 00:37:05 - nwilliam
**  \brief   Standard reliability layer ontop of UDP.
*******************************************************************************/

#ifndef SF_RUDP_H
#define SF_RUDP_H

#include "RPeer.h"
#include "Socket.h"
#include "RPacket.h"

namespace SF {
namespace Networking
{
  class RUDP
  {
    public:
      enum RUDPTags { RUDP_INFO, RUDP_UNRELIABLE };
      enum RecvType { RECV_NONE, RECV_INVALID, RECV_UNKNOWN, RECV_PEER };

      RUDP();
      ~RUDP();

      void Update();
      int SendTemp( const Address& to );
      RecvType Recieve( ByteStream& bs, Address& from, RPeer*& peer );
      
      ByteStream& TempBuffer();
      
      void RemovePeer( uint16 peerID );
      void UpdatePeer( uint16 peerID, const Address& addr );

      int BroadcastState( bool enabled );
      int Broadcast( const ByteStream& bs );

      void Reset();
      void SetInfo( uint16 port, uint32 marker );

      RPeerList& Peers();
      RPeer* Peer( uint16 id );

      template< typename T >
      void StreamReliable( const T& rhs );

      template< typename T >
      void StreamUnreliable( const T& rhs );

    private:
      typedef std::list< ByteStream > ByteStreamList;

      void InitStreamLists();
      void InitUnreliable( ByteStream& bs );
      RecvType ParseReceived( ByteStream& bs, RPeer*& peer );
      //void DumpPacketToFile( std::ostream& os, const ByteStream& orig );

      uint16 m_port;
      uint32 m_marker;
      RPeerList m_peers;
      Socket m_mainSocket;
      uint16 m_udpSequence;

      ByteStream m_temp;
      ByteStream m_scratch;
      ByteStreamList m_reliable;
      ByteStreamList m_unreliable;
  };




  // Template implementations
  template< typename T >
  void RUDP::StreamReliable( const T& rhs )
  {
    m_scratch.Clear();
    m_scratch << rhs;
    
    if( m_reliable.empty() || m_reliable.back().WillOverflow( m_scratch.Size() ) )
      m_reliable.push_back( ByteStream( ByteStream::DefaultMaxSize - sizeof(Messages::PacketHeader) - sizeof(Messages::RUDPHeader) ) );
    
    m_reliable.back() << m_scratch;
  }

  template< typename T >
  void RUDP::StreamUnreliable( const T& rhs )
  {
    m_scratch.Clear();
    m_scratch << rhs;
    
    if( m_unreliable.back().WillOverflow( m_scratch.Size() ) )
    {
      m_unreliable.push_back( ByteStream( ByteStream::DefaultMaxSize - sizeof(Messages::PacketHeader) ) );
      InitUnreliable( m_unreliable.back() );
    }
    
    m_unreliable.back() << m_scratch;
  }

} // Networking namespace
} // SF namespace

#endif // RUDP_H
