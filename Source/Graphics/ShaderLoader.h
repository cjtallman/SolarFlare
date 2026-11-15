/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    ShaderLoader.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:45:48 CTallman
**  \brief
*******************************************************************************/

#ifndef _SF_SHADER_LOADER_H_
#define _SF_SHADER_LOADER_H_

#include "CgShader.h"
#include "Utilities/HashTable.h"

namespace SF
{
  namespace Graphics
  {
    namespace ShaderType
    {
      enum
      {
        VERTEX_SHADER,
        FRAGMENT_SHADER
      };
    }

	class ShaderLoader
    {
    public:

      //************************************
      // Method:    ShaderLoader
      // FullName:  SF::Graphics::ShaderLoader::ShaderLoader
      // Access:    public
      // Returns:
      // Qualifier:
      //************************************
      ShaderLoader();

      //************************************
      // Method:    ~ShaderLoader
      // FullName:  SF::Graphics::ShaderLoader::~ShaderLoader
      // Access:    public
      // Returns:
      // Qualifier:
      //************************************
      virtual ~ShaderLoader();
      
      
      CgShader * LoadProgramFromSource( int shader_type, const char * source, const char * entry = 0 );
      CgShader CompileSource( int shader_type, const char * source );

      SharedShader2 GetFragmentShader( const std::string & mappedName );
      SharedShader2 GetVertexShader( const std::string & mappedName );
      
      CGcontext Context() { return cg_context; }

    private:
      CGcontext cg_context;

      typedef Utilities::HashTable<SharedShader2> ShaderTable;
      ShaderTable vertexShaders;
      ShaderTable fragmentShaders;
    };
  }
}

#endif
