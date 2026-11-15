-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--shield.lua

local dataMembers = { "ID" }

local function ShieldConstructor( self, ID, playerIndex, gridX, gridY )
  Structure.Init( self, ID, playerIndex, gridX, gridY )

  self.cooldownTime = serverSettings.shieldCooldown -- Get from serverSettings or something
  self.timer = self.cooldownTime 

  self.radius = 500
  
  self.rechargeRate = 1.0

  local script = self.entity:GetFirstComponent("script")

  script:SetUpdate( function() self:Main()  end )
  script:SetMessageHandler( function(msgType, stack) self:HandleMessage(msgType, stack) end )

  local color = GetPlayerColor(playerIndex)

  local models = self.entity:GetComponentsByType("model")

  for i = 1, #models do
    models[i]:SetGlowColor(color[1],color[2],color[3])
  end
end

Shield = ClassInheritable( Structure, ShieldConstructor )


function Shield.OnCreate( entity, playerIndex, gridX, gridY )
  local shield = Shield( entity:ID(), playerIndex, gridX, gridY )
  game.players[playerIndex].planet:AddStructure( shield, gridX, gridY )
  game.players[playerIndex].resources = game.players[playerIndex].resources - serverSettings.shieldCost
  return shield
end

function Shield.Create(playerIndex)
  local entity = EntityFactory:Create("SHIELD", playerIndex)
  return entity:GetLuaData()
end

EntityFactory:AddOnCreate("SHIELD", Shield.OnCreate )


function Shield:AddConnection(connection)
  Structure.AddConnection( self, connection )
  
  if connection:IsA( PowerPlant ) then
    self.rechargeRate = self.rechargeRate + 0.5
  end
end

function Shield:RemoveConnection(connection)
  Structure.RemoveConnection( self, connection )
  
  if connection:IsA( PowerPlant ) then
    self.rechargeRate = self.rechargeRate - 0.5
  end
end

function Shield:Main()
  local model = self.entity:GetComponentsByType("model")
  local physics = self.entity:GetFirstComponent("physics")
  local transform = self.entity:GetFirstComponent("transform")
  
  physics:SetBoundingRadius( 0 )
  
  while 1 do
    if self.timer > 0 then
      self.timer = self.timer - Game:FrameElapsed() * self.rechargeRate
      
      if self.timer <= 0 then
        physics:SetBoundingRadius( self.radius )
        self.timer = 0
      end
    end
    
    local t = 1 - self.timer / self.cooldownTime

    for i = 1, #model, 1 do
      if model[i] then
        model[i]:SetGlowAmount( t*t*t*t )
      end
    end
    
    
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
end

function Shield:HandleMessage(msgType, stack)
  if msgType == "PhysicsCollision" then
    local shield = EntityFactory:GetEntity(stack[1])
    local otherEntity = EntityFactory:GetEntity(stack[2])
    
    if otherEntity:Type() == "PROJECTILE" then
      -- TODO: Change to otherEntity:GetData() or whatever
      local projectile = game.projectiles[ stack[2] ]
      
      if projectile and projectile.alive then
        if projectile.playerIndex ~= self.playerIndex or projectile.age  > 100 then
          projectile.alive = false
          
          local shieldEffect = EntityFactory:Create( "SHIELD_EFFECT" )
          
          local shieldPhysics = self.entity:GetFirstComponent("physics")
          local shieldTransform = shield:GetFirstComponent("transform")
          local effectTransform = shieldEffect:GetFirstComponent("transform")
          
          local effectScript = shieldEffect:GetFirstComponent("script")
                  
          effectTransform:SetParent( shieldTransform )
          effectTransform:SetLocalPositionFromWorld( stack[3], stack[4], stack[5] )
          effectTransform:SetOrientationFromPosition( 0, 1, 0 )
          
          local sound = shield:GetFirstComponent("sound")
          
          if not sound then print ("WTF??") end
          
          if sound then
            sound:Play()
          end
          
          effectScript:SetUpdate( ShieldEffectUpdate )
          
          game.projectiles[stack[2]]:Delete()
          
          self.timer = self.cooldownTime
          shieldPhysics:SetBoundingRadius( 0 )
        end
      end
    end
  end
end

function ShieldEffectUpdate()
  local maxLifetime = 800
  local lifetime = maxLifetime

  local entity = EntityFactory:GetEntity( ScriptEngine:CurUpdateID() )
  
  local quad = entity:GetFirstComponent("world_quad")

  quad:SetScale(768)
  
  while 1 do
    if lifetime <= 0 then
      break
    else
      local t = 1 - lifetime / maxLifetime
      
      quad:SetTransparency( 0.8 * (1-t) )
      
      if not GameLib.PhysicsPaused() then 
        lifetime = lifetime - Game:FrameElapsed()
      end
    end
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
  
  EntityFactory:DeleteEntity( ScriptEngine:CurUpdateID() )
end

function RemoveShield(playerIndex, shieldID)   
  for i = 1, #game.players[ playerIndex ].planet.shields do
    if game.players[ playerIndex ].planet.shields[i].ID == shieldID then
    
      game.players[ playerIndex ].planet.shields[i]:Delete()
      
      table.remove( game.players[ playerIndex ].planet.shields, i)
      break
    end
  end
  
  game.shields[shieldID] = nil
end
