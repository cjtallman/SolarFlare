/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CgShader.cpp
**  \author  Chris Tallman
**  \par     email: CTallman\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 11/19/2008 @ 18:44:18 CTallman
**  \brief   
*******************************************************************************/

#include "CgShader.h"

namespace SF
{
  namespace Graphics
  {
      CgShader::CgShader()
        : program(0), profile(CG_PROFILE_UNKNOWN)
      {
      }


      //************************************
      // Method:    CgShader
      // FullName:  SF::Graphics::Shaders::CgShader::CgShader
      // Access:    public 
      // Returns:   
      // Qualifier: : program(_program), profile(_profile)
      // Parameter: CGprogram _program
      // Parameter: CGprofile _profile
      //************************************
      CgShader::CgShader( CGprogram _program, CGprofile _profile ) 
        : program(_program), profile(_profile)
      {
      }

      //************************************
      // Method:    ~CgShader
      // FullName:  SF::Graphics::Shaders::CgShader::~CgShader
      // Access:    public 
      // Returns:   
      // Qualifier:
      //************************************
      CgShader::~CgShader()
      {
        //if( cgIsProgram(program) )
        //  cgDestroyProgram(program);
      }

      //************************************
      // Method:    GetNamedParameter
      // FullName:  SF::Graphics::Shaders::CgShader::GetNamedParameter
      // Access:    public 
      // Returns:   CGparameter
      // Qualifier:
      // Parameter: const char * name
      //************************************
      CGparameter CgShader::GetNamedParameter( const char * name )
      {
        return cgGetNamedParameter(program, name);
      }

      SharedShaderParameter CgShader::GetParameter( const std::string & name )
      {
        ParamTable::iterator itr = parameters.find(name);
        if( itr == parameters.end() )
        {
          SharedShaderParameter param(new ShaderParameter());
          param->name = name;
          param->param = cgGetNamedParameter(program, name.c_str());
          itr = parameters.insert(name,param);
        }

        return *itr;
      }

      //************************************
      // Method:    Bind
      // FullName:  SF::Graphics::Shaders::CgShader::Bind
      // Access:    public 
      // Returns:   void
      // Qualifier:
      //************************************
      void CgShader::Bind()
      {
        cgGLEnableProfile(profile);					
        cgGLBindProgram(program);
      }

      //************************************
      // Method:    Unbind
      // FullName:  SF::Graphics::Shaders::CgShader::Unbind
      // Access:    public 
      // Returns:   void
      // Qualifier:
      //************************************
      void CgShader::Unbind()
      {
        cgGLDisableProfile(profile);
      }

      void CgShader::ReloadParameters()
      {
        ParamTable::iterator itr = parameters.begin();
        for( ; itr != parameters.end(); ++itr )
        {
          (*itr)->param = cgGetNamedParameter(program,(*itr)->name.c_str());
        }
      }

      CgShader & CgShader::operator=(const CgShader & A)
      {
        program = A.program;
        profile = A.profile;
        ReloadParameters();
        return *this;
      }

  }
}