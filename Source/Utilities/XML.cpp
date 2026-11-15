/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    XML.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/10/2008 @ 21:02:30 - pmera
**  \brief   
*******************************************************************************/

#include "XML.h"

namespace SF {
namespace Utilities {
namespace XML {

void ReadColor(const ticpp::Element *element, const std::string& childName, Color *color, const Color& def)
{
  ticpp::Element *child = element->FirstChildElement(childName, false);

  if( child )
  {
    int r, g, b, a;

    child->GetAttributeOrDefault("r", &r, def.r);
    child->GetAttributeOrDefault("g", &g, def.g);
    child->GetAttributeOrDefault("b", &b, def.b);
    child->GetAttributeOrDefault("a", &a, def.a);

    *color = Color((unsigned char)r,(unsigned char)g,(unsigned char)b,(unsigned char)a);
  }
  else
    *color = def;
}

}
}
}
