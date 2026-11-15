/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    Assets.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 03/03/2009 @ 21:09:19 - nwilliam
**  \brief   Loaders/wrappers for all types of assets.
*******************************************************************************/

#ifndef SF_ASSETS_H
#define SF_ASSETS_H

#include <string>
#include "Graphics/BaseModel.h"
#include "Graphics/Texture2D.h"
#include "Utilities/Delegates.h"
#include "Utilities/SharedPtr.h"
#include "Script/ScriptHelper.h"
#include "Utilities/SharedArray.h"
#include "Graphics/TextureAtlas.h"
#include "Graphics/CgShader.h"

namespace SF {
namespace Engine {
namespace Assets
{
  enum Types { T_MODEL, T_SCRIPT, T_SOUND, T_TEXTURE, T_TEXTURE_ATLAS, T_SHADER, T_LAST };

  enum Loaders { L_BASE, L_MODEL, L_IMAGE, L_IMAGE_ATLAS, L_NOISEIMAGE, L_RAW_FILE, 
                 L_SOUND, L_SCRIPT, L_FRAGMENT_SHADER, L_VERTEX_SHADER, L_LAST };


  typedef Utilities::Delegate0<void> OnFinishCallback;


  //////////////////////////////////////////////////////////////////////////////
  // BaseAsset
  //////////////////////////////////////////////////////////////////////////////
  struct BaseAsset
  {
    BaseAsset();
    virtual ~BaseAsset();
    virtual bool Load() = 0;
    virtual void Swap();
    virtual BaseAsset* New() const = 0;
    

    Types type;
    std::string name;
    std::string filePath;
    static const Loaders loader = L_BASE;
  };


  //////////////////////////////////////////////////////////////////////////////
  // VertextShader
  //////////////////////////////////////////////////////////////////////////////
  struct VertexShader : public BaseAsset
  {
    VertexShader();
    bool Load();
    void Swap();
    BaseAsset * New() const;

    static const Loaders loader = L_VERTEX_SHADER;

  private:
    int size;
    Utilities::SharedArray<char> data;
  };


  //////////////////////////////////////////////////////////////////////////////
  // FragmentShader
  //////////////////////////////////////////////////////////////////////////////
  struct FragmentShader : public BaseAsset
  {
    FragmentShader();
    bool Load();
    void Swap();
    
    BaseAsset * New() const;
    

    static const Loaders loader = L_FRAGMENT_SHADER;

  private:
    int size;
    Utilities::SharedArray<char> data;
  };


  //////////////////////////////////////////////////////////////////////////////
  // Model
  //////////////////////////////////////////////////////////////////////////////
  struct ModelAsset : public BaseAsset
  {
    ModelAsset();
    virtual ~ModelAsset();
    virtual bool Load();
    virtual void Swap();
    virtual BaseAsset* New() const;

    Graphics::SharedModel temp;
    Graphics::SharedModel model;
    static const Loaders loader = L_MODEL;
  };


  //////////////////////////////////////////////////////////////////////////////
  // Image
  //////////////////////////////////////////////////////////////////////////////
  struct Texture : public BaseAsset
  {
    Texture();
    virtual ~Texture();
    virtual bool Load();
    virtual void Swap();
    virtual BaseAsset* New() const;

    Graphics::SharedTexture image;
    static const Loaders loader = L_IMAGE;

    private:
      Graphics::TextureType::Texture2D temp;
  };


   //////////////////////////////////////////////////////////////////////////////
  // TextureAtlas
  //////////////////////////////////////////////////////////////////////////////
  struct TextureAtlas : public BaseAsset
  {
    TextureAtlas();
    virtual ~TextureAtlas();
    virtual bool Load();
    virtual void Swap();
    virtual BaseAsset* New() const;

    static const Loaders loader = L_IMAGE_ATLAS;
    Utilities::SharedPtr<Graphics::TextureType::TextureAtlas2D> image;

    private:
      Graphics::TextureType::TextureAtlas2D temp;
  };


  //////////////////////////////////////////////////////////////////////////////
  // RawFile
  //////////////////////////////////////////////////////////////////////////////
  struct RawFile : public BaseAsset
  {
    virtual ~RawFile();
    virtual bool Load();
    virtual BaseAsset* New() const;

    int size;
    Utilities::SharedArray<char> data;
    static const Loaders loader = L_RAW_FILE;
  };


  //////////////////////////////////////////////////////////////////////////////
  // Script
  //////////////////////////////////////////////////////////////////////////////
  struct ScriptAsset : public BaseAsset
  {
    ScriptAsset();
    virtual ~ScriptAsset();
    virtual bool Load();
    virtual void Swap();
    virtual BaseAsset* New() const;

    int loadStatus;
    Script::LuaFunction mainFunc;
    Script::LuaFunction msgHandler;
    static const Loaders loader = L_SCRIPT;
  };


  //////////////////////////////////////////////////////////////////////////////
  // Sound
  //////////////////////////////////////////////////////////////////////////////
  struct Sound : public BaseAsset
  {
    virtual ~Sound();
    virtual bool Load();
    virtual BaseAsset* New() const; 

    static const Loaders loader = L_SOUND;
  };

} // Asset namespace
} // Engine namespace
} // SF namespace

#endif // SF_ASSETS_H
