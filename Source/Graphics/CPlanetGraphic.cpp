/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CPlanetGraphic.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:30:16 CTallman
**  \brief   Draws the planet component.
*******************************************************************************/

#include "Utilities/Misc.h"
#include "CPlanetGraphic.h"
#include "Utilities/Image.h"
#include "SolarFlare/Globals.h"
#include "Engine/AssetManager.h"
#include "Threading/TaskManager.h"
#include "Utilities/Noise/AllNoise.h"
#include "Utilities/Random/Random.h"
#include "Utilities/LeakCheck.h"


namespace SF {
namespace Graphics {
namespace Components
{
  //************************************
  // Method:    CPlanetGraphic
  // FullName:  SF::Graphics::Components::CPlanetGraphic::CPlanetGraphic
  // Access:    private 
  // Returns:   
  // Qualifier:
  //************************************
  CPlanetGraphic::CPlanetGraphic()
  {
    this->type = Engine::ComponentType::PLANET;

    height_buffer               = new VertexBuffer3<Vertex::HeightVertex>();
    back_height_buffer          = new VertexBuffer3<Vertex::HeightVertex>();
    frag                        = new CgShader();
    vert                        = new CgShader();
    deformVert                  = Globals::shaderLoader->GetVertexShader("deform_vert");
    deformFrag                  = Globals::shaderLoader->GetFragmentShader("deform_frag");
    cg_min_radius               = new ShaderParameter();
    cg_max_radius               = new ShaderParameter();
    cg_tex_width                = new ShaderParameter();
    cg_tex_height               = new ShaderParameter();

    min_radius = 500;
    max_radius = 500;
    updated_heights = false;
    deform_faces = 0;
    deform_delay = (unsigned char)Globals::rRandom->RandInt(10);
  }

  //************************************
  // Method:    ~CPlanetGraphic
  // FullName:  SF::Graphics::Components::CPlanetGraphic::~CPlanetGraphic
  // Access:    public 
  // Returns:   
  // Qualifier:
  //************************************
  CPlanetGraphic::~CPlanetGraphic()
  {
    WaitForTasks();

    delete height_buffer;
    delete back_height_buffer;
  }

  void CPlanetGraphic::SetVertexShader( SharedShader2 shader )
  {
    CSunGraphic::SetVertexShader(shader);
  }

  void CPlanetGraphic::SetFragmentShader( SharedShader2 shader )
  {
    CSunGraphic::SetFragmentShader(shader);
    cg_tex_width = frag->GetParameter("texwidth");
    cg_tex_height = frag->GetParameter("texheight");
  }

  void CPlanetGraphic::SetVertexShader( const std::string & shader )
  {
    CSunGraphic::SetVertexShader(shader);
  }

  void CPlanetGraphic::SetFragmentShader( const std::string & shader )
  {
    CSunGraphic::SetFragmentShader(shader);
    cg_tex_width = frag->GetParameter("texwidth");
    cg_tex_height = frag->GetParameter("texheight");
  }

  void CPlanetGraphic::DeepCopy( const CPlanetGraphic & planet )
  {
    min_radius = planet.min_radius;
    max_radius = planet.max_radius;
    //crater_texture = planet.crater_texture;
    color_texture = planet.color_texture;
    height_texture.ShallowCopy( planet.height_texture );
    deform_texture.ShallowCopy( planet.deform_texture );
    updated_heights = false;
  }

  //************************************
  // Method:    PushToLua
  // FullName:  SF::Graphics::Components::CPlanetGraphic::PushToLua
  // Access:    public 
  // Returns:   void
  // Qualifier:
  // Parameter: lua_State * ls
  //************************************
  void CPlanetGraphic::PushToLua( lua_State *ls )
  {
    Lunar<CPlanetGraphic>::push( ls, this );
  }

  // Networking
  void CPlanetGraphic::InitIn( Networking::ByteStream& bs )
  {
    CSunGraphic::InitIn( bs );
    bs >> color_texture;
  }

