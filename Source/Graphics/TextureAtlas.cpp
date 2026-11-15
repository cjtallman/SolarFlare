/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TextureAtlas.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:46:19 CTallman
**  \brief   
*******************************************************************************/

#include "TextureAtlas.h"

#include "External/ticpp/include/ticpp.h"
#include "SolarFlare/Globals.h"
#include "Engine/AssetManager.h"
#include "Utilities/LeakCheck.h"
namespace SF
{
	namespace Graphics
	{
		namespace TextureType
		{

			TextureAtlas2D::TextureAtlas2D(void)
			{
			}

			TextureAtlas2D::~TextureAtlas2D(void)
			{
			}

			TextureAtlasInfo TextureAtlas2D::TextureInfo( unsigned int index )
			{
				if( index < texture_infos.size() )
					return texture_infos[index];
				else
					return TextureAtlasInfo();
			}

			bool TextureAtlas2D::LoadFromFile(const std::string &filepath)
			{
				bool succeed = true;

				try
				{
					ticpp::Document doc(filepath);
					doc.LoadFile();

					ticpp::Element * root = doc.FirstChildElement();

					if( root )
					{
						std::string texture_path = root->GetAttribute("texture");
						std::string asset_dir = Globals::assetManager->GetDirectory();
						texture.LoadFromFile(asset_dir + "Textures/" + texture_path);

						ticpp::Iterator< ticpp::Element > child("subimage");
						for( child = child.begin(root); child != child.end(); child++ )
						{
							TextureAtlasInfo info;
							std::string texname;

							child->GetAttributeOrDefault("name", &texname, "default");
							child->GetAttributeOrDefault("u_offset", &info.u_offset, 0.0f );
							child->GetAttributeOrDefault("v_offset", &info.v_offset, 0.0f );
							child->GetAttributeOrDefault("width", &info.width, 1.0f );
							child->GetAttributeOrDefault("height", &info.height, 1.0f );

							texture_infos.push_back( TextureAtlasInfo(info) );
						}
					}
					else
						succeed = false;
				}
				catch( ticpp::Exception & )
				{
					succeed = false;
				}

				return succeed;
			}


		}
	}
}