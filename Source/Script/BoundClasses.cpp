/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    BoundClasses.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/05/2009 @ 17:42:18 - nwilliam
**  \brief   ScriptEngine function that binds (through Lunar) classes to Lua.
*******************************************************************************/

#include "SolarFlare/Game.h"

#include "ScriptEngine.h"
#include "CScript.h"
#include "Sound/SoundEngine.h"
#include "Engine/InputEngine.h"
#include "Engine/EntityFactory.h"
#include "Graphics/GraphicsEngine.h"
#include "Networking/NetworkEngine.h"

#include "Interface/CRegion.h"
#include "Interface/CButton.h"
#include "Interface/CInputRegion.h"
#include "Interface/CSliderbar.h"
#include "Interface/MenuSystem.h"

#include "Physics/CPhysics.h"
#include "Physics/PhysicsEngine.h"
#include "Physics/COrbitalPhysics.h"
#include "Physics/CExplosionEmitter.h"
#include "Physics/CRocketEmitter.h"
#include "Physics/CConnectionEmitter.h"
#include "Physics/CFlashEmitter.h"

#include "Engine/Entity.h"
#include "Math/CTransform.h"
#include "Graphics/CSunGraphic.h"
#include "Utilities/DevConsole.h"
#include "Graphics/CActionRegion.h"
#include "Graphics/CPlanetGraphic.h"
#include "Graphics/CInterfaceQuad.h"
#include "Graphics/CInterfacePanel.h"
#include "Graphics/CBaseInterfaceGraphic.h"
#include "Graphics/CBuildButton.h"
#include "Graphics/CToolTip.h"
#include "Graphics/CModel.h"
#include "Graphics/CEllipse.h"
#include "Graphics/CSkybox.h"
#include "Graphics/CText.h"
#include "Graphics/CWorldQuad.h"
#include "Sound/CSound.h"

#include "Graphics/QuatCamera.h"

namespace SF {
namespace Script
{
  void ScriptEngine::RegisterScriptClasses()
  {
    lua_State* const ls = m_masterState;
    
    LUA_REGISTER_CLASS( Game, ls );

    LUA_REGISTER_CLASS( CScript, ls );

    LUA_REGISTER_CLASS( Sound::SoundEngine, ls );
    LUA_REGISTER_CLASS( Engine::InputEngine, ls );
    LUA_REGISTER_CLASS( Script::ScriptEngine, ls );
    LUA_REGISTER_CLASS( Engine::EntityFactory, ls );
    LUA_REGISTER_CLASS( Graphics::GraphicsEngine, ls );
    LUA_REGISTER_CLASS( Networking::NetworkEngine, ls );
    
    LUA_REGISTER_CLASS( UI::CButton, ls );
    LUA_REGISTER_CLASS( UI::CRegion, ls );
    LUA_REGISTER_CLASS( UI::CInputRegion, ls );
    LUA_REGISTER_CLASS( UI::CSliderbar, ls );
    LUA_REGISTER_CLASS( UI::MenuSystem, ls );
    LUA_REGISTER_CLASS( UI::CBasicElement, ls );
    
    
    
    LUA_REGISTER_CLASS( Physics::PhysicsEngine, ls );
    LUA_REGISTER_CLASS( Physics::CPhysics, ls );
    LUA_REGISTER_CLASS( Physics::COrbitalPhysics, ls );
    LUA_REGISTER_CLASS( Physics::CExplosionEmitter, ls );
    LUA_REGISTER_CLASS( Physics::CRocketEmitter, ls );
    LUA_REGISTER_CLASS( Physics::CConnectionEmitter, ls );
    LUA_REGISTER_CLASS( Physics::CFlashEmitter, ls );
    

    LUA_REGISTER_CLASS( Engine::Entity, ls );
    LUA_REGISTER_CLASS( Math::CTransform, ls );
    LUA_REGISTER_CLASS( Utilities::DevConsole, ls );
    LUA_REGISTER_CLASS( Graphics::CActionRegion, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CSunGraphic, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CPlanetGraphic, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CInterfaceQuad, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CInterfacePanel, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CBuildButton, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CToolTip, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CBaseInterfaceGraphic, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CModel, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CEllipse, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CSkybox, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CText, ls );
    LUA_REGISTER_CLASS( Graphics::Components::CWorldQuad, ls );
    
    LUA_REGISTER_CLASS( Sound::CSound, ls );

    LUA_REGISTER_CLASS( Graphics::QuatCamera, ls );
  }
  
} // Script namespace
} // SF namespace
