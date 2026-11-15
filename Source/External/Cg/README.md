# NVIDIA Cg

SolarFlare used NVIDIA's Cg toolkit for shaders.

The recovered deposit contained the following files under `Source/External/Cg/`:

```text
include/
  cg.h
  cgGL.h
  cgGL_profiles.h
  cg_bindlocations.h
  cg_datatypes.h
  cg_enums.h
  cg_errors.h
  cg_profiles.h
lib/
  vs2005/
    cg.lib
    cgGL.lib
```

It also contained runtime DLLs under `Source/SolarFlare/`:

```text
cg.dll
cgGL.dll
```

The DLLs report version 2.1.0009.

These third-party files were removed from the working copy. The recovered
headers included a redistribution notice; the terms applicable to the runtime
and libraries are unknown. The historical build is consequently incomplete.

Upstream: [NVIDIA Cg Toolkit](https://developer.nvidia.com/cg-toolkit). Cg is
legacy software; current downloads may not match the recovered version.
