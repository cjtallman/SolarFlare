-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--spaceDebris.lua

local function SpaceDebrisConstructor( self, entity )
  self.alive = true
  self.entity = entity
  self.isDebris = true
  self.ID = entity:ID()
  
  local num = math.random(1,4)
  
  local model = entity:GetFirstComponent('model')
    model:SetModel( 'spaceDebris_model' .. num )
    model:SetTexture( 'spaceDebris_texture' .. num )
  
  local script = self.entity:GetFirstComponent('script')
  script:SetUpdate( function() self:Update() end )
  script:SetMessageHandler( function(msgType, stack) self:HandleMessage(msgType, stack) end )
end


SpaceDebris = ClassSimple( SpaceDebrisConstructor )

function SpaceDebris.OnCreate( entity )
  local debris = SpaceDebris( entity )
  game.projectiles[entity:ID()] = debris
end

function SpaceDebris.OnDelete( entity )
  PhysicsEngine:RemoveEntity( entity:ID() )
  game.projectiles[entity:ID()] = nil
end


EntityFactory:AddOnCreate('SPACE_DEBRIS', SpaceDebris.OnCreate)
EntityFactory:AddOnDelete('SPACE_DEBRIS', SpaceDebris.OnDelete)


function SpaceDebris.Spawn( pos, velDir, velAdd, velMult, scaleMult )
  local gib = EntityFactory:CreateEntity( 'SPACE_DEBRIS' )
  local trans = gib:GetFirstComponent("transform")
    trans:SetOrientationFromPosition( 0, 1, 0, velDir.x, velDir.y, velDir.z )
    
  local xAxis = Vec3( trans:GetLocalXAxis() )
  local zAxis = Vec3( trans:GetLocalZAxis() )
  
  local a = -0.5 + math.random()
  local b = -0.5 + math.random()
  
  local vel = velDir + a * xAxis + b * zAxis;
    vel:Normalize()
    vel = vel * velMult
    vel = vel + velAdd
  
  local scale = (50 + math.random() * 75) * (scaleMult or 1)
    gib:GetFirstComponent('model'):SetScale(scale)
  
  local torque = Vec3( -1 + 2 * math.random(), -1 + 2 * math.random(), -1 + 2 * math.random() )
    torque:Normalize()
    torque = torque * (5e-4 + math.random() * 5e-4)
  
  local gibPhys = gib:GetFirstComponent('physics')
    gibPhys:SetBoundingRadius( scale )
    gibPhys:SetPosition( pos.x, pos.y, pos.z )
    gibPhys:SetVelocity( vel.x, vel.y, vel.z )
    gibPhys:Torque( torque.x, torque.y, torque.z )
  
  return gib
end



function SpaceDebris:Delete()
  if self.entity then
    EntityFactory:DeleteEntity( self.ID )
    
    game.projectiles[self.ID] = nil
    self.ID = nil
    self.entity = nil
  end
end

function SpaceDebris:Update()
  if self.alive then
    local model = self.entity:GetFirstComponent('model')
    
    local maxTime = 3500
    local t = maxTime
    model:SetGlowColor( 245, 20, 0 )
    
    while( t > 0 ) do
      if GameLib.PhysicsPaused() == false then
        t = t - Game:FrameElapsed()
        model:SetGlowAmount( 2 * t / maxTime )
      end
      coroutine.yield( YIELD_FRAME, 1 )
    end
  end
end

function SpaceDebris:HandleMessage( msgType, stack )
  if( msgType ~= 'PhysicsCollision' ) or not self.alive then
    return
  end
  
  -- PhysicsCollision stack: myID, collidedWithID, x, y, z
  local explosion = EntityFactory:Create( "EXPLOSION_ASTEROID" )
  local transform = explosion:GetFirstComponent("transform")
  local world_quad = explosion:GetFirstComponent("world_quad")
  
  local planet = EntityFactory:GetEntity(stack[2])
  local planetTrans = planet:GetFirstComponent('transform')
  local planetPhys  = planet:GetFirstComponent('physics')
  
  transform:SetParent( planetTrans )
  transform:SetLocalPositionFromWorld( stack[3], stack[4], stack[5] )
          
  local emitters = explosion:GetComponentsByType('emitter')
    
  self:Delete()
  self.alive = false
end
