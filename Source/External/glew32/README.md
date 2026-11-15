# GLEW

SolarFlare used the OpenGL Extension Wrangler Library (GLEW).

The recovered deposit contained the following files under `Source/External/glew32/`:

```text
include/
  glew.h
  glxew.h
  wglew.h
lib/
  vs2005/
    glew32s1.5.1.lib
```

The library filename identifies version 1.5.1.

The third-party library and headers, including `include/glew.h`, were removed
from the working copy. The historical build is consequently incomplete.

## SolarFlare modification to `glew.h`

The recovered header marked the following project addition. This zero-context
diff is reconstructed from that block, not from a complete upstream comparison.

```diff
--- a/include/glew.h
+++ b/include/glew.h
@@ -82,0 +83,2 @@
+// Modification, 02/16/2009 @ 20:20:38 - nwilliam
+#define GLEW_STATIC 1
```

Upstream: [GLEW](https://glew.sourceforge.net/). A current release is not
necessarily compatible with the original project.
