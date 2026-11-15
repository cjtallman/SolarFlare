/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    NetOSSpecific.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/14/2009 @ 16:06:17 - nwilliam
**  \brief   Unnavoidable networking related operating system specifics.
*******************************************************************************/

#ifndef SF_NET_OS_SPECIFIC_H
#define SF_NET_OS_SPECIFIC_H

#if defined( _WIN32 ) && defined( _MSC_VER )
  #include <winsock2.h>
  #pragma comment( lib, "wsock32.lib" )

  #define SF_USING_WINSOCK 1
  typedef int socklen_t;
#elif defined( __CYGWIN__ ) || defined( __linux )
  #include <fcntl.h>
  #include <unistd.h> // close
  #include <sys/socket.h>
  #include <netinet/in.h>

  #define SF_USING_WINSOCK 0
#else
  #error "Networking not compiled on this platform yet!"
#endif

#endif // SF_NET_OS_SPECIFIC_H
