/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CgShader.h
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:44:29 CTallman
**  \brief   
*******************************************************************************/

#ifndef _SF_CG_SHADER_H_
#define _SF_CG_SHADER_H_


#include "External/Cg/include/cg.h"
#include "External/Cg/include/cgGL.h"
#include "Utilities/SharedPtr.h"
#include "Utilities/HashTable.h"

namespace SF
{
  namespace Graphics
  {
    struct ShaderParameter
    {
      ShaderParameter():param(0), name(""){}
      CGparameter param;
      std::string name;
    };

    typedef Utilities::SharedPtr<ShaderParameter> SharedShaderParameter;
    typedef Utilities::HashTable<SharedShaderParameter> ParamTable;

    class CgShader
    {
    public:

      CgShader();

      //************************************
      // Method:    CgShader
      // FullName:  SF::Graphics::Shaders::CgShader::CgShader
      // Access:    public 
      // Returns:   
      // Qualifier:
      // Parameter: CGprogram _program
      // Parameter: CGprofile _profile
      //************************************
      CgShader( CGprogram _program, CGprofile _profile );

      //************************************
      // Method:    ~CgShader
      // FullName:  SF::Graphics::Shaders::CgShader::~CgShader
      // Access:    public 
      // Returns:   
      // Qualifier:
      //************************************
      ~CgShader();

      //************************************
      // Method:    GetNamedParameter
      // FullName:  SF::Graphics::Shaders::CgShader::GetNamedParameter
      // Access:    public 
      // Returns:   CGparameter
      // Qualifier:
      // Parameter: const char * name
      //************************************
      CGparameter GetNamedParameter( const char * name );

      SharedShaderParameter GetParameter( const std::string & name );

      //************************************
      // Method:    Bind
      // FullName:  SF::Graphics::Shaders::CgShader::Bind
      // Access:    public 
      // Returns:   void
      // Qualifier:
      //************************************
      void Bind();

      //************************************
      // Method:    Unbind
      // FullName:  SF::Graphics::Shaders::CgShader::Unbind
      // Access:    public 
      // Returns:   void
      // Qualifier:
      //************************************
      void Unbind();

      void ReloadParameters();

      CgShader & operator=(const CgShader & A);

    protected:
      CGprogram program;
      CGprofile profile;


      
      ParamTable parameters;

    };


    struct CGShaderWrapper
    {
      CGShaderWrapper() : shader(0), version(0){}
      Utilities::SharedPtr<CgShader> shader;
      int version;  //reloading increases this.
    };

    typedef Utilities::SharedPtr<CgShader> SharedShader2;

    typedef Utilities::SharedPtr<CGShaderWrapper> SharedShader;

  }
}

#endif
