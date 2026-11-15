/***** All content © 2009 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    XML.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 11/10/2008 @ 21:01:23 - pmera
**  \brief   
*******************************************************************************/

#include "External/ticpp/include/ticpp.h"
#include "Color.h"

#ifndef SF_XMLUTILS_H_
#define SF_XMLUTILS_H_

namespace SF {
namespace Utilities {
namespace XML {

void ReadColor(const ticpp::Element *element, const std::string& childName, Color *color, const Color& def);

template <typename T>
void ReadValue(const ticpp::Element *element, const std::string& childName, T *value, T def)
{
  ticpp::Element *child = element->FirstChildElement(childName, false);
  if( child )
    child->GetAttributeOrDefault("value", value, def);
  else
    *value = def;
}

}
}
}

#endif
