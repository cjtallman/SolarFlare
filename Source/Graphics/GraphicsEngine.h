/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    GraphicsEngine.h
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/12/2009 @ 01:00 - CJT 
**  \brief   This is the Mother of all Graphics files.
*******************************************************************************/

#ifndef SF_GRAPHICS_ENGINE_H
#define SF_GRAPHICS_ENGINE_H

#include "QuatCamera.h"
#include "CFrustum.h"

#include "Engine/Entity.h"
#include "Engine/BaseEngine.h"

#include "Math/LineSegment.h"

#include "Bloom.h"
#include "FrameBuffer2.h"

#include "Renderer.h"

#include "CActionRegion.h"

#include <vector>
#include <stack>
#include <set>

// Forward declarations
namespace SF { namespace Math { struct Ray; } }
namespace SF { namespace Engine { class EntityFactory; } }

namespace SF {
namespace Graphics
{

  class GraphicsEngine : public SF::Engine::BaseEngine
  {
    //////////////////////////////////////////////////////////////////////////
    /// Private Types
    //////////////////////////////////////////////////////////////////////////


  public:
    //////////////////////////////////////////////////////////////////////////
    /// Public Methods
    //////////////////////////////////////////////////////////////////////////
    GraphicsEngine();
    ~GraphicsEngine();

    static Math::Ray ScreenToWorld( int x, int y );
    static Math::Vector3 ScreenToWorldPoint(int x, int y);
    static Math::Vector3 WorldToScreenPoint(const Math::Vector3& point);

    void SetupWindow( void* data1, void* data2, int width=1024, int height=768, Color bg_color = Color() );
    void Init();
   
    virtual void Update( float dt );
    void UpdateCamera( float dt );
    
    void RenderPostProcess( bool enable );
    bool RenderPostProcess();

    int WindowWidth() { return renderer.WindowWidth(); }
    int WindowHeight() { return renderer.WindowHeight(); }

    void AddSun( int entity_id );
    void AddPlanet( int entity_id );
    void AddSkybox( int entity_id );
    void AddModel( int entity_id );
    void AddEllipse( int entity_id );
    void AddText( int entity_id );
    void AddWorldQuad( int entity_id );
    void AddUI( int entity_id );

    void RemoveSun( int entity_id );
    void RemovePlanet( int entity_id );
    void RemoveSkybox( int entity_id );
    void RemoveModel( int entity_id );
    void RemoveEllipse( int entity_id );
    void RemoveText( int entity_id );
    void RemoveWorldQuad( int entity_id );
    void RemoveUI( int entity_id );

    void DrawString( int x, int y, const std::string & text );
    void AddLine( const Math::Vector3& start, const Math::Vector3& end );

    Math::Vector3 CameraView() const;
    Math::Vector3 CameraPosition() const;

    virtual void HandleMessage( const Engine::Message* );

    Renderer::OpenGL & GetRenderer() { return renderer; }


    const CFrustum & GetFrustum() const { return m_frustum; }
    const QuatCamera & GetCamera() const { return m_camera; }

    CActionRegion actionRegion;

  private:
    //////////////////////////////////////////////////////////////////////////
    /// Private Methods
    //////////////////////////////////////////////////////////////////////////
    void RegisterComponentCreators();

    void UpdateComponents( float elapsed );
    void RenderComponents( float elapsed );
    void DrawParticles();
    void DrawConsole();
    void DrawLines();
    void DrawUI();
    void DrawFPS( float time_elapsed );
    void DrawCursor( float time_elapsed );
    
    void DrawEllipses( float elapsed );
    void DrawModels( float elapsed );
    void DrawParticles( float elapsed );
    void DrawPlanets( float elapsed );
    void DrawSkybox( float elapsed );
    void DrawSun( float elapsed );
    void DrawWorldQuads( float elapsed );

    void SetCursor( const std::string & mappedName );
    void ShowCursor(bool show);
    

    void SetSunLight();

    void ResetLights();

    

    //////////////////////////////////////////////////////////////////////////
    /// Private Data
    //////////////////////////////////////////////////////////////////////////

    Renderer::OpenGL renderer;

    Bloom * bloom;
    FrameBuffer2 * frame_buffer;

    Engine::EntityFactory * entity_factory;

    typedef std::set<Engine::EntityID> EntitySet;
    EntitySet sun_components;
    EntitySet planet_components;
    EntitySet skybox_components;
    EntitySet model_components;
    EntitySet ellipse_components;
    EntitySet text_components;
    EntitySet worldquad_components;
    EntitySet ui_components;
    std::vector<Math::LineSegment> m_lines;

    CFrustum m_frustum;
    QuatCamera  m_camera;
    bool post_process_on;

    int m_fps;

    SharedTexture cursorTexture;
    bool mShowCursor;

    void CameraShake(float time, Matrix4 & model_view);
    void GenShakePositions();
    std::vector<Math::Vector3> camera_shake_positions;
    Matrix4 camera_shake;
    float camera_shake_total_time;
    float camera_shake_current_time;
    float camera_shake_intensity;

    LUA_DECLARE_CLASS( GraphicsEngine )
      LUA_DECLARE_METHOD( LGetFPS )
      LUA_DECLARE_METHOD( LDrawLine )
      LUA_DECLARE_METHOD( LWireframe )
      LUA_DECLARE_METHOD( LPostProcess )
      LUA_DECLARE_METHOD( LGetCamera )
      LUA_DECLARE_METHOD( LGetScreenSize )
      LUA_DECLARE_METHOD( LScreenToWorld )
      LUA_DECLARE_METHOD( LProjectVector )
      LUA_DECLARE_METHOD( LUnprojectVector )
      LUA_DECLARE_METHOD( LSetAspectRatio )
      LUA_DECLARE_METHOD( LGetAspectRatio )
      LUA_DECLARE_METHOD( LSetViewingAngle )
      LUA_DECLARE_METHOD( LGetViewingAngle )
      LUA_DECLARE_METHOD( LSetGraphicsQuality )
      LUA_DECLARE_METHOD( LGetGraphicsQuality )
      LUA_DECLARE_METHOD( LSetVSync )
      LUA_DECLARE_METHOD( LGetVSync )
      LUA_DECLARE_METHOD( LSetLightDiffuse )
      LUA_DECLARE_METHOD( LSetLightSpecular )
      LUA_DECLARE_METHOD( LSetLightAmbient )
      LUA_DECLARE_METHOD( LSetLightPercent )
      LUA_DECLARE_METHOD( LResetLights )
      LUA_DECLARE_METHOD( LShakeCamera )
      LUA_DECLARE_METHOD( LAction )
      LUA_DECLARE_METHOD( LGetActionRegion )
      LUA_DECLARE_METHOD( LClearActions )
      LUA_DECLARE_METHOD( LSetCursor )
      LUA_DECLARE_METHOD( LShowCursor )


  };

} // Graphics namespace
} // SF namespace

#endif //SF_GRAPHICS_ENGINE_H
