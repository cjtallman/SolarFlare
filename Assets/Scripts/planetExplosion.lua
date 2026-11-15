-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--planetExplosion.lua

function Main()

  local lifetime = 5000
  local timer = 0

  local entity = EntityFactory:GetEntity( ScriptEngine:CurUpdateID() )
  
  local quad = entity:GetFirstComponent("world_quad")
  
  local sfx = entity:GetFirstComponent( "sound" )
  if sfx then
    sfx:Play()
  end
  
  while 1 do
    if timer > lifetime then
      break
    end
    
    local t = timer / lifetime
    local s = 1 - t
    
    if quad then
      quad:SetScale(t * 20000)
      quad:SetColor(255,200,128,255*s*s)
    end
    
    if not GameLib.PhysicsPaused() then
      timer = timer + Game:FrameElapsed()
    end
    
    coroutine.yield( 0, 1 )
  end
  
  EntityFactory:DeleteEntity( ScriptEngine:CurUpdateID() )
end
