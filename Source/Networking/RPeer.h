/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RPeer.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/18/2009 @ 00:39:36 - nwilliam
**  \brief   Wrapper for all reliable peer related data, with helper functions.
*******************************************************************************/

#ifndef SF_R_PEER_H
#define SF_R_PEER_H

#include <list>
#include "Types.h"
#include "Address.h"
#include "RPacket.h"
#include "PeerStats.h"
#include "NetworkMessages.h"

namespace SF {
namespace Networking
{
  class RPeer
  {
    public:
      static Messages::PacketHeader packetHeader;

      RPeer();
      RPeer( uint16 _id, const Address& _addr );

      bool operator==( uint16 _id ) const;
      bool operator==( const RPeer& rhs ) const;

      bool IsSendWaiting() const;
      bool IsRecvWaiting() const;

      void SentList();
      RPacket ProcessRecvFront();
      const RPacketList& ToSendList();

      void SendTo( ByteStream& bs );
      void SendTo( const Messages::Pong& msg );
      void SendTo( const Messages::GameMessage& msg );

      void UpdateAcknowledged( uint16 ack );
      void AddRecv( uint16 sequence, ByteStream& stream );

      bool AcceptUnreliable( uint16 sequenceID );

    public:
      uint16 id;
      Address addr;
      float comAccum;
      PeerStats stats;

    private:
      void AddSend( int32 size );
      static bool AcceptSequence( uint16 oldSeq, uint16 newSeq );

      bool m_sentDirty;
      RPacketList m_recv;
      RPacketList m_send;
      uint16 m_udpSequence;
      uint16 m_remoteAcked;
      uint16 m_localSequence;
      uint16 m_recvProcessSeq;
  };


  typedef std::list< RPeer > RPeerList;


} // Network namespace
} // SF namespace

#endif // SF_R_PEER_H
