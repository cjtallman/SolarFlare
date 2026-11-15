/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    OutputStream.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/26/2009 @ 10:39:03 - nwilliam
**  \brief   Custom streambuf and ostream derived class for convenient ostream
             like behavior, with customizable output location (with a delegate).
*******************************************************************************/

#ifndef SF_DEBUG_STREAM_H
#define SF_DEBUG_STREAM_H

#include <fstream>
#include <iostream>

#include "Utilities/Delegates.h"

// TODO: Make StreamBuffer buffer size configurable

namespace SF {
namespace Utilities
{
  // Stream Buffer Class
  // Heart of ostream implementations. Processes one character at a time,
  // until there is a newline or 150 characters. Then calls sync to output.
  class StreamBuffer : public std::streambuf
  {
    public:
      typedef std::streambuf::char_type char_type;
      typedef std::streambuf::traits_type traits_type;
      typedef std::streambuf::int_type int_type;
      typedef Utilities::Delegate1<void,const char*> SyncCallback;

    public:
      StreamBuffer();                       // Default, no log file
      StreamBuffer( const char* logPath );  // with log file
      ~StreamBuffer();

      SyncCallback GetSyncCallback();
      void SetSyncCallback( const SyncCallback& del );

    protected:
      virtual int sync();
      virtual int_type overflow( int_type _Meta = std::char_traits<char>::eof() );

    private:
      // No copy, assignment
      StreamBuffer( const StreamBuffer& );
      StreamBuffer& operator=( const StreamBuffer& );

    private:
      static const int BUF_SIZE = 150;

      std::ofstream m_log;
      SyncCallback m_syncDel;
      char m_buffer[BUF_SIZE + 1];
  };


  // Convenience class for using above stream
  class OutputStream : public std::ostream
  {
    public:
      OutputStream();
      OutputStream( const char* logPath );
      ~OutputStream();

      StreamBuffer::SyncCallback GetSyncCallback();
      void SetSyncCallback( const StreamBuffer::SyncCallback& del );

    private:
      StreamBuffer m_buffer;

    private:
      // No copy, assignment
      OutputStream( const OutputStream& );
      OutputStream& operator=( const OutputStream& );
  };

} // Utilities namespace
} // SF namespace

#endif // SF_DEBUG_STREAM_H
