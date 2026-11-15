/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    CDropDownList.h
**  \author  Patrick Mera
**  \par     email: pmera\@digipen.edu
**  \par     Course: GAM 300/350
**  \par     Updated: 03/06/2009 @ 13:42:20 - pmera
**  \brief   
*******************************************************************************/

#ifndef SF_DROPDOWNLIST_H_
#define SF_DROPDOWNLIST_H_

#include <string>
#include <vector>

#include "CBasicElement.h"

namespace SF {
namespace UI
{

class CDropDownList : public CBasicElement
{
public:
  CDropDownList();
  ~CDropDownList();

  virtual BaseComponent* Clone() const;
  virtual void PushToLua( lua_State *ls );

  virtual void OnLeftClick();
  virtual void OnMouseOver();
  virtual void OnMouseOut();

  static Engine::BaseComponent *Creator(const ticpp::Element *element);

private:
  void    Expand();
  void    Contract();

  typedef std::vector< std::string > ItemList;

  lua_State *       m_luaState;
  int               m_onSelect;
  
  ItemList          m_items;

  bool              m_expanded;

  float             m_originalHalfWidth;
  float             m_originalHalfHeight;
};

}

}

#endif
