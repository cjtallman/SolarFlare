/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SNCG.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/11/2008 @ 16:19:11 - nwilliam
**  \brief   My implementation of Bob Jenkins' SNCPRN generator.
*******************************************************************************/

#include "SNCG.h"

namespace SF {
namespace Utilities
{
  //////////////////////////////////////////////////////////////////////////////
  // Constructor
  //////////////////////////////////////////////////////////////////////////////
  SNCG::SNCG() : m_seed(),
                 m_a(),
                 m_b(),
                 m_c(),
                 m_d()
  {
    Init( m_seed );
  }
  
  
  ////////////////////////////////////////////////////////////////////////////
  // Public Methods
  ////////////////////////////////////////////////////////////////////////////
  void SNCG::Seed( int seed )
  {
    m_seed = seed;
    Init( m_seed );
  }
  
  
  ////////////////////////////////////////////////////////////////////////////
  // Protected Methods
  ////////////////////////////////////////////////////////////////////////////
  unsigned int SNCG::RandVal()
  {
    #define ROT( x , k ) ( (x) << (k) | ( (x) >> (32 - (k) ) ) )
   
    unsigned int e = m_a - ROT(m_b,27);
    m_a = m_b ^ ROT(m_c,17);
    m_b = m_c + m_d;
    m_c = m_d + e;
    m_d = e + m_a;
    
    #undef ROT
    
    return m_d;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Private Methods
  //////////////////////////////////////////////////////////////////////////////
  void SNCG::Init( int seed )
  {
    // 0xF1EA5EED = 4058668781
    m_a = 0xF1EA5EED; 
    m_b = m_c = m_d = (unsigned int)seed;
    
    for( int i = 0; i < 20; ++i )
    {
      RandVal();
    }
  }
  
  
  
  
  Networking::ByteStream& operator>>( Networking::ByteStream& bs, SNCG& sncg )
  {
    bs >> sncg.m_seed >> sncg.m_a >> sncg.m_b >> sncg.m_c >> sncg.m_d;
    return bs;
  }
  
  Networking::ByteStream& operator<<( Networking::ByteStream& bs, const SNCG& sncg )
  {
    bs << sncg.m_seed << sncg.m_a << sncg.m_b << sncg.m_c << sncg.m_d;
    return bs;
  }
  
} // Utilities namespace
} // SF namespace
