/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CSunGraphic.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 01/28/2009 @ 19:05:36 - nwilliam
**  \brief   
*******************************************************************************/

#include "CSunGraphic.h"
#include "SolarFlare/Globals.h"
#include "Engine/AssetManager.h"
#include "Threading/TaskManager.h"
#include "Utilities/Noise/AllNoise.h"
#include "Utilities/LeakCheck.h"

namespace SF {
  namespace Graphics {
    namespace Components
    {



      CSunGraphic::CSunGraphic()
        : BaseComponent(Engine::ComponentType::SUN)
      {
        model               = new Model::BaseModel();
        temp_texture        = new TextureType::Texture2D();
        frag                = new CgShader();
        vert                = new CgShader();
        cg_min_radius       = new ShaderParameter();
        cg_max_radius       = new ShaderParameter();

        transform = 0;
        min_radius = 500;
        max_radius = 500;    
      }

      CSunGraphic::~CSunGraphic()
      {
        WaitForTasks();
      }

      void CSunGraphic::AddColor( float value, const SF::Color & color )
      {
        color_texture.AddColor( value, Color(color.r, color.g, color.b, color.a) );
      }

      void CSunGraphic::SetModel( const std::string & mappedName )
      { 
        if( !mappedName.empty() )
        {
          Engine::AssetManager * asset_manager = Globals::assetManager;
          Engine::Assets::ModelAsset * asset = (Engine::Assets::ModelAsset*)asset_manager->GetAsset( mappedName );
          if( asset )
            SetModel( SharedModel(asset->model) );
        } 
      }

      void CSunGraphic::SwapHeightTexture()
      {
        height_texture = *temp_texture;
        height_texture.Recreate();

        if( noise_data.animate )
        {
          Utilities::Noise::SphereGenerator gen( noise_data.width, noise_data.height );
          noise_data.time += noise_data.dt;
          gen.SetNoiseData( noise_data );
          BuildNoiseTexture( gen );
        }
      }

      // Noise /////////////////////////////////////////////////////////////////////
      struct FinishedTaskPred
      {
        bool operator()( unsigned int taskID )
        {return !Globals::taskManager->IsValidTaskID( taskID );}
      };

      struct NoiseParams
      {
        NoiseParams( CSunGraphic* sun, SharedTexture tex, const Utilities::Noise::SphereGenerator& sg )
          : component(sun), texture(tex), gen( sg ) {}

        SharedTexture texture;
        CSunGraphic* component;
        mutable Utilities::Noise::SphereGenerator gen;
      };

      void NoiseCreationWrapper( const NoiseParams& p )
      {
        if( p.gen.GetNoiseData().animate )
          p.gen.BuildAnim();
        else
          p.gen.Build();

        p.gen.Array().Scale( p.gen.GetNoiseData().scaleMin, p.gen.GetNoiseData().scaleMax );

        p.texture->MakeHeightMap( p.gen );
      }

      void CSunGraphic::BuildNoiseTexture( const Utilities::Noise::SphereGenerator& gen )
      {
        TaskList::value_type lastTask = m_taskList.empty() ? 0 : m_taskList.back();

        NoiseParams params( this, temp_texture, gen );

        TaskList::value_type taskID;
        taskID = Globals::taskManager->AddTask( NoiseCreationWrapper,
          params,
          Engine::SwapCallback::Method<CSunGraphic, &CSunGraphic::SwapHeightTexture>(this),
          false,
          lastTask );

        // Remove finished taskIDs
        m_taskList.remove_if( FinishedTaskPred() );
        m_taskList.push_back( taskID );
      }

      // Multithreading ////////////////////////////////////////////////////////////


      void CSunGraphic::WaitForTasks()
      {
        TaskList::iterator iter;
        // Abort all tasks
        for( iter = m_taskList.begin(); iter != m_taskList.end(); ++iter)
          Globals::taskManager->AbortTask( *iter );

        // Make sure all tasks finish before we go
        for( iter = m_taskList.begin(); iter != m_taskList.end(); ++iter)
          Globals::taskManager->WaitForAbortedTask( *iter );

        m_taskList.clear();
      }


      // Shaders ///////////////////////////////////////////////////////////////////
      void CSunGraphic::SetVertexShader( SharedShader2 shader )
      {
        vert = shader;
        cg_min_radius = vert->GetParameter("min_radius");
        cg_max_radius = vert->GetParameter("max_radius");
      }

      void CSunGraphic::SetFragmentShader( SharedShader2 shader )
      {
        frag = shader;
      }

      void CSunGraphic::SetVertexShader( const std::string & shader )
      {
        vert = Globals::shaderLoader->GetVertexShader(shader);
        cg_min_radius = vert->GetParameter("min_radius");
        cg_max_radius = vert->GetParameter("max_radius");
      }

      void CSunGraphic::SetFragmentShader( const std::string & shader )
      {
        frag = Globals::shaderLoader->GetFragmentShader(shader);
      }



      // Networking ////////////////////////////////////////////////////////////////
      void CSunGraphic::InitIn( Networking::ByteStream& bs )
      {
        bs >> min_radius >> max_radius >> noise_data;

        Utilities::Noise::SphereGenerator gen;
        gen.SetSize( noise_data.width, noise_data.height );
        gen.SetNoiseData( noise_data );
        BuildNoiseTexture( gen );
      }

      void CSunGraphic::InitOut( Networking::ByteStream& bs ) const
      {
        bs << min_radius << max_radius << noise_data;
      }





    } // Components namespace
  } // Graphics namespace
} // SF namespace
