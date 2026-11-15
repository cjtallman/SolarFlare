-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--tracer.lua

function HandleMessage(type, stack)
  if(type == "PhysicsCollision") then
    if  EntityFactory:GetEntity(stack[2]):GetComponentByName("physics", "projectilePhysics") or
        EntityFactory:GetEntity(stack[2]):GetComponentByName("physics", "tracerPhysics")
    then
      return
    end
    
    --local collisionObject = EntityFactory:GetEntity(stack[2])
    --local collisionPoint = Vec3(stack[3], stack[4], stack[5])
    --
    --local tracer = EntityFactory:GetEntity(stack[1])
    --local tracerTransform = tracer:GetFirstComponent("transform")
    --local tracerPoint = Vec3( tracerTransform:GetWorldPosition() )
    --
    --local objectTransform = collisionObject:GetFirstComponent("transform")
    --local objectPoint = Vec3( objectTransform:GetWorldPosition() )
    --
    --local blip = EntityFactory:Create("TRACER_BLIP")
    --local blipTransform = blip:GetFirstComponent("transform")
    --
    --blipTransform:SetPosition( tracerPoint.x, tracerPoint.y, tracerPoint.z )
    --blipTransform:SetOrientationFromPosition(
      --0, 1, 0,
      --objectPoint.x, objectPoint.y, objectPoint.z )
    
    EntityFactory:DeleteEntity(stack[1])
  end
end

function Main()
  local time = serverSettings.tracerLifetime
  
  while 1 do
    time = time - Game:FrameElapsed()
  
    if time <= 0 then
      EntityFactory:DeleteEntity( ScriptEngine:CurUpdateID() )
    end
    
    coroutine.yield( 0, 1 );
  end

end

function CreateTracer( playerIndex, cannonID )
  local planet = EntityFactory:GetEntity( game.players[playerIndex].planet.ID )
  local cannon = EntityFactory:GetEntity( cannonID )
  
  if not planet or not cannon then
    return
  end
  
  local cannonTransform = cannon:GetFirstComponent( "transform" )
  local planetTransform = planet:GetFirstComponent( "transform" )
  local planetGraphics = planet:GetFirstComponent( "planet" )
  local planetPhysics = planet:GetFirstComponent("physics")

  local tracer = EntityFactory:Create( "TRACER" )
  local tracerPhysics = tracer:GetFirstComponent("physics")
  
  local velocity = serverSettings.rocketVelocity

  -- Projectile spawn point = PlanetCenter + (Cannon - Planet) * MaxRadius + offset
  local px, py, pz = planetTransform:GetWorldPosition()
  local dx, dy, dz = cannonTransform:GetRotatedLocalPositionNormalized()
  local h = planetGraphics:GetMaxRadius()
  
  local x = px + (h + serverSettings.rocketSpawnOffset) * dx
  local y = py + (h + serverSettings.rocketSpawnOffset) * dy
  local z = pz + (h + serverSettings.rocketSpawnOffset) * dz

  -- Moving reference frame (planet)
  local PVx, PVy, PVz = planetPhysics:GetVelocity()

  tracerPhysics:SetMass(1)
  tracerPhysics:SetMaxVelocity(serverSettings.rocketMaxVelocity)
  tracerPhysics:SetPosition(x, y, z)
  tracerPhysics:SetVelocity(dx * velocity + PVx, dy * velocity + PVy, dz * velocity + PVz)
  tracerPhysics:SetBoundingRadius(25)
end
