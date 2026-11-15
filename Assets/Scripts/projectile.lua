-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--projectile.lua

Projectile = ClassSimple(
  function( self, ID, playerIndex, target )
    self.age = 0
    self.alive = true
    self.entity = EntityFactory:GetEntity( ID )
    self.ID = ID
    self.playerIndex = playerIndex

    self.target = target
    
    local script = self.entity:GetFirstComponent("script")
    
    script:SetMessageHandler( function(msgType, stack) self:HandleMessage(msgType, stack) end )
    script:SetUpdate( function() self:Main() end )
  end )

function Projectile.OnCreate( entity, playerIndex, targetX, targetY, targetZ )
  local target = nil
  
  if targetX then
    target = Vec3(targetX, targetY, targetZ)
  end
  
  local projectile = Projectile( entity:ID(), playerIndex, target )
  game.projectiles[entity:ID()] = projectile
end

function Projectile.OnDelete( entity )
  if game.projectiles[entity:ID()] then
    PhysicsEngine:RemoveEntity( entity:ID() )
    
    game.projectiles[entity:ID()] = nil
  end
end

EntityFactory:AddOnCreate("PROJECTILE", Projectile.OnCreate)
EntityFactory:AddOnDelete("PROJECTILE", Projectile.OnDelete)


function Projectile:SetEntity(projectileID)
  self.entity = EntityFactory:GetEntity( projectileID )
  self.ID = projectileID
end

function Projectile:Delete()
  if self.entity then
    EntityFactory:DeleteEntity( self.entity:ID() )
    
    self.entity = nil
    self.ID = nil
  end
end

local doCollides = { ['PLANET'] = true,
                     ['SUN'] = true,
                     ['ASTEROID'] = true,
                     ['SATELLITE'] = true
                   } 

function Projectile:HandleMessage(msgType, stack)
  if(msgType == "PhysicsCollision") then
    local thisEntity = EntityFactory:GetEntity(stack[1])
    local otherEntity = EntityFactory:GetEntity(stack[2])
    
    if not self.alive then
      self:Delete()
      return
    end
    
    local otherType = otherEntity:Type()
    
    if doCollides[ otherType ]
    then
      self.alive = false
      
      if otherType == "SUN" then
        IncreaseStat( self.playerIndex, "your_shots_that_hit_sun" )           -- playerStats.lua
      elseif otherType == "ASTEROID" then
        IncreaseStat( self.playerIndex, "your_shots_that_hit_asteroid" )      -- player.lua
      end
      
      --local this_transform = thisEntity:GetFirstComponent("transform")
      local other_physics = otherEntity:GetFirstComponent("physics")
      local other_transform = otherEntity:GetFirstComponent("transform")
      
      if otherType == "PLANET"then
        SpawnPlanetaryExplosion( other_transform, other_physics, stack[3], stack[4], stack[5], self.playerIndex ) -- explosion.lua
      else
        SpawnExplosion( other_transform, other_physics, stack[3], stack[4], stack[5], self.playerIndex )
      end
      
      if game.camera.entityFocus and game.camera.entityFocus == stack[1] then
        game.camera:Orphan()
      end
      
      self:Delete()
    end
  end
end

function Projectile:Main()
  local maxAge = serverSettings.rocketLifetime + math.random(-500, 500)
  
  local emitter = self.entity:GetFirstComponent("emitter")
  local model = self.entity:GetFirstComponent("model")
  local physics = self.entity:GetFirstComponent("physics")
  local transform = self.entity:GetFirstComponent("transform")
  
  physics:SetOrientationMode("FromVelocity")
  
  local guideTime = 1500
  local guidePower = 0.03
  
  local color = GetPlayerColor(self.playerIndex)
  
  if emitter then
    emitter:SetStartColor( color[1],color[2],color[3], 255 )
    emitter:SetEndColor( color[1],color[2],color[3], 0 )
  end
  
  if model then
    model:SetModelColor( color[1],color[2],color[3] )
    model:SetGlowColor( color[1],color[2],color[3] )
    model:SetGlowAmount( 1 )
  end
  
  while 1 do
    if not GameLib.PhysicsPaused() then
      self.age = self.age + Game:FrameElapsed()
      
      if self.age > maxAge then
      
        local rocket = game.projectiles[ScriptEngine:CurUpdateID()]
        
        local projectileTransform = EntityFactory:GetEntity( ScriptEngine:CurUpdateID() ):GetFirstComponent("transform")
        
        IncreaseStat( self.playerIndex, "your_shots_that_missed" ) 
        SpawnExplosionTimeout( projectileTransform, self.playerIndex ) -- explosion.lua
        
        
        if game.camera.entityFocus and game.camera.entityFocus == ScriptEngine:CurUpdateID() then
          game.camera:Orphan()
        end
        
        self:Delete()
      end
        
      if self.age <= guideTime and self.target then
        local t = self.age / guideTime  -- t = [0,1]
        local a = 1 - 4*(t - 0.5)*(t - 0.5) -- parabola where f(0) = 0, f(.5) = 1, f(1) = 0
        
        local impulse = self.target - Vec3( transform:GetWorldPosition() )
        
        impulse:Normalize()
        impulse = impulse * guidePower * (1-t) * Game:FrameElapsed()
        
        if impulse * Vec3(physics:GetVelocity()) < 0 then
          impulse = impulse * 2
        end
        
        physics:ApplyImpulse( impulse.x, impulse.y, impulse.z )
      end
    end
    
    -- TODO: Alter trail emitter to reflect age; stop emitting soon before death maybe
    
    coroutine.yield( 0, 1 );
  end
end

function CreateRocketAtCursor()
    local point = {}
    
    point.x, point.y = Input:MousePosition()
    
    local ray = {}
    
    ray.x, ray.y, ray.z, ray.dx, ray.dy, ray.dz = GraphicsEngine:ScreenToWorld(point.x, point.y)
    
    local projectileEntity = EntityFactory:CreateEntity("PROJECTILE", MyPlayerIndex())
    local projectilePhysics = projectileEntity:GetFirstComponent("physics")
    
    local offset = 500
    local velocity = 2.0
    
    projectilePhysics:SetMass(1)
    projectilePhysics:SetMaxVelocity(serverSettings.rocketMaxVelocity)
    projectilePhysics:SetPosition(ray.x + offset * ray.dx, ray.y + offset * ray.dy, ray.z + offset * ray.dz)
    projectilePhysics:SetVelocity(velocity * ray.dx, velocity * ray.dy, velocity * ray.dz)
    projectilePhysics:SetBoundingRadius(25)
end

function ClearProjectiles()
  for i, v in pairs( game.projectiles ) do
    game.projectiles[i]:Delete()
  end
end
