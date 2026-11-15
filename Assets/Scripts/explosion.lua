-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--explosion.lua

function Main()

  local lifetime = 2000
  local timer = 0

  local entity = EntityFactory:GetEntity( ScriptEngine:CurUpdateID() )
  
  local quad = entity:GetFirstComponent("world_quad")
  
  local sfx = entity:GetFirstComponent("sound")
  if sfx then
    sfx:Play()
  end
  
  while 1 do
    if timer > lifetime then
      break
    end
    
    local t = timer / lifetime
    
    if quad then
      quad:SetScale(t * 2000)
      quad:SetTransparency((1-t)*(1-t))
    end
    
    if not GameLib.PhysicsPaused() then
      timer = timer + Game:FrameElapsed()
    end
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
  
  EntityFactory:DeleteEntity( ScriptEngine:CurUpdateID() )
end

function SpawnExplosion( planet_transform, planet_physics, x, y, z, playerIndex )
  local explosion = EntityFactory:Create( "EXPLOSION_SHIELD" )
  local transform = explosion:GetFirstComponent("transform")
  
  transform:SetPosition( x, y, z )
  
  SetExplosionColors( explosion, playerIndex )
end

function SpawnPlanetaryExplosion( planet_transform, planet_physics, x, y, z, playerIndex )
  local explosion = EntityFactory:Create( "EXPLOSION" )
  local transform = explosion:GetFirstComponent("transform")
  local world_quad = explosion:GetFirstComponent("world_quad")
  
  transform:SetParent( planet_transform )
  transform:SetLocalPositionFromWorld( x, y, z )
          
  local emitters = explosion:GetComponentsByType("emitter")
  for i = 1, #emitters do 
      emitters[i]:SetVelocity( planet_physics:GetVelocity() ) 
  end
  
  SetExplosionColors( explosion, playerIndex )
end

function SetExplosionColors( explosion, playerIndex )
  local world_quad = explosion:GetFirstComponent("world_quad")
  local emitters = explosion:GetComponentsByType("emitter")

  local color = GetPlayerColor(playerIndex)

  for i = 1, #emitters do
    emitters[i]:SetStartColor(color[1],color[2],color[3],255)
    emitters[i]:SetEndColor(color[1],color[2],color[3],0)
  end
  
  if world_quad then
    world_quad:SetColor(color[1],color[2],color[3],255)  
  end
          
    
end

function SpawnExplosionTimeout( projectile_transform, playerIndex )
    
    local explosion = EntityFactory:Create( "EXPLOSION_TIMEOUT" )
    local transform = explosion:GetFirstComponent("transform")
    local emitter = explosion:GetFirstComponent("emitter")
    
    transform:SetPosition( projectile_transform:GetWorldPosition() )
    
    local color = GetPlayerColor(playerIndex)
    emitter:SetStartColor(color[1],color[2],color[3],255)
    emitter:SetEndColor(255,255,255,0)

end
