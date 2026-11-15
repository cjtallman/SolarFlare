/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BaseEngine.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 10/28/2008 @ 17:06:59 - nwilliam
**  \brief   Base class for all game engines.
*******************************************************************************/

#ifndef SF_BASE_ENGINE_H
#define SF_BASE_ENGINE_H

namespace SF {
namespace Engine
{
  struct Message;
  
  class BaseEngine
  {
    public:
      virtual ~BaseEngine();
      
      virtual void HandleMessage( const Message* );
      virtual void Update( float elapsed ) = 0;
  };
  
} // Engine namespace
} // SF namespace

#endif // SF_BASE_ENGINE_H
