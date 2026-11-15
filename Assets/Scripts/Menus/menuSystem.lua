-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--menuSystem.lua

Menu = ClassSimple( function( self )
                      self.root = nil
                      self.looseElements = {}
                      self.tooltip = nil
                      self.HUD = nil
                      self.fadeTime = 0
                      self.stack = {}
                    end )
                    
function Menu:Push(menu)
  table.insert( self.stack, menu )
end

function Menu:Pop()
  table.remove( self.stack )
end

function Menu:Top()
  return self.stack[ #self.stack ]
end

function InitMenus()
  menu = Menu()
  
  menu.root = CreateElement("MENUREGION")
  menu.root:SetAlignment("CENTER", "MIDDLE")
  menu.root:SetLayout("TopToBottom")

  menu.HUD = HUDMenu()
  
  menu.fadeTime = 300
end

function MenuState( newMenu )
  menuState = GameState()
  
  menuState.OnPush = function() menu:Push( newMenu ) end
  menuState.OnPop = function() menu:Pop() end
  
  menuState.OnEnter = function() newMenu:Create() end
  menuState.OnLeave = function() newMenu:Delete() end
  
  table.insert( menuState.Updates, UpdateMenus )
  table.insert( menuState.Updates, UpdateMenuControls )
 
  if newMenu.Update then
    table.insert( menuState.Updates, function() newMenu:Update() end )
  end
  
  return menuState
end

function UpdateMenus()
  -- Update root
  menu.root:Update()
  
  -- Update "loose elements"
  for i = 1, #menu.looseElements do
    menu.looseElements[i]:Update()
  end
  
  -- Remove deleted loose elements
  local temp = {}
  
  for i = 1, #menu.looseElements do
    if menu.looseElements[i].entity then
      table.insert( temp, menu.looseElements[i] )
    end
  end
  
  menu.looseElements = temp
end
