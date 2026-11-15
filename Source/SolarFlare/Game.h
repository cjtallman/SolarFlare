/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Game.h
**  \author  Scorched Muffin Entertainment
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 00:38:52 - nwilliam
**  \brief   Interface for the 'game'. Where all loading, updated, etc happens.
*******************************************************************************/

#ifndef SF_GAME_H
#define SF_GAME_H

#include <map>

#include "Utilities/Timer.h"
#include "Script/ScriptHelper.h"
#include "Utilities/Window/Window.h"

namespace SF
{
  class Game
  {
    public:
      Game();
      ~Game();

      void Run();

    private:
      void InitAssetLoaders();
      void InitComponentCreators();
      void AddEngineTime(const std::string& engineName);

      bool m_running;
      Utilities::Window m_window;

      Utilities::Timer m_engineTimer;
      unsigned int m_engineTimerCount;
      std::map<std::string, double> m_engineTimes;

      LUA_DECLARE_CLASS( Game )
      LUA_DECLARE_METHOD( LQuit )
      LUA_DECLARE_METHOD( LEngineTimes )
      LUA_DECLARE_METHOD( LReloadAsset )
      LUA_DECLARE_METHOD( LGameElapsed )
      LUA_DECLARE_METHOD( LFrameElapsed )
      LUA_DECLARE_METHOD( LFullscreenOn )
      LUA_DECLARE_METHOD( LMouseCentered )
  };

} // SF namespace

#endif // SF_GAME_H
