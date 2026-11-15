/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ByteStream.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/19/2009 @ 16:53:18 - nwilliam
**  \brief   In memory stream implementation.
*******************************************************************************/

#include <cstring>      // memcpy, strlen
#include "ByteStream.h"

#include "Utilities/LeakCheck.h"

#define  SIN StreamIn( reinterpret_cast<const unsigned char*>( &data ), sizeof( data ) );  return *this;
#define SOUT StreamOut( reinterpret_cast<unsigned char*>( &data ), sizeof( data ) ); return *this;


namespace SF {
namespace Networking
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor / destructor
  //////////////////////////////////////////////////////////////////////////////
  ByteStream::ByteStream( int32 maxSize ) : m_maxSize( maxSize ),
                                            m_buffer( new uint8[maxSize] )
  {
    Clear();
  }
  
  ByteStream::~ByteStream()
  {
  }
  
  ByteStream ByteStream::Clone() const
  {
    ByteStream bs( *this );
    
    bs.m_buffer = new uint8[m_maxSize];
    std::memcpy( bs.m_buffer.Get(), m_buffer.Get(), sizeof( uint8 ) * m_maxSize );
    
    return bs;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  int32 ByteStream::Size() const
  {
    // Could calculate as: abs( (m_maxSize - m_readPos) - (m_maxSize - m_writePos) )
    return m_used;
  }

  void ByteStream::Size( int32 count )
  {
    if( count > m_maxSize )
      return;
    
    Clear();
    m_used = m_writePos = count;
  }

  int32 ByteStream::MaxSize() const
  {
    return m_maxSize;
  }

  void ByteStream::MaxSize( int max )
  {
    if( max != m_maxSize )
    {
      uint8* temp = new uint8[max];
      std::memcpy( temp, m_buffer.Get(), sizeof( uint8 ) * max );

      m_maxSize = max;
      m_buffer = temp;
    }
  }

  uint8* ByteStream::Buffer()
  {
    return m_buffer.Get();
  }

  const uint8* ByteStream::Buffer() const
  {
    return m_buffer.Get();
  }

  void ByteStream::Clear()
  {
    m_error = ByteStream::NONE;
    m_used = m_readPos = m_writePos = 0;
  }

  bool ByteStream::Readable() const
  {
    return Size() && !HadError();
  }

  bool ByteStream::HadError() const
  {
    return m_error != ByteStream::NONE;
  }

  ByteStream::ErrorFlag ByteStream::Error() const
  {
    return m_error;
  }

  bool ByteStream::WillOverflow( int32 size ) const
  {
    return ( m_used + size ) > m_maxSize;
  }

  ByteStream& ByteStream::operator<<( const bool data )
  {
    uint8 cdata = data ? 255 : 0;
    return (*this << cdata);
  }

  ByteStream& ByteStream::operator<<( const char data )     { SIN }
  ByteStream& ByteStream::operator<<( const int8 data )     { SIN }
  ByteStream& ByteStream::operator<<( const int16 data )    { SIN }
  ByteStream& ByteStream::operator<<( const int32 data )    { SIN }
  ByteStream& ByteStream::operator<<( const int64 data )    { SIN }
  ByteStream& ByteStream::operator<<( const uint8 data )    { SIN }
  ByteStream& ByteStream::operator<<( const uint16 data )   { SIN }
  ByteStream& ByteStream::operator<<( const uint32 data )   { SIN }
  ByteStream& ByteStream::operator<<( const uint64 data )   { SIN }
  ByteStream& ByteStream::operator<<( const float data )    { SIN }
  ByteStream& ByteStream::operator<<( const double data )   { SIN }

  ByteStream& ByteStream::operator>>( bool& data )
  {
    uint8 cdata;
    *this >> cdata;

    data = (cdata != 0);
    return *this;
  }

  ByteStream& ByteStream::operator>>( char& data )    { SOUT }
  ByteStream& ByteStream::operator>>( int8& data )    { SOUT }
  ByteStream& ByteStream::operator>>( int16& data )   { SOUT }
  ByteStream& ByteStream::operator>>( int32& data )   { SOUT }
  ByteStream& ByteStream::operator>>( int64& data )   { SOUT }
  ByteStream& ByteStream::operator>>( uint8& data )   { SOUT }
  ByteStream& ByteStream::operator>>( uint16& data )  { SOUT }
  ByteStream& ByteStream::operator>>( uint32& data )  { SOUT }
  ByteStream& ByteStream::operator>>( uint64& data )  { SOUT }
  ByteStream& ByteStream::operator>>( float& data )   { SOUT }
  ByteStream& ByteStream::operator>>( double& data )  { SOUT }

  ByteStream& ByteStream::operator<<( const ByteStream& rhs )
  {
    if( rhs.m_readPos + rhs.m_used <= rhs.m_maxSize )
    {
      // Simple case, we can read it in one shot
      StreamIn( rhs.m_buffer.Get() + rhs.m_readPos, rhs.m_used );
    }
    else
    {
      // Need to do it in two passes
      // Since we have to read this in two passes, make sure we wont' overflow
      if( WillOverflow( rhs.Size() ) )
      {
        m_error = ByteStream::OVER_FLOW;
        return *this;
      }
      
      const int32 pass1 = rhs.m_maxSize - rhs.m_readPos;
      const int32 pass2 = rhs.m_used - pass1;
      
      StreamIn( rhs.m_buffer.Get() + rhs.m_readPos, pass1 );
      StreamIn( rhs.m_buffer.Get(), pass2 );
    }
    
    return *this;
  }

  ByteStream& ByteStream::operator>>( std::string& rhs )
  {
    uint8 len = 0;
    StreamOut( &len, sizeof( len ) );
    
    rhs.clear();
    rhs.resize( len );  // So we can [] into it
    
    StreamOut( reinterpret_cast<uint8*>( &rhs[0] ), sizeof( std::string::value_type ) * len );
    
    return *this;
  }

  ByteStream& ByteStream::operator<<( const std::string& rhs )
  {
    uint8 len = static_cast<uint8>( rhs.length() ); // concious decision
    StreamIn( reinterpret_cast<const uint8*>( &len ), sizeof( len ) );
    StreamIn( reinterpret_cast<const uint8*>( &rhs[0] ), sizeof( std::string::value_type ) * len );
    return *this;
  }

  size_t ByteStream::SerializedStringSize( const std::string& str )
  {        // length     + character data
    return sizeof(uint8) + (sizeof(std::string::value_type) * str.length() );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private methods
  //////////////////////////////////////////////////////////////////////////////
  void ByteStream::StreamOut( uint8* data , int32 size )
  {
    if( HadError() )
      return;
    
    if( ( m_used - size ) < 0 )
    {
      m_error = ByteStream::UNDER_FLOW;
      return;
    }
    
    m_used -= size;
    
    if( ( m_readPos + size ) > m_maxSize )
    {
      int32 amount = m_maxSize - m_readPos;
      
      std::memcpy( data, m_buffer.Get() + m_readPos, amount );
      
      m_readPos = 0;
      size -= amount;
      data += amount;
    }
      
    std::memcpy( data, m_buffer.Get() + m_readPos, size );
    m_readPos += size;
  }
  
  void ByteStream::StreamIn( const uint8* data , int32 size )
  {
    if( HadError() )
      return;
    
    if( ( m_used + size ) > m_maxSize )
    {
      m_error = ByteStream::OVER_FLOW;
      return;
    }
    
    m_used += size;
    
    if( ( m_writePos + size ) > m_maxSize )
    {
      int32 amount = m_maxSize - m_writePos;
      
      std::memcpy( m_buffer.Get() + m_writePos, data, amount );
      
      m_writePos = 0;
      size -= amount;
      data += amount;
    }

    std::memcpy( m_buffer.Get() + m_writePos , data , size );
    m_writePos += size;
  }

} // Networking namespace
} // SF namespace
