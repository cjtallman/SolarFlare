/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Message.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 01/27/2009 @ 16:31:13 - nwilliam
**  \brief   Completely generic message implementation (just serialization).
*******************************************************************************/

#include "Message.h"

namespace SF {
namespace Engine
{
  size_t Message::StreamOutByteSize() const
  {
    return Networking::ByteStream::SerializedStringSize(type) + stack.SizeInBytes();
  }

  Networking::ByteStream& operator>>( Networking::ByteStream& bs, Message& m )
  {
    bs >> m.type >> m.stack;
    return bs;
  }

  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const Message& m )
  {
    bs << m.type << m.stack;
    return bs;
  }

} // Engine namespace
} // SF namespace
