/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    RPacket.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/22/2009 @ 17:43:24 - nwilliam
**  \brief   Reliable packet implementation
*******************************************************************************/

#include "RPacket.h"

namespace SF {
namespace Networking
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors
  //////////////////////////////////////////////////////////////////////////////
  RPacket::RPacket() : m_seq( 0 )
  {
  }

  RPacket::RPacket( uint16 seq ) : m_seq( seq )
  {
  }

  RPacket::RPacket( uint16 seq, const ByteStream& bs ) : m_seq( seq )
  {
    m_stream << bs;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  bool RPacket::operator<( const RPacket& rhs ) const
  {
    return m_seq < rhs.Seq();
  }

  uint16 RPacket::Seq() const
  {
    return m_seq;
  }

  ByteStream& RPacket::Stream()
  {
    return m_stream;
  }

  const ByteStream& RPacket::Stream() const
  {
    return m_stream;
  }

  bool RPacket::WillOverflow( int32 size ) const
  {
    return m_stream.WillOverflow( size );
  }

} // Network namespace
} // SF namespace