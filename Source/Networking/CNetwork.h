/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CNetwork.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/14/2009 @ 16:19:46 - nwilliam
**  \brief   Network component.
*******************************************************************************/

#ifndef SF_C_NETWORK_H
#define SF_C_NETWORK_H

#include "Engine/Entity.h"
#include "Engine/BaseComponent.h"

// Forward declaration
namespace ticpp { class Element; }

namespace SF {
namespace Networking
{
  class CNetwork : public Engine::BaseComponent
  {
    public:
      CNetwork();
      virtual ~CNetwork();
      
      // Default copy, assignment ok
      
      virtual Engine::BaseComponent* Clone() const;
      virtual bool InitDependencies( Engine::Entity* );
      
      static Engine::BaseComponent* Creator( const ticpp::Element* );
      
    private:
      friend class NetworkEngine;
  };
  
} // Networking namespace
} // SF namespace

#endif // SF_C_NETWORK_H
