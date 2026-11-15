/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ShaderLoader.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:44:53 CTallman
**  \brief
*******************************************************************************/

#include "OpenGLInclude.h"
#include "ShaderLoader.h"

#include "SolarFlare/Globals.h"
#include "Engine/AssetManager.h"
#include "Utilities/DevConsole.h"



namespace SF
{
  namespace Graphics
  {
    //************************************
    // Method:    ShaderLoader
    // FullName:  SF::Graphics::ShaderLoader::ShaderLoader
    // Access:    public
    // Returns:
    // Qualifier:
    //************************************
    ShaderLoader::ShaderLoader()
    {
      cg_context = cgCreateContext();
      if( cg_context == 0 )
        Globals::console->Error("ShaderLoader::ShaderLoader() --- Failed to create Cg context.");
    }

    //************************************
    // Method:    ~ShaderLoader
    // FullName:  SF::Graphics::ShaderLoader::~ShaderLoader
    // Access:    public
    // Returns:
    // Qualifier:
    //************************************
    ShaderLoader::~ShaderLoader()
    {
      if( cg_context )
        cgDestroyContext( cg_context );
    }

    //************************************
    // Method:    LoadProgramFromSource
    // FullName:  SF::Graphics::ShaderLoader::LoadProgramFromSource
    // Access:    public
    // Returns:   Shaders::CgShader *
    // Qualifier:
    // Parameter: int shader_type
    // Parameter: const char * source
    // Parameter: const char * entry
    //************************************
    CgShader * ShaderLoader::LoadProgramFromSource( int shader_type, const char * source, const char * entry )
    {
      if( !cg_context )
      {
        Globals::console->Error("ShaderLoader::LoadProgramFromSource() --- Cg context does not exist.");
        return 0;
      }

      CGprofile profile;

      switch(shader_type)
      {
      case ShaderType::VERTEX_SHADER:
        profile = cgGLGetLatestProfile(CG_GL_VERTEX);
        break;

      case ShaderType::FRAGMENT_SHADER:
        profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
        break;

      default:
        profile = CG_PROFILE_UNKNOWN;
        Globals::console->Error("ShaderLoader::LoadProgramFromSource() --- Invalid shader type specified.");
        break;
      }

      if( profile == CG_PROFILE_UNKNOWN )
      {
        Globals::console->Error("ShaderLoader::LoadProgramFromSource() --- Unknown shader profile.");
        return 0;
      }

      cgGLSetOptimalOptions( profile );

      CGprogram program = cgCreateProgram( cg_context, CG_SOURCE, source, profile, entry, 0 );

      if( !program )
      {
        const char * listing = cgGetLastListing(cg_context);
        if( listing )
          Globals::console->Warning( std::string("ShaderLoader::LoadProgramFromSource() --- Compile Listing: ") + listing );

        CGerror error = cgGetError();
        const char * error_string = cgGetErrorString(error);

        if( error_string )
          Globals::console->Error( std::string("ShaderLoader::LoadProgramFromSource() --- Compile Error: ") + error_string );


        return 0;
      }

      cgGLLoadProgram(program);

      return new CgShader( program, profile );
    }

    CgShader ShaderLoader::CompileSource( int shader_type, const char * source )
    {
      if( !cg_context )
      {
        Globals::console->Error("ShaderLoader::LoadProgramFromSource() --- Cg context does not exist.");
        return CgShader();
      }

      CGprofile profile;

      switch(shader_type)
      {
      case ShaderType::VERTEX_SHADER:
        profile = cgGLGetLatestProfile(CG_GL_VERTEX);
        break;

      case ShaderType::FRAGMENT_SHADER:
        profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
        break;

      default:
        profile = CG_PROFILE_UNKNOWN;
        Globals::console->Error("ShaderLoader::LoadProgramFromSource() --- Invalid shader type specified.");
        break;
      }

      if( profile == CG_PROFILE_UNKNOWN )
      {
        Globals::console->Error("ShaderLoader::LoadProgramFromSource() --- Unknown shader profile.");
        return CgShader();
      }

      cgGLSetOptimalOptions( profile );

      CGprogram program = cgCreateProgram( cg_context, CG_SOURCE, source, profile, 0, 0 );

      if( !cgIsProgram(program) )
      {
        const char * listing = cgGetLastListing(cg_context);
        if( listing )
          Globals::console->Warning( std::string("ShaderLoader::LoadProgramFromSource() --- Compile Listing: ") + listing );

        CGerror error = cgGetError();
        const char * error_string = cgGetErrorString(error);

        if( error_string )
          Globals::console->Error( std::string("ShaderLoader::LoadProgramFromSource() --- Compile Error: ") + error_string );

        return CgShader();
      }

      cgGLLoadProgram(program);

      return CgShader( program, profile );
    }

    SharedShader2 ShaderLoader::GetFragmentShader( const std::string & mappedName )
    {
      if( !Globals::assetManager->GetAsset(mappedName) )
        Globals::console->Error() << "Shader does not exist: " << mappedName << std::endl;

      ShaderTable::iterator itr = fragmentShaders.find(mappedName);
      if( itr == fragmentShaders.end() )
      {
        itr = fragmentShaders.insert(mappedName,SharedShader2(new CgShader));
      }

      return *itr;
    }

    SharedShader2 ShaderLoader::GetVertexShader( const std::string & mappedName )
    {
      if( !Globals::assetManager->GetAsset(mappedName) )
        Globals::console->Error() << "Shader does not exist: " << mappedName << std::endl;

      ShaderTable::iterator itr = vertexShaders.find(mappedName);
      if( itr == vertexShaders.end() )
      {
        itr = vertexShaders.insert(mappedName,SharedShader2(new CgShader));
      }

      return *itr;
    }
  }
}
