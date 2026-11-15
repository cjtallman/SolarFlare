/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CNetwork.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/14/2009 @ 16:19:53 - nwilliam
**  \brief   Network component implementation.
*******************************************************************************/

#include "CNetwork.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Networking
{
  //////////////////////////////////////////////////////////////////////////////
  // 
  //////////////////////////////////////////////////////////////////////////////
  CNetwork::CNetwork() : BaseComponent( Engine::ComponentType::NETWORK )
  {
  }
  
  CNetwork::~CNetwork()
  {
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  Engine::BaseComponent* CNetwork::Clone() const
  {
    return new CNetwork( *this );
  }
  
  bool CNetwork::InitDependencies( Engine::Entity* )
  {
    return true;
  }
  
  Engine::BaseComponent* CNetwork::Creator( const ticpp::Element* )
  {
    return new CNetwork();
  }
  
} // Networking engine
} // SF namespace
