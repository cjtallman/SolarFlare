-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--muzzleFlash.lua

function Main()

  local lifetime = 200
  local timer = 0

  local entity = EntityFactory:GetEntity( ScriptEngine:CurUpdateID() )
  
  --local quad = entity:GetFirstComponent("world_quad")

  while 1 do
  
    if timer > lifetime then
      break
    end
    
    --local t = timer / lifetime
    --if quad then
      --quad:SetScale(t * 1500)
      --quad:SetColor(255,255,255,255*(1-t)*(1-t))
    --end
    
    if not GameLib.PhysicsPaused() then
      timer = timer + Game:FrameElapsed()
    end
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
  
  EntityFactory:DeleteEntity( ScriptEngine:CurUpdateID() )
end

---------------------------------------------------------------
-- transform is from the cannon that fired.
-- velocity is from the planet, implemented as Vec3.
function CreateMuzzleFlash( cannon_transform, planet_velocity, playerIndex )
  
  local flash_entity = EntityFactory:Create( "MUZZLE_FLASH" )
  if not flash_entity or not cannon_transform or not planet_velocity then
    return
  end
  
  local flash_transform = flash_entity:GetFirstComponent( "transform" )
  local flash_emitter = flash_entity:GetFirstComponent( "emitter" )
  
  if not flash_transform or not flash_emitter then
    return
  end
  
  flash_transform:SetParent( cannon_transform )
  flash_transform:SetPosition( 000, 300, 000 )
  flash_emitter:SetVelocity( planet_velocity.x, planet_velocity.y, planet_velocity.z )
  
  local color = GetPlayerColor(playerIndex)
  flash_emitter:SetStartColor( color[1],color[2],color[3], 50 )
  flash_emitter:SetEndColor( color[1],color[2],color[3], 0 )
 
 end
