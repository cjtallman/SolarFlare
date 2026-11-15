/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ByteStream.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/18/2009 @ 01:12:46 - nwilliam
**  \brief   Generic stream, memory based.
*******************************************************************************/

#ifndef SF_BYTE_STREAM_H
#define SF_BYTE_STREAM_H

#include <string>
#include "Types.h"
#include "Utilities/SharedArray.h"

namespace SF {
namespace Networking
{
  class ByteStream
  {
    public:
      // The ever questioned "recommended" UDP datagram size
      // Ethernet: 1538-12-8-4-6-6-2-2-5-20-8..ish = 1472..ish
      // 576-byte IP datagram guarantee - junk = 512, would be sanity check
      static const int DefaultMaxSize = 1400;
      enum ErrorFlag { NONE, UNDER_FLOW, OVER_FLOW };

    public:
      ByteStream( int32 maxSize = DefaultMaxSize );
      ~ByteStream();
      ByteStream Clone() const;

      int32 Size() const;
      void Size( int32 count );

      int32 MaxSize() const;
      void MaxSize( int32 max );

      uint8* Buffer();
      const uint8* Buffer() const;

      void Clear();
      bool Readable() const;
      bool HadError() const;
      ErrorFlag Error() const;
      bool WillOverflow( int32 size ) const;

      ByteStream& operator<<( const bool data );
      ByteStream& operator<<( const char data );
      ByteStream& operator<<( const int8 data );
      ByteStream& operator<<( const int16 data );
      ByteStream& operator<<( const int32 data );
      ByteStream& operator<<( const int64 data );
      ByteStream& operator<<( const uint8 data );
      ByteStream& operator<<( const uint16 data );
      ByteStream& operator<<( const uint32 data );
      ByteStream& operator<<( const uint64 data );
      ByteStream& operator<<( const float data );
      ByteStream& operator<<( const double data );

      ByteStream& operator>>( bool& data );
      ByteStream& operator>>( char& data );
      ByteStream& operator>>( int8& data );
      ByteStream& operator>>( int16& data );
      ByteStream& operator>>( int32& data );
      ByteStream& operator>>( int64& data );
      ByteStream& operator>>( uint8& data );
      ByteStream& operator>>( uint16& data );
      ByteStream& operator>>( uint32& data );
      ByteStream& operator>>( uint64& data );
      ByteStream& operator>>( float& data );
      ByteStream& operator>>( double& data );

      ByteStream& operator<<( const ByteStream& rhs );

      // Note: Also inserts/extracts uint8 for strlen (ie, assumes <= 255)
      ByteStream& operator>>( std::string& rhs );
      ByteStream& operator<<( const std::string& rhs );
      static size_t SerializedStringSize( const std::string& str );

    private:
      void StreamOut( uint8* data , int32 size );
      void StreamIn( const uint8* data , int32 size );

      int32 m_used;
      int32 m_maxSize;
      int32 m_readPos;
      int32 m_writePos;
      ErrorFlag m_error;
      Utilities::SharedArray<uint8> m_buffer;
  };

} // Networking namespace
} // SF namespace

#endif // SF_BYTE_STREAM_H
