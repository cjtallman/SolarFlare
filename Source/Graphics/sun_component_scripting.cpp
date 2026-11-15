/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    sun_component_scripting.cpp
**  \author  Chris Tallman
**  \par     email: ctallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 04/16/2009 @ 14:10:34 - nwilliam
*******************************************************************************/

#include "CSunGraphic.h"
#include "Utilities/Noise/AllNoise.h"

namespace SF
{
  namespace Graphics
  {
    namespace Components
    {

      // Scripting /////////////////////////////////////////////////////////////////
      void CSunGraphic::PushToLua( lua_State *ls )
      {
        Lunar<CSunGraphic>::push( ls, this );
      }

      LUA_START_DEFINE_CLASS( CSunGraphic )
        LUA_NAME_METHOD( CSunGraphic, "GetMinRadius",    LGetMinRadius )
        LUA_NAME_METHOD( CSunGraphic, "SetMinRadius",    LSetMinRadius )
        LUA_NAME_METHOD( CSunGraphic, "GetMaxRadius",    LGetMaxRadius )
        LUA_NAME_METHOD( CSunGraphic, "SetMaxRadius",    LSetMaxRadius )
        LUA_NAME_METHOD( CSunGraphic, "SetNoise",        LSetNoise )
        LUA_NAME_METHOD( CSunGraphic, "StopAnimation",   LStopAnimation )
        LUA_NAME_METHOD( CSunGraphic, "StartAnimation",  LStartAnimation )
        LUA_END_DEFINE_CLASS

        LUA_DEFINE_METHOD( CSunGraphic, LGetMinRadius )
      {
        lua_pushnumber( ls, GetMinRadius() );
        return 1;
      }

      LUA_DEFINE_METHOD( CSunGraphic, LSetMinRadius )
      {
        float rad = luaL_checkfloat(ls, 1);
        SetMinRadius(rad);
        return 0;
      }

      LUA_DEFINE_METHOD( CSunGraphic, LGetMaxRadius )
      {
        lua_pushnumber( ls, GetMaxRadius() );
        return 1;
      }

      LUA_DEFINE_METHOD( CSunGraphic, LSetMaxRadius )
      {
        float rad = luaL_checkfloat(ls, 1);
        SetMaxRadius(rad);
        return 0;
      }

      LUA_DEFINE_METHOD( CSunGraphic, LSetNoise )
      {
        using namespace Utilities;
        using namespace Noise;

        const char* nStrings[] = { "FBM", "HMF", "Perlin", "RMF", "Simplex", 0 };
        GeneratorType nGens[] = { GEN_FBM, GEN_HMF, GEN_PERLIN, GEN_RMF, GEN_SIMPLEX };

        const char* options[] = { "seed", "octaves", "gain", "offset", "frequency", 
                                  "lacunarity", "hurstIndex", "persistence", "time",
                                  "width", "height", "scaleMin", "scaleMax", 0 };

        // Require first parameter to be noise type as string
        int tOpt = luaL_checkoption( ls, 1, 0, nStrings );
        noise_data.SetDefaults( nGens[tOpt] );

        if( lua_gettop( ls ) > 1 )
        {
          // Require second parameter (if there is one) to be table
          luaL_checktype( ls, 2, LUA_TTABLE );

          // lua_next: pushes key then value onto stack
          lua_pushnil( ls );
          while( lua_next( ls, 2 ) != 0 )
          {
            // Require string keys
            luaL_checktype( ls, -2, LUA_TSTRING );

            switch( luaL_checkoption( ls, -2, 0, options ) )
            {
            case 0:  noise_data.seed         = luaL_checkint( ls, -1 );    break;
            case 1:  noise_data.octaves      = luaL_checkint( ls, -1 );    break;
            case 2:  noise_data.gain         = luaL_checkfloat( ls, -1 );  break;
            case 3:  noise_data.offset       = luaL_checkfloat( ls, -1 );  break;
            case 4:  noise_data.frequency    = luaL_checkfloat( ls, -1 );  break;
            case 5:  noise_data.lacunarity   = luaL_checkfloat( ls, -1 );  break;
            case 6:  noise_data.hurstIndex   = luaL_checkfloat( ls, -1 );  break;
            case 7:  noise_data.persistence  = luaL_checkfloat( ls, -1 );  break;
            case 8:  noise_data.time         = luaL_checkfloat( ls, -1 );  break;

            case 9:  noise_data.width        = luaL_checkint( ls, -1 );    break;
            case 10: noise_data.height       = luaL_checkint( ls, -1 );    break;

            case 11: noise_data.scaleMin     = luaL_checkfloat( ls, -1 );  break;
            case 12: noise_data.scaleMax     = luaL_checkfloat( ls, -1 );  break;
            }

            lua_pop( ls, 1 );
          }
        }

        noise_data.ComputeWeights();
        Utilities::Noise::SphereGenerator gen( noise_data.width, noise_data.height );
        gen.SetNoiseData( noise_data );

        BuildNoiseTexture( gen );

        return 0;
      }

      LUA_DEFINE_METHOD( CSunGraphic, LStartAnimation )
      {
        noise_data.dt = luaL_checkfloat( ls, 1 );
        noise_data.animate = true;

        Utilities::Noise::SphereGenerator gen( noise_data.width, noise_data.height );
        gen.SetNoiseData( noise_data );

        BuildNoiseTexture( gen );
        return 0;
      }

      LUA_DEFINE_METHOD_NOLS( CSunGraphic, LStopAnimation )
      {
        // The next OnFinish callback will not add another frame creation task now
        noise_data.animate = false;
        return 0;
      }

    }//Components
  }//Graphics
}//SF