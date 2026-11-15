/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Message.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 01/29/2009 @ 15:16:56 - nwilliam
**  \brief   Generic game message.
*******************************************************************************/

#ifndef SF_MESSAGE_H
#define SF_MESSAGE_H

#include <string>
#include "MessageStack.h"
#include "Networking/ByteStream.h"

namespace SF {
namespace Engine
{
  struct Message
  {
    enum ROUTE { MSG_ALL          = 0, 
                 MSG_LOCAL        = 1, 
                 MSG_ALL_PEERS    = 2, 
                 MSG_SINGLE_PEER  = 3 };

    std::string   type;
    MessageStack  stack;
    Message::ROUTE route;

    size_t StreamOutByteSize() const;
  };
  
  
  Networking::ByteStream& operator>>( Networking::ByteStream& bs, Message& );
  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const Message& );
  
}
}

#endif // SF_MESSAGE_H
