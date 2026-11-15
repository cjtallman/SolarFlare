/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CSunGraphic.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/25/2009 @ 20:04:27 - nwilliam
**  \brief   
*******************************************************************************/

#ifndef SF_CSUN_GRAPHIC_H
#define SF_CSUN_GRAPHIC_H

#include "VertexBuffer3.h"
#include "Math/CTransform.h"

#include "Texture2D.h"
#include "ColorRamp.h"
#include "BaseModel.h"
#include "CFrustum.h"
#include "QuatCamera.h"

#include "External/Lua/include/LunarHelper.h"

#include "ShaderLoader.h"
#include "Utilities/Noise/Noise.h"

namespace SF {
  // Forward declaration
  namespace Utilities { namespace Noise { class SphereGenerator; } }

namespace Graphics {
namespace Components
{

  // Component Creator /////////////////////////////////////////////////////////
  Engine::BaseComponent * CreateSunGraphic( const ticpp::Element * element );

  class CSunGraphic : public Engine::BaseComponent
  {
  public:
    // Public Static Functions
    static void BeginRender();
    static void EndRender();

    // Public Important Stuff
    Engine::BaseComponent * Clone() const;
    bool InitDependencies( Engine::Entity * parent );
    CSunGraphic();
    virtual ~CSunGraphic();

    // Public Drawing Stuff
    void Draw( const CFrustum & frustum );
    void Update( float elapsed );
    
    // Public Setters
    void SetModel( SharedModel new_model ){ model = new_model; }
    void SetModel( const std::string & mappedName );
    
    void SetMinRadius( float _radius ) {min_radius = _radius;}
    void SetMaxRadius( float _radius ) {max_radius = _radius;}
    virtual void SetVertexShader( const std::string & vert_shader );
    virtual void SetVertexShader( SharedShader2 vert_shader );
    virtual void SetFragmentShader( const std::string & frag_shader );
    virtual void SetFragmentShader( SharedShader2 frag_shader );
    
    // Public Getters
    SharedModel GetModel() { return model; }
    float GetMinRadius() const {return min_radius;}
    float GetMaxRadius() const {return max_radius;}

    // Public Misc Stuff
    void AddColor( float value, const SF::Color & color );
    bool IsVisible( const CFrustum & frustum );
    virtual void BuildNoiseTexture( const Utilities::Noise::SphereGenerator& gen );
    
    // Public Networking Stuff
    virtual void InitIn( Networking::ByteStream& bs );
    virtual void InitOut( Networking::ByteStream& bs ) const;

    // Public Scripting Stuff
    virtual void PushToLua( lua_State *ls );

  protected:
    // Private Multithreading Stuff
    typedef std::list<unsigned int> TaskList;
    void WaitForTasks();
    TaskList m_taskList;

    // Private Texture Stuff
    virtual void BindTexture();
    virtual void UnbindTexture();
    void SwapHeightTexture();
    TextureType::Texture2D height_texture;
    SharedTexture temp_texture;
    TextureType::ColorRamp color_texture;

    // Private Noise Stuff
    Utilities::Noise::NoiseData noise_data;

    // Private Model Stuff
    void OrientModel();
    virtual void RenderModel();
    SharedModel model;
    
    // Private Radius Stuff
    float min_radius;
    float max_radius;
    
    // Private Transform Stuff
    Math::CTransform * transform;

    // Private Shader Stuff
    SharedShader2 vert;
    SharedShader2 frag;
    SharedShaderParameter cg_min_radius;
    SharedShaderParameter cg_max_radius;

    LUA_DECLARE_CLASS( CSunGraphic )
    LUA_DECLARE_METHOD( LSetNoise )
    LUA_DECLARE_METHOD( LGetMinRadius )
    LUA_DECLARE_METHOD( LGetMaxRadius )
    LUA_DECLARE_METHOD( LSetMinRadius )
    LUA_DECLARE_METHOD( LSetMaxRadius )
    LUA_DECLARE_METHOD( LStopAnimation )
    LUA_DECLARE_METHOD( LStartAnimation )
    
    NO_COPY( CSunGraphic );
  };

} // Components namespace
} // Graphics namespace
} // SF namespace

#endif // SF_CSUN_GRAPHIC_H_
