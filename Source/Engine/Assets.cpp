/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Assets.cpp
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/03/2009 @ 21:09:22 - nwilliam
**  \brief   Asset loader/wrapper implementations
*******************************************************************************/

#include <vector>
#include <fstream>
#include <cstring>  // memcpy
#include "Assets.h"
#include "SolarFlare/Globals.h"
#include "Script/ScriptEngine.h"
#include "Utilities/DevConsole.h"
#include "Graphics/ShaderLoader.h"

#include "Utilities/LeakCheck.h"

namespace SF {
namespace Engine {
namespace Assets
{
  //////////////////////////////////////////////////////////////////////////////
  // BaseAsset
  //////////////////////////////////////////////////////////////////////////////
  BaseAsset::BaseAsset() : type( T_LAST )
  {
  }

  BaseAsset::~BaseAsset()
  {
  }

  void BaseAsset::Swap()
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  // FragmentShader
  //////////////////////////////////////////////////////////////////////////////
  FragmentShader::FragmentShader()
  {
  }
  
  bool FragmentShader::Load()
  {
    std::fstream file( filePath.c_str() , std::ios::in | std::ios::binary );

    if( !file.is_open() )
      return false;

    file.seekg( 0 , std::ios::end );
    size = file.tellg();
    file.seekg( 0 , std::ios::beg );

    // +1 for null terminator
    data = new char[ size + 1 ];

    file.read( data.Get(), size );
    file.close();

    // Add null terminator to end
    data[size] = 0;

    return true;
  }

  BaseAsset * FragmentShader::New() const
  {
    return new FragmentShader(); 
  }

  void FragmentShader::Swap()
  {
    Graphics::SharedShader2 real_shader = Globals::shaderLoader->GetFragmentShader(name);
    *real_shader.Get() = Globals::shaderLoader->CompileSource(Graphics::ShaderType::FRAGMENT_SHADER, data.Get());
  }

  //////////////////////////////////////////////////////////////////////////////
  // VertexShader
  //////////////////////////////////////////////////////////////////////////////
  VertexShader::VertexShader()
  {
  }
  
  bool VertexShader::Load()
  {
    std::fstream file( filePath.c_str() , std::ios::in | std::ios::binary );

    if( !file.is_open() )
      return false;

    file.seekg( 0 , std::ios::end );
    size = file.tellg();
    file.seekg( 0 , std::ios::beg );

    // +1 for null terminator
    data = new char[ size + 1 ];

    file.read( data.Get(), size );
    file.close();

    // Add null terminator to end
    data[size] = 0;

    return true;
  }

  BaseAsset * VertexShader::New() const
  {
    return new VertexShader(); 
  }

  void VertexShader::Swap()
  {
    Graphics::SharedShader2 real_shader = Globals::shaderLoader->GetVertexShader(name);
    *real_shader.Get() = Globals::shaderLoader->CompileSource(Graphics::ShaderType::VERTEX_SHADER, data.Get());
  }



  //////////////////////////////////////////////////////////////////////////////
  // Model
  //////////////////////////////////////////////////////////////////////////////
  ModelAsset::ModelAsset()
  {
    model = new Graphics::Model::BaseModel();
    temp = new Graphics::Model::BaseModel();
  }

  ModelAsset::~ModelAsset()
  {
  }

  void ModelAsset::Swap()
  {
    *model.Get() = *temp.Get();
    temp.Reset( 0 );
  }

  BaseAsset* ModelAsset::New() const
  {
    return new ModelAsset();
  }

  bool ModelAsset::Load()
  {
    return temp->LoadFromFile( filePath );
  }


  //////////////////////////////////////////////////////////////////////////////
  // Texture
  //////////////////////////////////////////////////////////////////////////////
  Texture::Texture()
  {
    image = new Graphics::TextureType::Texture2D();
  }

  Texture::~Texture()
  {
  }

  void Texture::Swap()
  {
    *image.Get() = temp;
    image->Recreate();
    temp.image.Reset( 0 );
  }

  BaseAsset* Texture::New() const
  {
    return new Texture();
  }

  bool Texture::Load()
  {
    temp.LoadFromFile( filePath );
    return true;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Texture Atlas
  //////////////////////////////////////////////////////////////////////////////
  TextureAtlas::TextureAtlas()
  {
    image = new Graphics::TextureType::TextureAtlas2D();
  }

  TextureAtlas::~TextureAtlas()
  {
  }

  void TextureAtlas::Swap()
  {
    *image.Get() = temp;
    image->texture.Recreate();
  }

  BaseAsset* TextureAtlas::New() const
  {
    return new TextureAtlas();
  }

  bool TextureAtlas::Load()
  {
    temp.LoadFromFile(filePath);
    return true;
  }


  //////////////////////////////////////////////////////////////////////////////
  // RawFile
  //////////////////////////////////////////////////////////////////////////////
  RawFile::~RawFile()
  {
  }

  bool RawFile::Load()
  {
    std::fstream file( filePath.c_str() , std::ios::in | std::ios::binary );

    if( !file.is_open() )
      return false;

    file.seekg( 0 , std::ios::end );
    size = file.tellg();
    file.seekg( 0 , std::ios::beg );

    // +1 for null terminator
    data = new char[ size + 1 ];

    file.read( data.Get(), size );
    file.close();

    // Add null terminator to end
    data[size] = 0;

    return true;
  }

  BaseAsset* RawFile::New() const
  {
    return new RawFile;
  }


  //////////////////////////////////////////////////////////////////////////////
  // ScriptAsset
  //////////////////////////////////////////////////////////////////////////////
  ScriptAsset::ScriptAsset() : loadStatus( 0 )
  {
  }

  ScriptAsset::~ScriptAsset()
  {
  }

  void ScriptAsset::Swap()
  {
    lua_State* ls = Globals::scriptEngine->m_masterState;

    if( luaL_dofile( ls, filePath.c_str() ) )
    {
      loadStatus = -1;
      Globals::scriptEngine->HandleLuaError( ls, "Loading...", false );
      return;
    }

    loadStatus = 1;

    lua_getglobal( ls, "Main" );

    if( mainFunc.Set( ls ) )
    {
      lua_pushnil( ls );
      lua_setglobal( ls, "Main" );
    }
    else
    {
      lua_pop( ls, 1 );
    }

    lua_getglobal( ls, "HandleMessage" );

    if( msgHandler.Set( ls ) )
    {
      lua_pushnil( ls );
      lua_setglobal( ls, "HandleMessage" );
    }
    else
    {
      lua_pop( ls, 1 );
    }
  }

  bool ScriptAsset::Load()
  {
    return true;
  }

  BaseAsset* ScriptAsset::New() const
  {
    return new ScriptAsset;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Sound
  //////////////////////////////////////////////////////////////////////////////
  Sound::~Sound()
  {
  }

  BaseAsset* Sound::New() const
  {
    return new Sound();
  }

  bool Sound::Load()
  {
    return true;
  }

} // Assets namespace
} // Engine namespace
} // SF namespace
