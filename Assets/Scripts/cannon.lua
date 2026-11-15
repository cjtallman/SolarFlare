-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--cannon.lua

local dataMembers = { "ID" }

local function CannonConstructor( self, ID, playerIndex, gridX, gridY )
  Structure.Init( self, ID, playerIndex, gridX, gridY )
  
  self.cooldown = serverSettings.firingCooldown
  
  self.target = Vec3()
  self.fireDelay = 0
  
  self.rechargeRate = 1.0
  
  local script = self.entity:GetFirstComponent("script")
  script:SetUpdate( function() self:Main() end )
  
  local color = GetPlayerColor(playerIndex)
  self.entity:GetFirstComponent("model"):SetGlowColor(color[1],color[2],color[3])
end

Cannon = ClassInheritable( Structure, CannonConstructor )


function Cannon.OnCreate(entity, playerIndex, gridX, gridY )
  local cannon = Cannon(entity:ID(), playerIndex, gridX, gridY)
  game.players[playerIndex].planet:AddStructure( cannon, gridX, gridY )
  game.players[playerIndex].resources = game.players[playerIndex].resources - serverSettings.cannonCost
  return cannon
end

function Cannon.Create(playerIndex)
  local entity = EntityFactory:Create("CANNON", playerIndex)
  return entity:GetLuaData()
end

EntityFactory:AddOnCreate("CANNON", Cannon.OnCreate)


function Cannon:AddConnection(connection)
  Structure.AddConnection( self, connection )
  
  if connection:IsA( PowerPlant ) then
    self.rechargeRate = self.rechargeRate + 0.5
  end
end

function Cannon:RemoveConnection(connection)
  Structure.RemoveConnection( self, connection )
  
  if connection:IsA( PowerPlant ) then
    self.rechargeRate = self.rechargeRate - 0.5
  end
end

function Cannon:Fire(target)
  local planet = EntityFactory:GetEntity( game.players[self.playerIndex].planet.ID )
  
  local planetTransform = planet:GetFirstComponent( "transform" )
  local planetGraphics = planet:GetFirstComponent( "planet" )
  local planetPhysics = planet:GetFirstComponent( "physics" )

  local cannonTransform = self.entity:GetFirstComponent( "transform" )

  self:PlaySound()
  
  local projectileEntity = EntityFactory:Create("PROJECTILE", self.playerIndex, target:Elements())
  
  local projectilePhysics = projectileEntity:GetFirstComponent("physics")
  
  local planetPos = Vec3( planetTransform:GetWorldPosition() )
  local planetVel = Vec3( planetPhysics:GetVelocity() )
  
  local dir = Vec3( cannonTransform:GetWorldPosition() ) - planetPos
  local h = planetGraphics:GetMaxRadius()
  
  dir:Normalize()
  
  local projPos = planetPos + (h + serverSettings.rocketSpawnOffset) * dir
  local projVel = dir * serverSettings.rocketVelocity + planetVel

  projectilePhysics:SetMass(1)
  projectilePhysics:SetMaxVelocity(serverSettings.rocketMaxVelocity)
  projectilePhysics:SetPosition(projPos.x, projPos.y, projPos.z)
  projectilePhysics:SetVelocity(projVel.x, projVel.y, projVel.z)
  projectilePhysics:SetBoundingRadius(25)
  
  
  -- MuzzleFlash.lua
  CreateMuzzleFlash( cannonTransform, Vec3( planetPhysics:GetVelocity() ), self.playerIndex )

  IncreaseStat( self.playerIndex, "your_shots_fired" ) --"playerStats.lua"
  
  self.cooldown = serverSettings.firingCooldown
end

function Cannon:FireDelayed( target, delay )
  self.target = target
  self.fireDelay = delay
end

function Cannon:PlaySound()
  local sfx = self.entity:GetFirstComponent( "sound" )
  if sfx then
    sfx:Play()
  end
end

function Cannon:Main()
  local model = self.entity:GetFirstComponent("model")
  
  while 1 do
    local t = 1 - self.cooldown / serverSettings.firingCooldown

    if self.cooldown > 0 then
      self.cooldown = self.cooldown - Game:FrameElapsed() * self.rechargeRate
      
      model:SetGlowAmount( t*t*t*t )
      
      if self.cooldown <= 0 then
        model:SetGlowAmount( 1 )
      end
    end
    
    if self.fireDelay > 0 then
      self.fireDelay = self.fireDelay - Game:FrameElapsed()
      
      if self.fireDelay <= 0 then
        self:Fire( self.target )
      end
    end
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
end

function RemoveCannon(playerIndex, cannonID)   
  for i = 1, #game.players[ playerIndex ].planet.cannons do
    if game.players[ playerIndex ].planet.cannons[i].ID == cannonID then
    
      game.players[ playerIndex ].planet.cannons[i]:Delete()
      
      table.remove( game.players[ playerIndex ].planet.cannons, i)
      break
    end
  end
end
