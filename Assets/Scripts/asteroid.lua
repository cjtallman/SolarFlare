-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--asteroid.lua

local dataMembers = {}

Asteroid = ClassSimple( function( self, ID ) 
                          self.entity = EntityFactory:GetEntity( ID )
                          self.ID = ID
                        end )

function Asteroid.OnCreate( entity )
  game.asteroids[entity:ID()] = Asteroid( entity:ID() )
end

function Asteroid.OnDelete( entity )
  game.asteroids[entity:ID()] = nil
end

EntityFactory:AddOnCreate("ASTEROID1", Asteroid.OnCreate)
EntityFactory:AddOnCreate("ASTEROID2", Asteroid.OnCreate)
EntityFactory:AddOnCreate("ASTEROID3", Asteroid.OnCreate)

EntityFactory:AddOnDelete("ASTEROID1", Asteroid.OnDelete)
EntityFactory:AddOnDelete("ASTEROID2", Asteroid.OnDelete)
EntityFactory:AddOnDelete("ASTEROID3", Asteroid.OnDelete)

function Asteroid:CreateEntity()
  self.ID = self.entity:ID()
end

function Asteroid:SetEntity(asteroidID)
  self.entity = EntityFactory:GetEntity( asteroidID )
  self.ID = asteroidID
end

function Asteroid:Delete()
  if self.entity then
    EntityFactory:DeleteEntity( self.entity:ID() )
    
    self.entity = nil
    self.ID = nil
  end
end

function Asteroid:SetDistance(distance)
  local asteroidPhysics = self.entity:GetFirstComponent("physics")

  local theta = math.random() * 6.28 * 10
  
  local x = math.sin( theta )
  local z = math.cos( theta )
  
  local G = serverSettings.asteroidGravity
  local v = math.sqrt( G * serverSettings.sunMass / distance )
  
  asteroidPhysics:SetPosition(distance * x, -500 + 1000 * math.random(), distance * z)
  asteroidPhysics:SetVelocity(v * -z, 0, v * x)
end

function Asteroid:SetMass(mass)
  local asteroidPhysics = self.entity:GetFirstComponent("physics")
  asteroidPhysics:SetMass(1e4)
end

function Asteroid:SetRotation()
  local asteroidPhysics = self.entity:GetFirstComponent("physics")
  
  asteroidPhysics:SetOrientation(2 * math.pi * math.random(), 2 * math.pi * math.random(), 2 * math.pi * math.random())
  
  local torque = Vec3(
    -1 + 2 * math.random(),
    -1 + 2 * math.random(),
    -1 + 2 * math.random())
  
  torque:Normalize()
  torque = torque * (1e-5 + math.random() * 1e-5)
  
  asteroidPhysics:Torque(torque.x, torque.y, torque.z)
end

function Asteroid:SetSize(size)
  local asteroidPhysics = self.entity:GetFirstComponent("physics")
  local asteroidGraphic = self.entity:GetFirstComponent("model")

  asteroidPhysics:SetBoundingRadius(size)
  asteroidGraphic:SetScale(size)
end

function ClearAsteroids()
  for i, v in pairs(game.asteroids) do
    v:Delete()
  end
  
  game.asteroids = {}
end

function CreateAsteroid()
  local asteroidEntity = nil

  local number = math.random(1,3)
  if number == 1 then
    asteroidEntity = EntityFactory:Create( "ASTEROID1" )
  elseif number == 2 then
    asteroidEntity = EntityFactory:Create( "ASTEROID2" )
  else
    asteroidEntity = EntityFactory:Create( "ASTEROID3" )
  end
  
  local asteroid = game.asteroids[ asteroidEntity:ID() ]
  
  asteroid:SetDistance( AsteroidDistance() )
  asteroid:SetMass(1e4)
  asteroid:SetRotation()
  asteroid:SetSize(200 + math.random() * 300)
end

function AsteroidDistance()
  return  serverSettings.sunRadius + 
          serverSettings.minimumPlanetDistance +
          (#game.players + 1) * serverSettings.radialSpacing + 
          (-0.5 + 3.5 * math.random()) * serverSettings.radialSpacing
end

--[[
function HandleMessage(type, stack)
  if(type == "PhysicsCollision") then
  
  end
end
--]]