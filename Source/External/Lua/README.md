# Lua and Lunar

SolarFlare used Lua for scripting.

The recovered deposit contained the following files under `Source/External/Lua/`:

```text
include/
  lauxlib.h
  lua.h
  lua.hpp
  luaconf.h
  lualib.h
  lunar.h
  LunarHelper.h
lib/
  vs2005/
    lua5.1.4.lib
    lua5.1.4d.lib
```

The recovered `lua.h` identifies Lua 5.1.4 and contains its copyright and
permission notice. The Lunar files are discussed separately below.

The third-party Lua headers and libraries, including `include/lauxlib.h`, were
removed from the working copy. The historical build is consequently incomplete.

The following recovered files remain in the working copy:

- `include/LunarHelper.h` carries a DigiPen notice and is project-specific.
- `include/lunar.h` also carries a DigiPen notice, but describes itself as based
  on the [Lunar binding example](http://lua-users.org/wiki/CppBindingWithLunar).
  The upstream example's license is unknown.

## SolarFlare modification to `lauxlib.h`

The recovered header marked the following project addition. This zero-context
diff is reconstructed from that block, not from a complete upstream comparison.

```diff
--- a/include/lauxlib.h
+++ b/include/lauxlib.h
@@ -108,0 +109,3 @@
+// SF addition! 09/12/2008 @ 13:12:32 - nwilliam
+#define luaL_checkfloat(L,n)  ((float)luaL_checknumber(L, (n)))
+// END addition
```

Upstream: [Lua](https://www.lua.org/) and [Lua 5.1](https://www.lua.org/versions.html#5.1).
A current release is not necessarily compatible with the original project.
