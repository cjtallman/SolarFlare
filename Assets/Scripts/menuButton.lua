-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--menuButton.lua

function HandleMessage(type, stack)
  local entity = EntityFactory:GetEntity( stack[1] )
  local graphic = entity:GetFirstComponent("ui_graphic")
  local button = entity:GetFirstComponent("ui_element")
  local sound = entity:GetFirstComponent("sound")
  
  if type == "onMouseOver" then
    graphic:SetState("OVER")
  elseif type == "onMouseOut" then
    graphic:SetState("UP")
  end
end

function Main()
  while 1 do
    local entity = EntityFactory:GetEntity( ScriptEngine:CurUpdateID() )
    local graphic = entity:GetFirstComponent("ui_graphic")
    local button = entity:GetFirstComponent("ui_element")
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
end
