/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RPacket.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/16/2009 @ 17:59:08 - nwilliam
**  \brief   Reliable packet that keeps track of its own sequence number
*******************************************************************************/

#ifndef SF_R_PACKET_H
#define SF_R_PACKET_H

#include <list>
#include "Types.h"
#include "ByteStream.h"

namespace SF {
namespace Networking
{
  class RPacket
  {
    public:
      RPacket();
      RPacket( uint16 seq );
      RPacket( uint16 seq, const ByteStream& bs );

      // Default copy, assignment, destructor ok

      bool operator<( const RPacket& rhs ) const;

      uint16 Seq() const;
      ByteStream& Stream();
      bool WillOverflow( int32 size ) const;

      const ByteStream& Stream() const;

    private:
      uint16 m_seq;
      ByteStream m_stream;
  };

  typedef std::list< RPacket > RPacketList;


} // Network namespace
} // SF namespace

#endif // SF_R_PACKET_H
