/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Globals.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/08/2009 @ 15:40:49 - nwilliam
**  \brief   Where globals are actually defined.
*******************************************************************************/

#include "Globals.h"
#include "Utilities/Timer.h"
#include "Sound/SoundEngine.h"
#include "Engine/InputEngine.h"
#include "Script/ScriptEngine.h"
#include "Engine/AssetManager.h"
#include "Engine/EntityFactory.h"
#include "Engine/MessageEngine.h"
#include "Utilities/DevConsole.h"
#include "Interface/MenuSystem.h"
#include "Threading/TaskManager.h"
#include "Physics/PhysicsEngine.h"
#include "Graphics/GraphicsEngine.h"
#include "Graphics/ShaderLoader.h"
#include "Graphics/ParticleEngine.h"
#include "Networking/NetworkEngine.h"
#include "Utilities/Random/SNCG.h"

#include "Utilities/LeakCheck.h"

namespace SF
{
  Utilities::SNCG*                Globals::dRandom          = 0;
  Utilities::SNCG*                Globals::rRandom          = 0;

  Utilities::DevConsole*          Globals::console          = 0;
  Engine::AssetManager*           Globals::assetManager     = 0;
  Engine::EntityFactory*          Globals::entityFactory    = 0;
  Engine::InputEngine*            Globals::input            = 0;
  Utilities::Timer*               Globals::timer            = 0;

  Engine::MessageEngine*          Globals::messageEngine    = 0;
  Threading::TaskManager*         Globals::taskManager      = 0;
  Script::ScriptEngine*           Globals::scriptEngine     = 0;
  Physics::PhysicsEngine*         Globals::physicsEngine    = 0;
  Graphics::GraphicsEngine*       Globals::graphicsEngine   = 0;
  Graphics::ShaderLoader*         Globals::shaderLoader     = 0;
  ParticleSystem::ParticleEngine* Globals::particleEngine   = 0;
  UI::MenuSystem *                Globals::menuSystem       = 0;
  Networking::NetworkEngine*      Globals::networkEngine    = 0;
  Sound::SoundEngine*             Globals::soundEngine      = 0;

namespace Globals
{
  void Init()
  {
    Globals::dRandom          = new Utilities::SNCG;
    Globals::rRandom          = new Utilities::SNCG;

    Globals::console          = new Utilities::DevConsole;
    Globals::assetManager     = new Engine::AssetManager;
    Globals::entityFactory    = new Engine::EntityFactory;
    Globals::input            = new Engine::InputEngine;
    Globals::timer            = new Utilities::Timer;

    Globals::messageEngine    = new Engine::MessageEngine;
    Globals::taskManager      = new Threading::TaskManager(1);
    Globals::scriptEngine     = new Script::ScriptEngine;
    Globals::physicsEngine    = new Physics::PhysicsEngine;
    Globals::graphicsEngine   = new Graphics::GraphicsEngine;
    Globals::shaderLoader     = new Graphics::ShaderLoader;
    Globals::particleEngine   = new ParticleSystem::ParticleEngine;
    Globals::menuSystem       = new UI::MenuSystem;
    Globals::networkEngine    = new Networking::NetworkEngine;
    Globals::soundEngine      = new Sound::SoundEngine;
  }

  void Destroy()
  {
    Globals::taskManager->AbortAllTasks();
    Globals::taskManager->WaitForAllTasks();

    delete Globals::dRandom;
    delete Globals::rRandom;

    delete Globals::entityFactory;
    delete Globals::assetManager;
    delete Globals::console;
    delete Globals::input;

    delete Globals::messageEngine;
    delete Globals::scriptEngine;
    delete Globals::physicsEngine;
    delete Globals::graphicsEngine;
    delete Globals::shaderLoader;
    delete Globals::particleEngine;
    delete Globals::menuSystem;
    delete Globals::networkEngine;
    delete Globals::soundEngine;

    delete Globals::taskManager;
  }

} // Globals namespace
} // SF namespace
