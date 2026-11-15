-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--basicElement.lua

--[[
function HandleMessage(type, stack)
  local entity = EntityFactory:GetEntity( stack[1] )
  local graphic = entity:GetFirstComponent("ui_graphic")
  local button = entity:GetFirstComponent("ui_element")
end

function Main()
  local entity = EntityFactory:GetEntity( ScriptEngine:CurUpdateID() )
  local graphic = entity:GetFirstComponent("ui_graphic")
  local text = entity:GetFirstComponent("text")
  local button = entity:GetFirstComponent("ui_element")
  
  while 1 do
    coroutine.yield( YIELD_FRAME, 1 )
  end
end
--]]