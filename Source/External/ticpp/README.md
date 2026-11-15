# TinyXML++ and TinyXML

SolarFlare used TinyXML++ (ticpp), a C++ wrapper around TinyXML.

The recovered deposit contained the following files under `Source/External/ticpp/`:

```text
include/
  ticpp.h
  ticpprc.h
  tinystr.h
  tinyxml.h
lib/
  vs2005/
    ticpp-2.5.3.lib
    ticppd-2.5.3.lib
```

The recovered TinyXML header identifies version 2.5.3; the precise TinyXML++
release is unknown.

These third-party files were removed from the working copy. The recovered
headers included separate notices for ticpp and TinyXML. The historical build is
consequently incomplete.

References: [TinyXML++](https://github.com/wxFormBuilder/ticpp) and
[TinyXML](https://sourceforge.net/projects/tinyxml/). Later releases may not
match the original project.
