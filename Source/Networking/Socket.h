/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Socket.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/13/2009 @ 17:39:12 - nwilliam
**  \brief   Platform independent socket wrapper.
*******************************************************************************/

#ifndef SF_SOCKET_H
#define SF_SOCKET_H

#include "Types.h"
#include "Address.h"

namespace SF {
namespace Networking
{
  class Socket
  {
    public:
      static int Shutdown();
      static int Initialize();

      static const int SocketOk = 0;
      static const int SocketError = -1;

    public:
      Socket();
      ~Socket();

      void Close();
      bool IsOpen() const;
      int Open( uint16 port, bool blocking = false );

      void ClearError();
      int GetError() const;
      bool HadError() const;

      int BroadcastState( bool enabled );
      int Broadcast( const void* data, int size );
      int Receive( Address& sender, void* data, int size );
      int Send( const Address& receiver, const void* data, int size );

    private:
      void ErrorFromOS();

      int m_error;
      uint16 m_port;
      socket_t m_socketID;
  };
  
} // Networking namespace
} // SF namespace

#endif // SF_SOCKET_H
