/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    OutputStream.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/26/2009 @ 10:44:32 - nwilliam
**  \brief   Custom streambuf and ostream implementations.
*******************************************************************************/

#include "OutputStream.h"

namespace SF {
namespace Utilities
{
  //////////////////////////////////////////////////////////////////////////////
  // StreamBuffer
  //////////////////////////////////////////////////////////////////////////////
  StreamBuffer::StreamBuffer()
  {
    setp( m_buffer, m_buffer + StreamBuffer::BUF_SIZE );
  }

  StreamBuffer::StreamBuffer( const char* logPath ) : m_log( logPath )
  {
    setp( m_buffer, m_buffer + StreamBuffer::BUF_SIZE );
  }

  StreamBuffer::~StreamBuffer()
  {
    sync();
  }

  //////////////////////////////////////////////////////////////////////////////
  // Public Methods
  //////////////////////////////////////////////////////////////////////////////
  StreamBuffer::SyncCallback StreamBuffer::GetSyncCallback()
  {
    return m_syncDel;
  }

  void StreamBuffer::SetSyncCallback( const StreamBuffer::SyncCallback& del )
  {
    m_syncDel = del;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Protected methods
  //////////////////////////////////////////////////////////////////////////////
  StreamBuffer::int_type StreamBuffer::overflow( int_type _Meta )
  {
    // If the m_buffer is full - flush it out
    if( pptr() == epptr() )
    {
      sync();
    }

    // Append the character
    if( !traits_type::eq_int_type( traits_type::eof(), _Meta ) )
    {
      *pptr() = traits_type::to_char_type(_Meta);
      pbump(1);
    }

    // Newline flushes the m_buffer
    if( traits_type::eq_int_type('\n', _Meta) )
    {
      sync();
    }

    // Return success if not EOF
    return traits_type::not_eof( _Meta );
  }

  int StreamBuffer::sync()
  {
    // Append a NULL character to the end of the buffer
    *pptr() = '\0';

    if( m_syncDel )
    {
      m_syncDel( m_buffer );
    }

    // Write to the log if there is one open
    if( m_log.is_open() )
    {
      m_log.write( pbase(), static_cast<std::streamsize>( pptr() - pbase() ) );

      // Flush to disk to make sure it is written
      m_log.flush();
    }

    // Reset the m_buffer pointers
    setp( pbase(), epptr() );

    // Return success
    return 0;
  }


  //////////////////////////////////////////////////////////////////////////////
  // OutputStream
  //////////////////////////////////////////////////////////////////////////////
  OutputStream::OutputStream() : std::ostream( &m_buffer )
  {
  }

  OutputStream::OutputStream( const char* logPath ) : std::ostream( &m_buffer ),
                                                      m_buffer( logPath )
  {
  }

  OutputStream::~OutputStream()
  {
  }

  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  StreamBuffer::SyncCallback OutputStream::GetSyncCallback()
  {
    return m_buffer.GetSyncCallback();
  }

  void OutputStream::SetSyncCallback( const StreamBuffer::SyncCallback& del )
  {
    m_buffer.SetSyncCallback( del );
  }

} // Utilities namespace
} // SF namespace
