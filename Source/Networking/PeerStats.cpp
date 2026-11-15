/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    PeerStats.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/20/2009 @ 20:54:14 - nwilliam
**  \brief   Struct with lots of data for tracking usage and performance 
             statistics.
*******************************************************************************/

#include "PeerStats.h"

#if defined( _MSC_VER )
  // 4351: new behavior, array will be default initialized (sigh, intented behavior!)
  #pragma warning( push )
  #pragma warning( disable : 4351 )
#endif

namespace
{
  const double ONE_OVER_KB = 1.0f / 1024.0f;
  const double ONE_OVER_MB = 1.0f / 1048576.0f;
}

namespace SF {
namespace Networking
{
  PeerStats::PeerStats() : latencyTo( 0.0f ),
                           latencyFrom( 0.0f ),
                           byteStat(),
                           packetStat()
  {
  }

  uint32 PeerStats::ByteCount( PeerStats::STAT type )
  {
    return byteStat[type];
  }

  double PeerStats::KByteCount( PeerStats::STAT type )
  {
    return byteStat[type] * ONE_OVER_KB;
  }

  double PeerStats::MByteCount( PeerStats::STAT type )
  {
    return byteStat[type] * ONE_OVER_MB;
  }

  uint32 PeerStats::PacketCount( PeerStats::STAT type )
  {
    return packetStat[type];
  }

  double PeerStats::RoundTripLatency() const
  {
    return latencyTo + latencyFrom;
  }

  void PeerStats::Track( PeerStats::STAT type, uint32 numBytes, uint32 numPackets )
  {
    byteStat[type] += numBytes;
    packetStat[type] += numPackets;
  }

} // Networking namespace
} // SF namespace

#if defined( _MSC_VER )
  // 4351: new behavior, array will be default initialized (sigh, intented behavior!)
  #pragma warning( pop )
#endif
