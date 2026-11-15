# Assimp

SolarFlare used the Open Asset Import Library (Assimp) to load 3D models.

The recovered deposit contained the following files under `Source/External/assimp/`:

```text
include/
  aiAnim.h
  aiAssert.h
  aiCamera.h
  aiConfig.h
  aiDefines.h
  aiFileIO.h
  aiLight.h
  aiMaterial.h
  aiMaterial.inl
  aiMatrix3x3.h
  aiMatrix3x3.inl
  aiMatrix4x4.h
  aiMatrix4x4.inl
  aiMesh.h
  aiPostProcess.h
  aiQuaternion.h
  aiScene.h
  aiTexture.h
  aiTypes.h
  aiVector2D.h
  aiVector3D.h
  aiVector3D.inl
  assimp.h
  assimp.hpp
  DefaultLogger.h
  IOStream.h
  IOSystem.h
  Logger.h
  LogStream.h
  NullLogger.h
  Compiler/
    poppack1.h
    pstdint.h
    pushpack1.h
    MSVC/
      stdint.h
lib/
  vs2005/
    assimp.lib
    assimpd.lib
```

It also contained runtime DLLs under `Source/SolarFlare/`:

```text
Assimp32.dll
Assimp32d.dll
```

Both DLLs report file version 1.0.0.0; the precise Assimp release is unknown.

These third-party files were removed from the working copy. The recovered header
included redistribution terms. The historical build is consequently incomplete.

Upstream: [Assimp](https://www.assimp.org/). A current release is not necessarily
compatible with the original project.
