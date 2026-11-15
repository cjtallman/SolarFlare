/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    PeerStats.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/20/2009 @ 20:46:47 - nwilliam
**  \brief   Struct with lots of data for tracking usage and performance 
             statistics.
*******************************************************************************/

#ifndef SF_PEER_STATS_H
#define SF_PEER_STATS_H

#include "Types.h"

namespace SF {
namespace Networking
{
  struct PeerStats
  {
    enum STAT { R_SENT, U_SENT, DROPPED_RECV, UNIQUE_RECV, DUP_RECV, LAST };

    PeerStats();

    uint32 ByteCount( PeerStats::STAT type );
    double KByteCount( PeerStats::STAT type );
    double MByteCount( PeerStats::STAT type );
    uint32 PacketCount( PeerStats::STAT type );

    double RoundTripLatency() const;
    void Track( PeerStats::STAT type, uint32 numBytes, uint32 numPackets = 1 );

    // Data members
    double latencyTo;
    double latencyFrom;
    uint32 byteStat[PeerStats::LAST];
    uint32 packetStat[PeerStats::LAST];
  };

} // Networking namespace
} // SF namespace

#endif // SF_PEER_STATS_H
