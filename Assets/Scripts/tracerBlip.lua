-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--tracerBlip.lua

function Main()

  local lifetime = 1000
  local timer = 0

  local entity = EntityFactory:GetEntity( ScriptEngine:CurUpdateID() )
  
  local quad = entity:GetFirstComponent("world_quad")
  
  while 1 do
    if timer > lifetime then
      break
    end
    
    local t = timer / lifetime
    local s = 1 - t
    
    if quad then
      quad:SetScale(t * 500)
      quad:SetColor(255,255,255,255*s*s)
    end
    
    if not GameLib.PhysicsPaused() then
      timer = timer + Game:FrameElapsed()
    end
    
    coroutine.yield( 0, 1 )
  end
  
  EntityFactory:DeleteEntity( ScriptEngine:CurUpdateID() )
end
