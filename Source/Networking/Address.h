/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Address.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/16/2009 @ 18:09:15 - nwilliam
**  \brief   Simple IP4 address and port wrapper.
*******************************************************************************/

#ifndef SF_ADDRESS_H
#define SF_ADDRESS_H

#include "Types.h"

namespace SF {
namespace Networking
{
  class Address
  {
    public:
      Address();
      Address( uint32 address, uint16 port );
      Address( const char* addr, uint16 port );
      Address( uint8 a, uint8 b, uint8 c, uint8 d, uint16 port );

      // Default copy, assignment, destructor ok

      uint8 A() const;
      uint8 B() const;
      uint8 C() const;
      uint8 D() const;
      uint16 Port() const;
      uint32 FullAddress() const;
      const char* ToString() const;

      operator bool() const;
      bool operator<( const Address& rhs ) const;
      bool operator>( const Address& rhs ) const;
      bool operator==( const Address& rhs ) const;
      bool operator!=( const Address& rhs ) const;

    private:
      void BuildString() const;

      uint16 m_port;
      uint32 m_address;
      mutable char m_string[16];
  };

} // Networking namespace
} // SF namespace

#endif // SF_ADDRESS_H
