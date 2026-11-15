/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    GraphicsEngine_Base.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 02/12/2009 @ 01:00 - CJT 
**  \brief   This file is responsible for initialization and basic functions.
*******************************************************************************/

#include "GraphicsEngine.h"
#include "SolarFlare/Globals.h"       // console, messageEngine
#include "Engine/MessageEngine.h"
#include "Engine/EntityFactory.h"
#include "Utilities/DevConsole.h"
#include "Utilities/Macros.h"
#include "GraphicsComponents.h"
#include "StateManager.h"
#include "Utilities/Random/Random.h"

#if defined(_WIN32)
  #include <windows.h>
  #include "OpenGLInclude.h"
#elif defined(__linux)
  #include "OpenGLInclude.h"
  #include <X11/Xlib.h>
  #include <GL/glx.h>
#else
  #error "GraphicsEngine not tested on this OS"
#endif

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Graphics
{

  


  void GraphicsEngine::RegisterComponentCreators()
  {
    entity_factory = SF::Globals::entityFactory;

    entity_factory->CreateNotifier( Engine::ComponentType::UI_GRAPHIC, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::AddUI >(this) );
    entity_factory->DeleteNotifier( Engine::ComponentType::UI_GRAPHIC, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::RemoveUI >(this) );
    entity_factory->CreateNotifier( Engine::ComponentType::SUN, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::AddSun >(this) );
    entity_factory->DeleteNotifier( Engine::ComponentType::SUN, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::RemoveSun >(this) );
    entity_factory->CreateNotifier( Engine::ComponentType::PLANET, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::AddPlanet >(this) );
    entity_factory->DeleteNotifier( Engine::ComponentType::PLANET, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::RemovePlanet >(this) );
    entity_factory->CreateNotifier( Engine::ComponentType::SKYBOX, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::AddSkybox >(this) );
    entity_factory->DeleteNotifier( Engine::ComponentType::SKYBOX, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::RemoveSkybox >(this) );
    entity_factory->CreateNotifier( Engine::ComponentType::MODEL, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::AddModel >(this) );
    entity_factory->DeleteNotifier( Engine::ComponentType::MODEL, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::RemoveModel >(this) );
    entity_factory->CreateNotifier( Engine::ComponentType::ELLIPSE, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::AddEllipse >(this) );
    entity_factory->DeleteNotifier( Engine::ComponentType::ELLIPSE, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::RemoveEllipse >(this) );
    entity_factory->CreateNotifier( Engine::ComponentType::TEXT, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::AddText >(this) );
    entity_factory->DeleteNotifier( Engine::ComponentType::TEXT, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::RemoveText >(this) );
    entity_factory->CreateNotifier( Engine::ComponentType::WORLD_QUAD, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::AddWorldQuad >(this) );
    entity_factory->DeleteNotifier( Engine::ComponentType::WORLD_QUAD, Engine::Notifier::Method< GraphicsEngine, &GraphicsEngine::RemoveWorldQuad >(this) );

    entity_factory->Creator( "PLANET", Engine::CCreator::Function< Components::CreatePlanetGraphic >() );
    entity_factory->Creator( "SUN", Engine::CCreator::Function< Components::CreateSunGraphic >() );
    entity_factory->Creator( "SKYBOX", Engine::CCreator::Function< Components::CreateSkyboxComponent >() );
    entity_factory->Creator( "MODEL", Engine::CCreator::Function< Components::CreateModel >() );
    entity_factory->Creator( "INTERFACEQUAD", Engine::CCreator::Function< Components::CInterfaceQuad::CreateComponent >() );
    entity_factory->Creator( "TOOLTIP", Engine::CCreator::Function< Components::CToolTip::CreateComponent >() );
    entity_factory->Creator( "INTERFACEPANEL", Engine::CCreator::Function< Components::CInterfacePanel::CreateComponent >() );
    entity_factory->Creator( "BUILDBUTTON", Engine::CCreator::Function< Components::CBuildButton::CreateComponent >() );
    entity_factory->Creator( "ELLIPSE", Engine::CCreator::Function< Components::CEllipse::CreateComponent >() );
    entity_factory->Creator( "TEXT", Engine::CCreator::Function< Components::CText::CreateComponent >() );
    entity_factory->Creator( "WORLD_QUAD", Engine::CCreator::Function< Components::CWorldQuad::CreateComponent >() );
  }

  GraphicsEngine::GraphicsEngine()
  {
    bloom = 0;
    frame_buffer = 0;
    post_process_on = true;

    m_fps = 0;

    RegisterComponentCreators();
    Globals::messageEngine->AddSubscription( "WindowResize", this );
    ShowCursor(false);    
  }

  GraphicsEngine::~GraphicsEngine()
  {
    delete bloom;
    delete frame_buffer;
  }

  void GraphicsEngine::SetupWindow( void * data1, void * data2, int width, int height, Color bg_color )
  {
    renderer.Init( data1, data2 );
    renderer.CheckMinimumRequirements();
    renderer.ColorClearValue( bg_color );
    renderer.WindowWidth( width );
    renderer.WindowHeight( height );
    renderer.Begin();
    renderer.End();
  }

  void GraphicsEngine::Init()
  {
    renderer.PerspectiveProjection(90, 1);
    glGetFloatv( GL_PROJECTION_MATRIX, SquarePerspectiveMatrix().Array() );
    int width = renderer.WindowWidth();
    int height = renderer.WindowHeight();
    double aspect = static_cast<double>(width) / static_cast<double>(height);
    renderer.Viewport(0,0,width,height);
    renderer.VSync(false);
    renderer.AspectRatio(aspect);
    renderer.PerspectiveProjection();
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glShadeModel( GL_SMOOTH );

    frame_buffer = new FrameBuffer2( width, height, GL_HALF_FLOAT_ARB, 24 );
    bloom = new Bloom(width,height);
    
    ResetLights();
    GenShakePositions();

    actionRegion.SetRegion(0,width,300,width-300);
    actionRegion.SetFont("stentiga");
    actionRegion.SetFontSize(20);
    actionRegion.SetFragmentShader("text_frag");
    actionRegion.SetTexture(0,"hit_icon");
    actionRegion.SetTexture(1,"death_icon");
    actionRegion.SetTexture(2,"asteroid_icon");
    SetCursor("arrowCursor");
  }

  void GraphicsEngine::ResetLights()
  {
    renderer.SetLightAmbient( Color4f(.5f,.5f,.5f) );
    renderer.SetLightDiffuse( Color4f(1.0f,1.0f,1.0f) );
    renderer.SetLightSpecular( Color4f(.6f,.6f,.6f) );
  }

  void GraphicsEngine::GenShakePositions()
  {
    camera_shake_positions.clear();
    for( int i = 0; i < 50; ++i )
    {
      camera_shake_positions.push_back(  Math::Vector3( 
                                         float((2 * Globals::dRandom->Rand()) - 1.0), 
                                         float((2 * Globals::dRandom->Rand()) - 1.0), 
                                         float((2 * Globals::dRandom->Rand()) - 1.0)
                                         ));
    }

    camera_shake_total_time = 1;
    camera_shake_current_time = 0;
    camera_shake_intensity = 1;
  }

  void GraphicsEngine::CameraShake(float time, Matrix4 & model_view )
  {
    model_view = m_camera.GetViewMatrix();

    if( camera_shake_current_time > 0 )
    {
      camera_shake_current_time -= time;

      float percent = (camera_shake_current_time / camera_shake_total_time);

      int num_points = (int)camera_shake_positions.size();

      int id0 = Utilities::Clamp<int>((int)(percent * num_points), 0, num_points-1);
      int id1 = Utilities::Clamp<int>(id0 - 1,              0, num_points-1);

      Vector3 trans0 = camera_shake_positions[id0];
      Vector3 trans1 = camera_shake_positions[id1];

      float t = float(percent*num_points) - int(percent*num_points);

      Vector3 translate = percent * ((t) * trans0 + (1 - t) * trans1);
      
      model_view.Translate(camera_shake_intensity * translate.x, camera_shake_intensity * translate.y, camera_shake_intensity * translate.z);
    }
  }

  void GraphicsEngine::UpdateCamera( float elapsed )
  {
    m_camera.Update(elapsed);

    Matrix4 model_view = m_camera.GetViewMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf( model_view.Array() );

    m_frustum.SetModelview( model_view );
  }

  void GraphicsEngine::Update( float time_elapsed )
  {
    //CheckOpenGLError("OpenGL error found at beginning of GraphicsEngine::Update");

    renderer.Begin();

    Matrix4 projectionMatrix;
    glGetFloatv( GL_PROJECTION_MATRIX, projectionMatrix.Array() );

    SetSunLight();

    m_frustum.SetProjection( projectionMatrix );

    bool wire = renderer.Wireframe();

    if( post_process_on )
    {
      frame_buffer->BeginCapture();
      frame_buffer->ClearTarget();
      RenderComponents( time_elapsed );
      frame_buffer->EndCapture();
      frame_buffer->SwapTargets();

      if( wire )
        renderer.Wireframe(false);

      frame_buffer->BindTexture(0);
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      glViewport( 0,0, renderer.WindowWidth(), renderer.WindowHeight() );
      DrawScreenQuad();
      glPopAttrib();
      frame_buffer->UnbindTexture(0);


      bloom->Process(frame_buffer);


      if( wire )
        renderer.Wireframe(true);
    }
    else
    {
      RenderComponents(time_elapsed);
    }

    if( wire )
      renderer.Wireframe(false);



    DrawLines();
    DrawUI();
    DrawFPS(time_elapsed);
    actionRegion.Draw(time_elapsed);
    DrawCursor(time_elapsed);
    DrawConsole();

    if( wire )
      renderer.Wireframe(true);

    renderer.End();

    //CheckOpenGLError("OpenGL error found at end of GraphicsEngine::Update");
  }

  

  void GraphicsEngine::SetSunLight()
  {
    // Put light to view space. In the shader we will take it to model space.
    static float lightPosition[4] = { 0, 0, 0, 1 };
    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );
  }

  void GraphicsEngine::AddSun( int entity_id )
  {
    sun_components.insert( entity_id );
  }

  void GraphicsEngine::RemoveSun( int entity_id )
  {
    sun_components.erase( entity_id );
  }

  void GraphicsEngine::AddPlanet( int entity_id )
  {
    planet_components.insert( entity_id );
  }

  void GraphicsEngine::RemovePlanet( int entity_id )
  {
    planet_components.erase( entity_id );
  }

  void GraphicsEngine::AddSkybox( int entity_id )
  {
    skybox_components.insert( entity_id );
  }

  void GraphicsEngine::RemoveSkybox( int entity_id )
  {
    skybox_components.erase( entity_id );
  }

  void GraphicsEngine::AddModel( int entity_id )
  {
    model_components.insert( entity_id );
  }

  void GraphicsEngine::RemoveModel( int entity_id )
  {
    model_components.erase( entity_id );
  }

  void GraphicsEngine::AddEllipse( int entity_id )
  {
    ellipse_components.insert( entity_id );
  }

  void GraphicsEngine::RemoveEllipse( int entity_id )
  {
    ellipse_components.erase( entity_id );
  }

  void GraphicsEngine::AddText( int entity_id )
  {
    text_components.insert( entity_id );
  }

  void GraphicsEngine::RemoveText( int entity_id )
  {
    text_components.erase( entity_id );
  }

  void GraphicsEngine::AddWorldQuad( int entity_id )
  {
    worldquad_components.insert( entity_id );
  }

  void GraphicsEngine::RemoveWorldQuad( int entity_id )
  {
    worldquad_components.erase( entity_id );
  }

  void GraphicsEngine::AddUI( int entity_id )
  {
    ui_components.insert(entity_id);
  }

  void GraphicsEngine::RemoveUI( int entity_id )
  {
    ui_components.erase(entity_id);
  }

  Math::Vector3 GraphicsEngine::CameraView() const
  {
    return m_camera.GetOrientation().LocalZ().Normalize();
  }

  Math::Vector3 GraphicsEngine::CameraPosition() const
  {
    return m_camera.GetPosition();
  }

  void GraphicsEngine::HandleMessage( const Engine::Message* msg )
  {
    if( msg->type == "WindowResize" )
    {
      
        int w = msg->stack.Get<int>(0);
        int h = msg->stack.Get<int>(1);
        if( w > 0 && h > 0 )
        {
          frame_buffer->Create( w, h, GL_HALF_FLOAT_ARB, 24 );
          //frame_buffer.Create( 640, 480, GL_HALF_FLOAT_ARB, 24 );

          renderer.ResizeWindow( w, h );

          actionRegion.SetRegion(0,w,300,w-300);
        }
    }
  }

  void GraphicsEngine::RenderPostProcess( bool enable )
  {
    post_process_on = enable;
  }

  bool GraphicsEngine::RenderPostProcess()
  {
    return post_process_on;
  } 

} // Graphics namespace
} // SF namespace
