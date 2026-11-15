-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--ai.lu

function InitAINames()
  AINames = { "Bob", "Frank", "Julie", "Mark", "Harold", "Joe",
              "Jane", "Pete", "Wilson", "Mortimer", "Charles", "Wilford",
              "Gerard", "Matt", "Albert", "George", "Fred", "Billy", 
              "Jenine", "Sally", "Jessie", "Raphael", "Donatello",
              "Splinter", "Wilbert", "Bert", "Ernie", "Bart", "John",
              "Chuck", "Howard", "Samantha", "Granny", "Archibald" }
end

function AIControllerConstructor( self, entityID, player )
  self.entity = EntityFactory:GetEntity(entityID)
  self.player = player
  
  self.buildTimerReset = 4000
  self.buildTimer = (0.5 + math.random() * 0.5) * self.buildTimerReset
  
  self.firingTimerReset = 3000
  self.firingTimer = (0.5 + math.random() * 0.5) * self.firingTimerReset
  
  self.cannonBias = 1.0
  self.shieldBias = 1.0
  self.powerPlantBias = 1.0
  
  self.target = nil
  
  self.buildFunction = Planet.CreateCannon
  self.buildCost = serverSettings.cannonCost
  
  self.clusterX = 0
  self.clusterY = 0
  
  local script = self.entity:GetFirstComponent("script")
  
  script:SetUpdate( function() self:Main() end )
  --script:SetMessageHandler( function() self:HandleMessage() end )
end

AIController = ClassSimple( AIControllerConstructor )

function AIController.OnCreate(entity, playerIndex)
  local AIController = AIController( entity:ID(), game.players[playerIndex] )
  
  return AIController
end

EntityFactory:AddOnCreate("AICONTROLLER", AIController.OnCreate)

function GetAIName()
  local name = table.remove( AINames, math.random( 1, #AINames ) )
  return name
end

function AddAIName( name )
  table.insert( AINames, name );
end

function AIController:Main()
  local planet = self.player.planet
  
  self.clusterY = math.random(0, planet.gridHeight)
  self.clusterX = math.random(0, planet.gridWidth[self.clusterY])
  
  while 1 do
    while gameStateManager:Top() ~= gamePlayState or GameLib.PhysicsPaused() do
      coroutine.yield( YIELD_FRAME, 1 )
    end
    
    if not self.player.planet.entity then
      break
    end
    
    if self.buildTimer > 0 then
      self.buildTimer = self.buildTimer - Game:FrameElapsed()
      
      if self.buildTimer <= 0 then
        self:ChooseNextStructure()
      end
    else
      if self.player.resources >= self.buildCost then
        self:BuildNextStructure()
        self.buildTimer = 0.5 * self.buildTimerReset + 0.5 * math.random() * self.buildTimerReset
      end
    end
    
    if self.firingTimer > 0 then
      self.firingTimer = self.firingTimer - Game:FrameElapsed()
      
      if self.firingTimer <= 0 then
        self:FindTarget()
        
        self:Fire()
        
        self.firingTimer = 0.5 * self.firingTimerReset + math.random() * 0.5 * self.firingTimerReset
      end
    end
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
end

function AIController:ChooseNextStructure()
  local cannonCount = 0
  local shieldCount = 0
  local powerPlantCount = 0
  local totalCount = 0
  
  local planet = self.player.planet
  
  for kA, vA in pairs(planet.structures) do
    for kB, vB in pairs(planet.structures[kA]) do
      if Cannon:IsA( planet.structures[i] ) then
        cannonCount = cannonCount + 1
      elseif Shield:IsA( planet.structures[i] ) then
        shieldCount = shieldCount + 1
      elseif PowerPlant:IsA( planet.structures[i] ) then
        powerPlantCount = powerPlantCount + 1
      end
      
      totalCount = totalCount + 1
    end
  end
  
  local cannonWeight = 0
  local shieldWeight = 0
  local powerPlanetWeight = 0
  
  if totalCount < 1 then
    cannonWeight = 1
    shieldWeight = 1
    powerPlanetWeight = 0
  else
    cannonWeight = 1 - cannonCount / totalCount
    shieldWeight = 1 - shieldCount / totalCount
    powerPlanetWeight = 1 - powerPlantCount / totalCount
  end
  
  -- Play-style bias
  cannonWeight = cannonWeight * self.cannonBias
  shieldWeight = shieldWeight * self.shieldBias
  powerPlanetWeight = powerPlanetWeight * self.powerPlantBias
  
  -- Normalize weights
  local totalWeight = cannonWeight + shieldWeight + powerPlanetWeight
  cannonWeight = cannonWeight / totalWeight
  shieldWeight = shieldWeight / totalWeight
  powerPlanetWeight = powerPlanetWeight / totalWeight
  
  local rand = math.random()
  
  if rand <= cannonWeight then
    self.buildFunction = Planet.CreateCannon
    self.buildCost = serverSettings.cannonCost
  elseif rand <= cannonWeight + shieldWeight then
    self.buildFunction = Planet.CreateShield
    self.buildCost = serverSettings.shieldCost    
  else
    self.buildFunction = Planet.CreatePowerPlant
    self.buildCost = serverSettings.powerPlantCost
  end
end

function AIController:BuildNextStructure()
  local planet = self.player.planet
  local structures = planet.structures
  
  local directions = { {0,0}, {1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,1}, {-1,-1}, {1,-1} }

  for magnitude = 1, 1 do
    for i = 1, #directions do
      local startX, startY = planet:GridMovement(self.clusterX, self.clusterY, directions[i][1] * magnitude, directions[i][2] * magnitude)
      
      for j = 1, #directions do
        local gridX, gridY = planet:GridMovement(startX, startY, directions[j][1], directions[j][2])
        
        if not planet:GetStructure(gridX, gridY) then
          self.buildFunction(planet, gridX, gridY)
          return
        end
      end
    end
  end
  
  for i = 1, 5 do
    if not planet:GetStructure(self.clusterX, self.clusterY) then
      break
    end
    
    self.clusterY = math.random(0, planet.gridHeight)
    self.clusterX = math.random(0, planet.gridWidth[self.clusterY])
  end

end

function AIController:FindTarget()
  if not self.player.planet then
    return
  end
  
  local myTransform = self.player.planet.entity:GetFirstComponent("transform")
  local myPosition = Vec3( myTransform:GetWorldPosition() )

  local minDistSquare = nil
  
  for i = 1, #game.players do  
    if game.players[i] ~= self.player then
      if game.players[i].planet.entity then
        local otherTransform = game.players[i].planet.entity:GetFirstComponent("transform")
        local otherPosition = Vec3( otherTransform:GetWorldPosition() )
        
        local v = otherPosition - myPosition
        local distSquare = v*v
        
        if (not minDistSquare or distSquare < minDistSquare) and game.players[1].planet then
          local cannonCount = self.player.planet:CannonCountInRange( otherPosition )
          
          if cannonCount > 0 then
            minDistSquare = distSquare
            self.target = otherPosition
          end
        end
      end
    end
  end
  
  if self.target then
    local targetVector = self.target - myPosition
    
    targetVector = targetVector * (0.2 + 0.5 * math.random())
  end
end

function AIController:Fire()
  if self.target then
    self.player.planet:FireCannons( self.target )
  end
end
