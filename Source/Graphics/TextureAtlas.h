/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    TextureAtlas.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:46:36 CTallman
**  \brief   
*******************************************************************************/

#ifndef _SF_TEXTURE_ATLAS_H_
#define _SF_TEXTURE_ATLAS_H_

#include "Texture2D.h"
#include "Math/TVector2.h"
#include <vector>

namespace SF
{
	namespace Graphics
	{
		namespace TextureType
		{

			struct TextureAtlasInfo
			{
				TextureAtlasInfo()
				{
					u_offset = 0;
					v_offset = 0;
					width = .5f;
					height = .5f;
				}

				float u_offset;
				float v_offset;
				float width;
				float height;
			};


			class TextureAtlas2D
			{
			public:
				TextureAtlas2D(void);
				~TextureAtlas2D(void);

				bool LoadFromFile( const std::string & filepath );
				TextureAtlasInfo TextureInfo( unsigned int index );

				Texture2D texture;
				std::vector<TextureAtlasInfo> texture_infos;
			};

		}
	}
}
#endif
