/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Globals.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/08/2009 @ 15:41:26 - nwilliam
**  \brief   Include for, well, globals.
*******************************************************************************/

#ifndef SF_GLOBALS_H
#define SF_GLOBALS_H

namespace SF {
  
  // Forward declarations
  namespace Utilities       { class DevConsole; class SNCG; class Timer; }
  namespace Threading       { class TaskManager;    }
  namespace Script          { class ScriptEngine;   }
  namespace Graphics        { class GraphicsEngine; class ShaderLoader; }
  namespace Engine          { class AssetManager;  class InputEngine;
                              class EntityFactory; class MessageEngine; }
  namespace Physics         { class PhysicsEngine;  }
  namespace ParticleSystem  { class ParticleEngine; }
  namespace UI              { class MenuSystem;     }
  namespace Networking      { class NetworkEngine;  }
  namespace Sound           { class SoundEngine;    }

  namespace Globals
  {
    void Init();
    void Destroy();

    extern class Utilities::SNCG* dRandom;
    extern class Utilities::SNCG* rRandom;

    extern class Utilities::DevConsole* console;
    extern class Engine::AssetManager* assetManager;
    extern class Engine::EntityFactory* entityFactory;
    extern class Engine::InputEngine* input;
    extern class Utilities::Timer* timer;

    
    extern class Engine::MessageEngine* messageEngine;
    extern class Threading::TaskManager* taskManager;
    extern class Script::ScriptEngine* scriptEngine;
    extern class Physics::PhysicsEngine* physicsEngine;
    extern class Graphics::GraphicsEngine* graphicsEngine;
    extern class Graphics::ShaderLoader* shaderLoader;
    extern class ParticleSystem::ParticleEngine* particleEngine;
    extern class UI::MenuSystem* menuSystem;
    extern class Networking::NetworkEngine* networkEngine;
    extern class Sound::SoundEngine* soundEngine;
  }

} // SF namespace

#endif // SF_GLOBALS_H
