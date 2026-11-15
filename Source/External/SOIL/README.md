# SOIL

SolarFlare used the Simple OpenGL Image Library (SOIL) for image loading.

The recovered deposit contained the following files under `Source/External/SOIL/`:

```text
include/
  SOIL.h
lib/
  vs2005/
    SOIL.lib
    SOILd.lib
```

Their exact release is unknown. The historical `Source/SConstruct`
also refers to a Linux library named `libSOIL2007.08.a`, which was not present
in the recovered deposit.

The recovered third-party files were removed from the working copy. The
historical build is consequently incomplete.

Reference: [SOIL source mirror](https://github.com/kbranigan/Simple-OpenGL-Image-Library).
Whether the mirror matches the recovered version is unknown.
