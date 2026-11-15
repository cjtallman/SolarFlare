/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    planet_component_base.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:31:06 CTallman
**  \brief   Contains all of the necessary functions for BaseComponent.
*******************************************************************************/

#include "Engine/Entity.h"
#include "CPlanetGraphic.h"
#include "SolarFlare/Globals.h"
#include "Engine/AssetManager.h"
#include "Utilities/Random/Random.h"
#include "External/ticpp/include/ticpp.h"

#include "Utilities/LeakCheck.h"

namespace SF
{

  namespace Graphics
  {
    namespace Components
    {


      Engine::BaseComponent * CPlanetGraphic::Clone() const
      {
        CPlanetGraphic * new_planet = new CPlanetGraphic();
        new_planet->SetMinRadius( min_radius );
        new_planet->SetMaxRadius( max_radius );

        new_planet->transform = 0;
        new_planet->color_texture.RandomizeColors();

        new_planet->SetVertexShader(vert);
        new_planet->SetFragmentShader(frag);
        new_planet->SetModel( model );
        new_planet->crater_texture = crater_texture;
        //new_planet->deform_texture = deform_texture;
        //new_planet->height_texture = height_texture;
        return new_planet;
      }  



      bool CPlanetGraphic::InitDependencies( Engine::Entity * parent )
      {
        bool succeed = true;
        if( parent && parent->Has( Engine::ComponentType::TRANSFORM ) )
        {
          transform = static_cast<Math::CTransform*>(parent->GetFirstComponent( Engine::ComponentType::TRANSFORM ));
        }
        else
          succeed &= false;

        return succeed;
      }


      Engine::BaseComponent * CreatePlanetGraphic( const ticpp::Element * element )
      {
        if( element )
        {
          CPlanetGraphic * out = new CPlanetGraphic();
          static SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;

          try
          {
            ticpp::Element * child = 0;

            //out->BuildNoiseTexture();

            ////////////////////////////////////////////////////////////////////
            // Radius Portion
            ////////////////////////////////////////////////////////////////////
            child = element->FirstChildElement( "radius" );
            if( child )
            {
              float planet_radius;
              child->GetAttributeOrDefault("radius", &planet_radius, 100.0f );
              out->SetMinRadius( planet_radius );
            }

            for(child = element->FirstChildElement("color", false); child;
              child = child->NextSiblingElement("color", false))
            {
              float position;
              int r;
              int g;
              int b;

              child->GetAttribute("val", &position);
              child->GetAttribute("r", &r);
              child->GetAttribute("g", &g);
              child->GetAttribute("b", &b);

              out->AddColor(position, SF::Color((SF::Color::Type)r,(SF::Color::Type)g,(SF::Color::Type)b) );
            }

            child = element->FirstChildElement( "model", false );
            if( child )
            {
              std::string model_name = child->GetAttribute("name");
              if( !model_name.empty() )
              {

                SF::Engine::AssetManager * asset_manager = SF::Globals::assetManager;
                SF::Engine::Assets::ModelAsset * asset = (SF::Engine::Assets::ModelAsset*)asset_manager->GetAsset( model_name );
                if( asset )
                  out->SetModel( SharedModel(asset->model) );
              }
            }

            child = element->FirstChildElement( "shader", false );
            if( child )
            {
              std::string shader_name = child->GetAttribute("vertex");
              if( !shader_name.empty() )
              {
                out->SetVertexShader( shader_name );
              }

              shader_name = child->GetAttribute("fragment");
              if( !shader_name.empty() )
              {
                out->SetFragmentShader( shader_name );
              }
            }


            child = element->FirstChildElement( "texture", false );
            if( child )
            {
              std::string crater_texture = child->GetAttribute("crater");
              if( !crater_texture.empty() )
              {
                Engine::AssetManager * asset_manager = Globals::assetManager;
                Engine::Assets::Texture * texture = (Engine::Assets::Texture*)asset_manager->GetAsset( crater_texture );
                out->crater_texture = texture->image;
              }
            }

            return out;
          }

          catch( ticpp::Exception & )
          {
            delete out;
            return 0;
          }
        }

        return 0;
      }



    }
  }
}