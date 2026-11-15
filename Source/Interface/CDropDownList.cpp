/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CDropDownList.cpp
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 03/06/2009 @ 13:50:40 - pmera
**  \brief   
*******************************************************************************/

#include "CDropDownList.h"

namespace SF {
namespace UI
{

CDropDownList::CDropDownList()
{
  m_expanded = false;
}

CDropDownList::~CDropDownList()
{
}

Engine::BaseComponent *CDropDownList::Clone() const
{
  CDropDownList *dropDownList = new CDropDownList( *this );

  return dropDownList;
}

//void CDropDownList::PushToLua( lua_State *ls )
//{
//
//}

void CDropDownList::OnLeftClick()
{
  if(!m_expanded)
  {
    Expand();
  }
}

//void OnMouseOver();

void CDropDownList::OnMouseOut()
{
  Contract();
}

Engine::BaseComponent *CDropDownList::Creator(const ticpp::Element *element)
{
  CDropDownList *dropDownList = new CDropDownList;

  return dropDownList;
}

void CDropDownList::Expand()
{
  Focus();

  // Disable formatting
  // Resize

  m_originalHalfWidth = halfWidth;
  m_originalHalfHeight = halfHeight;

  halfHeight = halfHeight + halfHeight * m_items.size();

  // Show list items
}

void CDropDownList::Contract()
{
  Unfocus();
}

}

}
