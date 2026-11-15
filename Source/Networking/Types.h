/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Types.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/14/2009 @ 16:03:46 - nwilliam
**  \brief   Typedef for known, and garunteed, types of specific sizes.
*******************************************************************************/

#ifndef SF_TYPES_H
#define SF_TYPES_H

#if defined( _MSC_VER )
  typedef    signed __int8  int8_type;
  typedef  unsigned __int8  uint8_type;
  typedef   signed __int16  int16_type;
  typedef unsigned __int16  uint16_type;
  typedef   signed __int32  int32_type;
  typedef unsigned __int32  uint32_type;
  typedef   signed __int64  int64_type;
  typedef unsigned __int64  uint64_type;
#elif defined( __GNUC__ )
  #include <stdint.h>
  
  typedef   int8_t  int8_type;
  typedef  uint8_t  uint8_type;
  typedef  int16_t  int16_type;
  typedef uint16_t  uint16_type;
  typedef  int32_t  int32_type;
  typedef uint32_t  uint32_type;
  typedef  int64_t  int64_type;
  typedef uint64_t  uint64_type;
#else
  // Make the best guesses we can
  typedef        signed char  int8_type;
  typedef      unsigned char  uint8_type;
  typedef       signed short  int16_type;
  typedef     unsigned short  uint16_type;
  typedef         signed int  int32_type;
  typedef       unsigned int  uint32_type;
  typedef   signed long long  int64_type;
  typedef unsigned long long  uint64_type;
#endif

// Signed types
typedef  int8_type   int8;
typedef int16_type   int16;
typedef int32_type   int32;
typedef int64_type   int64;

// Unsigned types
typedef  uint8_type  uint8;
typedef uint16_type  uint16;
typedef uint32_type  uint32;
typedef uint64_type  uint64;


#if defined( _WIN32 ) && defined( _MSC_VER )
  typedef __w64 unsigned int socket_t;
#elif defined( __CYGWIN__ ) || defined( __linux )
  typedef int socket_t;
#else
  #error "Define socket_T for this platform"
#endif


#endif // SF_TYPES_H
