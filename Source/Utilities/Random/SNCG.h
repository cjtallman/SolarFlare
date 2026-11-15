/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    SNCG.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 11/11/2008 @ 16:04:00 - nwilliam
**  \brief   A 'Small Non-Cryptographic Psuedorandom Number Generator'.
             Thanks to Bob Jenkins:
              http://burtleburtle.net/bob/rand/smallprng.html
*******************************************************************************/

#ifndef SF_SNCG_H
#define SF_SNCG_H

#include "RNG.h"

namespace SF {
namespace Utilities
{
  class SNCG : public RNG< SNCG >
  {
    public:
      SNCG();
      
      // Default copy, assignment, destructor ok
      
      void Seed( int seed );
      unsigned int RandVal();
      
    private:
      friend Networking::ByteStream& operator>>( Networking::ByteStream&, SNCG& );
      friend Networking::ByteStream& operator<<( Networking::ByteStream&, const SNCG& );
      
      void Init( int seed );
      
      int m_seed;
      unsigned int m_a;
      unsigned int m_b;
      unsigned int m_c;
      unsigned int m_d;
  };
  
  Networking::ByteStream& operator>>( Networking::ByteStream&, SNCG& );
  Networking::ByteStream& operator<<( Networking::ByteStream&, const SNCG& );
  
} // Utilities namespace
} // SF

#endif // SF_SNCG_H
