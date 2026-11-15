/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Address.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/16/2009 @ 18:10:52 - nwilliam
**  \brief   Implementation of IP4 address wrapper.
*******************************************************************************/

#include "Address.h"

namespace SF {
namespace Networking
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructors
  //////////////////////////////////////////////////////////////////////////////
  Address::Address() : m_port( 0 ),
                       m_address( 0 )
  {
    m_string[0] = 0;
  }

  Address::Address( uint8 a, uint8 b, uint8 c, uint8 d, uint16 port ) : m_port( port )
  {
    m_address = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
    m_string[0] = 0;
  }

  Address::Address( const char* addr, uint16 port )
  {
    int index = 0;
    uint8 octets[4] = { 0 };

    if( addr )
    {
      while( *addr && index < 4 )
      {
        if( *addr != '.' )
        {
          octets[index] *= 10;
          octets[index] += (*addr - '0');
        }
        else
        {
          ++index;
        }

        ++addr;
      }
    }

    *this = Address( octets[0], octets[1], octets[2], octets[3], port );
  }

  Address::Address( uint32 address, uint16 port ) : m_port( port ),
                                                    m_address( address )
  {
    m_string[0] = 0;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Public functions
  //////////////////////////////////////////////////////////////////////////////
  uint8 Address::A() const
  {
    return static_cast<uint8>( m_address >> 24 );
  }

  uint8 Address::B() const
  {
    return static_cast<uint8>( m_address >> 16 );
  }

  uint8 Address::C() const
  {
    return static_cast<uint8>( m_address >> 8 );
  }

  uint8 Address::D() const
  {
    return static_cast<uint8>( m_address );
  }

  uint16 Address::Port() const
  {
    return m_port;
  }

  uint32 Address::FullAddress() const
  {
    return m_address;
  }

  const char* Address::ToString() const
  {
    BuildString();
    return m_string;
  }

  Address::operator bool() const
  {
    return (m_address != 0) && (m_port != 0);
  }

  bool Address::operator<( const Address& rhs ) const
  {
    if( m_address < rhs.m_address )
      return true;
    else if( m_address > rhs.m_address )
      return false;
    else
      return m_port < rhs.m_port;
  }

  bool Address::operator>( const Address& rhs ) const
  {
    return !this->operator<( rhs );
  }

  bool Address::operator==( const Address& rhs ) const
  {
    return (m_address == rhs.m_address) && (m_port == rhs.m_port);
  }

  bool Address::operator!=( const Address& rhs ) const
  {
    return !this->operator==( rhs );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Private Methods
  //////////////////////////////////////////////////////////////////////////////
  void Address::BuildString() const
  {
    uint8 nums[4] = { A(), B(), C(), D() };
    
    int pos = 0;
    
    for( int i = 0; i < 4; ++i )
    {
      char hundreds = nums[i] / 100;
      char tens = ( nums[i] / 10 ) % 10;
      char ones = nums[i] % 10;
      
      if( hundreds ) m_string[pos++] = '0' + hundreds;
      if( tens ) m_string[pos++] = '0' + tens;
      m_string[pos++] = '0' + ones;

      if( i < 3 ) m_string[pos++] = '.';
    }
    
    m_string[pos] = '\0';
  }

} // Networking namespace
} // SF namespace
