/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Socket.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/13/2009 @ 17:39:20 - nwilliam
**  \brief   Platform independent socket wrapper.
*******************************************************************************/

#include "Socket.h"
#include "NetOSSpecific.h"
#include "Utilities/Asserts.h"

#if !SF_USING_WINSOCK
  #include <cerrno>
#endif

namespace
{
  #if SF_USING_WINSOCK
    const socket_t InvalidSocket = (socket_t)~0;
  #else
    const socket_t InvalidSocket = -1;
  #endif
}

namespace SF {
namespace Networking
{
  //////////////////////////////////////////////////////////////////////////////
  // Static member functions
  //////////////////////////////////////////////////////////////////////////////
  int Socket::Initialize()
  {
    #if SF_USING_WINSOCK
      WSADATA data;
      return WSAStartup( MAKEWORD(2,2), &data );
    #else
      return SocketOk;
    #endif
  }

  int Socket::Shutdown()
  {
    #if SF_USING_WINSOCK
      return WSACleanup();
    #else
      return SocketOk;
    #endif
  }


  //////////////////////////////////////////////////////////////////////////////
  // Constructor / Destructor
  //////////////////////////////////////////////////////////////////////////////
  Socket::Socket() : m_error( Socket::SocketOk ),
                     m_port( 0 ),
                     m_socketID( InvalidSocket )
  {
  }

  Socket::~Socket()
  {
    Close();
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  void Socket::Close()
  {
    if( IsOpen() )
    {
      #if SF_USING_WINSOCK
        closesocket( m_socketID );
      #else
        close( m_socketID );
      #endif

      m_port = 0;
      m_socketID = InvalidSocket;
    }
  }

  bool Socket::IsOpen() const
  {
    return m_socketID != SocketError;
  }

  int Socket::Open( unsigned short port, bool blocking )
  {
    if( IsOpen() )
    {
      Close();
    }

    m_socketID = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

    if( m_socketID == InvalidSocket )
    {
      ErrorFromOS();
      return Socket::SocketError;
    }

    sockaddr_in addr;
    addr.sin_port        = htons( port );
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    int result = bind( m_socketID,
                       reinterpret_cast<const sockaddr*>( &addr ),
                       sizeof( addr )
                     );

    if( result == Socket::SocketError )
    {
      ErrorFromOS();
      Close();
      return result;
    }

    if( port )
    {
      // Set port after successful bind
      m_port = port;
    }
    else
    {
      // Called with 0 port, so get whatever port we did bind to
      int size = 0;
      getsockname( m_socketID, reinterpret_cast<sockaddr*>( &addr ), &size );
      m_port = addr.sin_port;
    }

    if( !blocking )
    {
      #if SF_USING_WINSOCK
        u_long nonBlocking = 1;
        result = ioctlsocket( m_socketID, FIONBIO, &nonBlocking );
      #else
        int nonBlocking = 1;
        // Supposedly this doesn't work on SunOS 4.1.x or AIX 3.2.5, would use ictl
        result = fcntl( m_socketID, F_SETFL, O_NONBLOCK, nonBlocking );
      #endif

      if( result == SocketError )
      {
        ErrorFromOS();
        Close();
        return Socket::SocketError;
      }
    }

    return Socket::SocketOk;
  }

  void Socket::ClearError()
  {
    m_error = Socket::SocketOk;
  }

  int Socket::GetError() const
  {
    return m_error;
  }

  bool Socket::HadError() const
  {
    return m_error != Socket::SocketOk;
  }

  int Socket::BroadcastState( bool enabled )
  {
    char isEnabled = enabled ? 0xFF: 0;

    int result = setsockopt( m_socketID,
                             SOL_SOCKET,
                             SO_BROADCAST,
                             reinterpret_cast<const char*>( &isEnabled ),
                             sizeof( isEnabled )
                           );

    if( result == Socket::SocketError )
    {
      ErrorFromOS();
    }

     return result;
  }

  int Socket::Broadcast( const void* data, int size )
  {
    Address bcast( INADDR_BROADCAST, m_port );
    return Send( bcast, data, size );
  }

  int Socket::Receive( Address& sender, void* data, int size )
  {
    sockaddr_in from;
    socklen_t fromLen = sizeof( from );

    int result = recvfrom( m_socketID,
                           reinterpret_cast<char*>( data ),
                           size,
                           0,
                           reinterpret_cast<sockaddr*>( &from ),
                           &fromLen
                         );

    if( result == Socket::SocketError )
    {
      ErrorFromOS();
    }
    else
    {
      sender = Address( ntohl( from.sin_addr.s_addr ), ntohs( from.sin_port ) );
    }

    return result;
  }

  int Socket::Send( const Address& receiver, const void* data, int size )
  {
    sockaddr_in address;
    address.sin_family      = AF_INET;
    address.sin_port        = htons( receiver.Port() );
    address.sin_addr.s_addr = htonl( receiver.FullAddress() );

    int result  = sendto( m_socketID,
                          reinterpret_cast<const char*>(data),
                          size,
                          0,
                          reinterpret_cast<const sockaddr*>( &address ),
                          sizeof( address )
                        );

    if( result == Socket::SocketError )
    {
      ErrorFromOS();
    }

    return result;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void Socket::ErrorFromOS()
  {
    #if SF_USING_WINSOCK
      int error = WSAGetLastError();

      if( (error != WSAEWOULDBLOCK) && (error >= 10000) && (error <= 11999) )
      {
        m_error = error;
        WSASetLastError( Socket::SocketOk );
      }
    #else
      if( errno != EWOULDBLOCK )
      {
        m_error = errno;
        errno = Socket::SocketOK;
      }
    #endif
  }

} // Networking namespace
} // SF namespace