  void CPlanetGraphic::InitOut( Networking::ByteStream& bs ) const
  {
    CSunGraphic::InitOut( bs );
    bs << color_texture;
  }

  //////////////////////////////////////////////////////////////////////////
  // Lua Methods
  //////////////////////////////////////////////////////////////////////////
  LUA_START_DEFINE_CLASS( CPlanetGraphic )
    LUA_NAME_METHOD( CPlanetGraphic, "GetMinRadius", LGetMinRadius )
    LUA_NAME_METHOD( CPlanetGraphic, "SetMinRadius", LSetMinRadius )
    LUA_NAME_METHOD( CPlanetGraphic, "GetMaxRadius", LGetMaxRadius )
    LUA_NAME_METHOD( CPlanetGraphic, "SetMaxRadius", LSetMaxRadius )
    LUA_NAME_METHOD( CPlanetGraphic, "GetHeight", LGetHeight )
    LUA_NAME_METHOD( CPlanetGraphic, "SetNoise", LSetNoise )
    LUA_NAME_METHOD( CPlanetGraphic, "Deform", LDeform )
    LUA_NAME_METHOD( CPlanetGraphic, "DeepCopy", LDeepCopy )
    LUA_NAME_METHOD( CPlanetGraphic, "SetModel", LSetModel )
  LUA_END_DEFINE_CLASS

  LUA_DEFINE_METHOD( CPlanetGraphic, LGetMinRadius )
  {
    return CSunGraphic::LGetMinRadius( ls );
  }

  LUA_DEFINE_METHOD( CPlanetGraphic, LSetMinRadius )
  {
    return CSunGraphic::LSetMinRadius( ls );
  }

  LUA_DEFINE_METHOD( CPlanetGraphic, LGetMaxRadius )
  {
    return CSunGraphic::LGetMaxRadius( ls );
  }

  LUA_DEFINE_METHOD( CPlanetGraphic, LSetMaxRadius )
  {
    return CSunGraphic::LSetMaxRadius( ls );
  }

  LUA_DEFINE_METHOD( CPlanetGraphic, LGetHeight )
  {
    Math::Vector3 v;

    v.x           = luaL_checkfloat( ls, 1 );
    v.y           = luaL_checkfloat( ls, 2 );
    v.z           = luaL_checkfloat( ls, 3 );
    float theta   = luaL_checkfloat( ls, 4 );
    float phi     = luaL_checkfloat( ls, 5 );

    // Note: Planet texture coordinates shifted along u-axis by pi/2
    float s = (theta + Math::Constants::f_pi_2) / Math::Constants::p_2_pi;
    float t = (phi + Math::Constants::f_pi_2) / Math::Constants::pi;
    Color4f noise_height = height_texture.GetTexel( s, t );
    Color4f deform_height = deform_texture.GetTexel(v.Normalized());

    lua_pushnumber( ls, deform_height.r * noise_height.r );
    return 1;
  }

  LUA_DEFINE_METHOD( CPlanetGraphic, LSetNoise )
  {
    return CSunGraphic::LSetNoise( ls );
  }

  LUA_DEFINE_METHOD( CPlanetGraphic, LDeform )
  {
    // x, y, z, phiRange, thetaRange, intensity, falloff
    Math::Vector3 v;
    
    v.x = luaL_checkfloat( ls, 1 );
    v.y = luaL_checkfloat( ls, 2 );
    v.z = luaL_checkfloat( ls, 3 );
    float scale = (float)lua_tonumber( ls, 4 );
    if( scale == 0 )
      scale = 1;
    
    AddDeform( v, scale );
    
    return 0;
  }

  LUA_DEFINE_METHOD( CPlanetGraphic, LDeepCopy )
  {
    CPlanetGraphic * copy = Lunar<CPlanetGraphic>::check(ls, 1);
    DeepCopy(*copy);

    return 0;
  }

  LUA_DEFINE_METHOD( CPlanetGraphic, LSetModel )
  {
    const char * mappedName = luaL_checkstring(ls, 1);
    SetModel(mappedName);

    return 0;
  }

} // Components namespace
} // Graphics namespace
} // SF namespace
