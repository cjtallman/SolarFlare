-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--planet.lua

local noises = { { "Perlin",  { octaves=4, scaleMin=-0.5 } },
                 { "Perlin",  { scaleMin=0.0, frequency=2, lacunarity=1.5 } },
                 { "Simplex", { octaves=4, scaleMin=-0.5 } },
                 { "Simplex", { octaves=4, scaleMin=0.0, frequency=3, persistence=.75, lacunarity=1.5 } },
                 { "HMF",     { scaleMin=-0.5 } },
                 { "HMF",     { octaves=4, scaleMin=0.0, lacunarity=3, offset=-.5 } },
                 { "RMF",     { scaleMin=0.0 } },
                 { "RMF",     { octaves=5, scaleMin=-0.5, lacunarity=5 } },
                 { "RMF",     { octaves=3, scaleMin=0.0, offset=0.1, gain=100, lacunarity=4 } },
                 { "FBM",     { octaves=3, lacunarity=1.35, scaleMin=-0.5 } },
                 { "FBM",     { octaves=3, hurstIndex=0.5, lacunarity=5, scaleMin=0.0 } }
               }

local function PlanetConstructor( self, ID, playerIndex )
  self.entity = EntityFactory:GetEntity(ID)
  self.ID = ID
  
  local noise = noises[ math.random(1, #noises) ]
  self.entity:GetFirstComponent("planet"):SetNoise( noise[1], noise[2] )
  
  self.playerIndex = playerIndex
  
  self.exploding = false
  
  self.satellites = {}
  
  -- Set up grid
  self.structures = {}
  self.gridHeight = 0
  self.gridWidth = {}
  
  self:InitGrid()
  
  self.distanceTimer = 1
  self.distanceTimerScale = 1
  self.distanceStart = 0
  self.distanceGoal = 0
  
  local script = self.entity:GetFirstComponent("script")
  script:SetMessageHandler( function(type,stack) self:HandleMessage(type,stack) end )
  script:SetUpdate( function() self:Main() end )
end


Planet = ClassSimple( PlanetConstructor )

function Planet.OnCreate(entity, playerIndex)
  local planet = Planet(entity:ID(), playerIndex)
  
  game.players[playerIndex].planet = planet
  
  GameLib.AddMessageSubscription( "Deform", entity:ID() )
  
  return planet
end

EntityFactory:AddOnCreate("PLANET", Planet.OnCreate)

function Planet:Main()
  while 1 do
    self:InterpolatePosition()
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
end

function Planet:InterpolatePosition()
  if self.distanceTimer < 1 then
    self.distanceTimer = self.distanceTimer + Game:FrameElapsed() * self.distanceTimerScale
    
    local planetOrbit = self.entity:GetFirstComponent("physics")
    local ellipse = self.entity:GetFirstComponent("ellipse")
    
    planetOrbit:SetPerihelionDistance( self.distanceStart + self.distanceTimer * (self.distanceGoal - self.distanceStart) )
    
    if ellipse then
      ellipse:SetMajorAxis( planetOrbit:GetMajorAxis() )
      ellipse:SetMinorAxis( planetOrbit:GetMinorAxis() )
    end
  end
end

function Planet:SetDistanceGoal(goal, time)
  --if self.distanceTimer >= 1 then
    local planetOrbit = self.entity:GetFirstComponent("physics")
    
    self.distanceTimer = 0
    self.distanceTimerScale = 1 / time
    self.distanceStart = planetOrbit:GetPerihelionDistance()
    self.distanceGoal = goal
  --end
end

function Planet:InitGrid()
  local turretSize = 150
  local planetRadius = serverSettings.planetRadius
  local graphic = self.entity:GetFirstComponent("planet")
  
  local CLat = 2 * math.pi * planetRadius
  local latIntervals = math.floor(CLat / turretSize / 2)
  
  self.gridHeight = latIntervals
  
  for i = 0, latIntervals do
    self.structures[i] = {}

    local phi = -math.pi/2 + math.pi * i / self.gridHeight 
    local CLong = 2 * math.pi * (planetRadius * math.sin(math.pi / 2 - phi))
    
    self.gridWidth[i] = math.floor(CLong / turretSize)
  end
end

function Planet:CreateSatellite()
  local satelliteEntity = EntityFactory:Create("SATELLITE")
  local satellite = Satellite(self, satelliteEntity:ID())
  
  table.insert( self.satellites, satellite )
  
  local transform = satellite.entity:GetFirstComponent("transform")
  local physics = satellite.entity:GetFirstComponent("physics")
  local graphic = satellite.entity:GetFirstComponent("model")
      
  transform:SetParent( self.entity:GetFirstComponent("transform") )
  
  physics:SetBoundingRadius( 150 )
  physics:SetMass( 100 )

  physics:SetPerihelionDistance(1400 + math.random() * 600)
  physics:SetSunDistance(0)
  
  physics:SetPeriod(60000)

  physics:SetTime( 0 )
  physics:SetRotationPeriod( 20000 )
  physics:SetTimeShift( math.random() )
  
  physics:SetOrientation(2 * math.pi * math.random(), 0, 0 )
  
  local omega = Vec3( -1 + 2 * math.random(), -1 + 2 * math.random(), -1 + 2 * math.random() )
  
  omega:Normalize()
  omega = omega * math.pi / 2 / 2000
  
  physics:SetRotationalVelocity( omega.x, omega.y, omega.z )
  
  graphic:SetScale( 100 + math.random() * 100 )
end

function Planet:Delete()
  self:ClearStructures()
  
  if self.entity then
    EntityFactory:DeleteEntity( self.entity:ID() )
    self.entity = nil
    self.ID = nil
  end
end

function Planet:MakeConnections(structure)
  local connectionLengthSq = 500 * 500
  
  local neighbours =
  {
    {-1,0},  {1,0},  {0,1},  {0,-1},
    {-1,-1}, {-1,1}, {1,-1}, {1,1}
  }
  
  for i = 1, #neighbours do
    local gridX, gridY = self:GridMovement(structure.gridX, structure.gridY, neighbours[i][1], neighbours[i][2])
    
    local otherStructure = self:GetStructure(gridX, gridY)
    
    if otherStructure and otherStructure ~= structure and not structure:Connected(otherStructure) then
      structure:AddConnection( otherStructure )
      otherStructure:AddConnection( structure )
    end
  end
end

function Planet:CreateStructure(gridX, gridY, StructureType)
  gridX, gridY = self:WrapGridCoords(gridX, gridY)
  EntityFactory:Create(StructureType, self.playerIndex, gridX, gridY)
end

function Planet:CreateCannon(x, y)
  self:CreateStructure(x, y, "CANNON")
  IncreaseStat( playerIndex, "your_towers_built" ) -- playerStats.lua  
end

function Planet:CreateShield(x, y)
  self:CreateStructure(x, y, "SHIELD")
end

function Planet:CreatePowerPlant(x, y)
  self:CreateStructure(x, y, "POWERPLANT")
end

function Planet:AddStructure( structure, gridX, gridY )
  -- Converting from grid coordinates, to spherical, to local cartesian
  local theta, phi = self:GridToSpherical(gridX, gridY)
  
  local position = SphericalToCartesian(theta, phi)
  position = position * GetPlanetSurfaceHeight( self.entity, position:Elements() )
  
  self.structures[gridY][gridX] = structure
  
  local planetTransform = self.entity:GetFirstComponent( "transform" )
  local structureTransform = structure.entity:GetFirstComponent( "transform" )
  
  structureTransform:SetParent( planetTransform )
  structureTransform:SetPosition( position.x, position.y, position.z )
  structureTransform:SetOrientationFromPosition(0, 1, 0)
  
  self:MakeConnections(structure)
end

function Planet:RemoveStructure(structureID)
  for kA, vA in pairs(self.structures) do
    for kB, vB in pairs(self.structures[kA]) do
      if vB.ID == structureID then
        vB:Delete()
        self.structures[kA][kB] = nil
        break
      end
    end
  end
end

function Planet:ClearStructures()
  for kA, vA in pairs(self.structures) do
    for kB, vB in pairs(self.structures[kA]) do
      EntityFactory:DeleteEntity( vB.ID )
    end
  end
  
  self.structures = {}
end

function Planet:ClearSatellites()
  while #self.satellites > 0 do
    local satelliteCount = #self.satellites
    
    self.satellites[satelliteCount]:Delete()
    
    table.remove( self.satellites )
  end
end

function Planet:DestroyStructures( impactPoint, impactRadius )
  local planetTransform = self.entity:GetFirstComponent("transform")
  
  local localImpactPoint = Vec3( planetTransform:WorldToLocal( impactPoint:Elements() ) )
  local theta, phi = CartesianToSpherical( localImpactPoint:Elements() )
  local gridX, gridY = self:SphericalToGrid( theta, phi )
  
  local impactRadiusSquared = impactRadius * impactRadius
  local cannons_destroyed = 0
  
  for i = -1, 1 do
    for j = -1, 1 do
      local structure = self:GetStructure( self:GridMovement(gridX, gridY, i, j) )
      if structure then
        local structurePosition = Vec3( structure.entity:GetFirstComponent("transform"):GetWorldPosition() )
        
        local difference = structurePosition - impactPoint
        
        local dot = difference:Dot( difference )
        
        if dot < impactRadiusSquared then
          cannons_destroyed = cannons_destroyed + 1
          GameLib.AddMessage("global", "RemoveStructure", 0, MSG_ALL, self.playerIndex, structure.ID )
        end
      end
    end
  end
  
  return cannons_destroyed
end

function Planet:GetStructure(gridX, gridY)
  gridX, gridY = self:WrapGridCoords(gridX, gridY)
  
  return self.structures[gridY][gridX]
end

function Planet:SetPosition(position)
  if self.entity then
    local planetOrbit = self.entity:GetFirstComponent("physics")
    local planetGraphic = self.entity:GetFirstComponent("planet")
    
    planetGraphic:SetMaxRadius( serverSettings.planetRadius )
    planetGraphic:SetMinRadius( serverSettings.planetRadius * .75 )
    
    planetOrbit:SetBoundingRadius( serverSettings.planetRadius * .95 )
    planetOrbit:SetMass( serverSettings.planetMass )

    local distance = serverSettings.minimumPlanetDistance + position * serverSettings.radialSpacing;
    local period = (serverSettings.period * distance / 10000) * 1000

    local t = 0
    
    if planetOrbit:GetPeriod() > 0 then
      t = planetOrbit:GetTime() / planetOrbit:GetPeriod()
    end

    planetOrbit:SetTime( t * period )
    planetOrbit:SetPeriod( period )
    planetOrbit:SetRotationPeriod( period / 2.15 )
    planetOrbit:SetTimeShift( math.random() )
    
    planetOrbit:SetPerihelionDistance( serverSettings.sunRadius + serverSettings.minimumPlanetDistance + (position-1) * serverSettings.radialSpacing )
    planetOrbit:SetSunDistance( 0 )
    planetOrbit:SetRotationalVelocity(0,4e-5 + math.random() * 2e-5,0)

    local ellipse = self.entity:GetFirstComponent("ellipse")
    
    if ellipse then
      ellipse:SetCenter( 0, 0, 0 )
      ellipse:SetMajorAxis( planetOrbit:GetMajorAxis() )
      ellipse:SetMinorAxis( planetOrbit:GetMinorAxis() )
    end
    
    t = position / #game.players
    
    local yaw =  -serverSettings.tiltRange * 0.5 + t * serverSettings.tiltRange
    
    planetOrbit:SetOrientation(yaw, 0, 0 )
    
    if planetEllipse then
      planetEllipse:SetOrientation(yaw, 0, 0)
    end
  end
end

function Planet:CannonCountInRange(target)
  if not self.entity then return 0 end

  local planetTransform = self.entity:GetFirstComponent("transform")
  
  local planetPos = Vec3( planetTransform:GetWorldPosition() )
  local targetVector = target - planetPos
  
  local localPoint = Vec3( planetTransform:WorldToLocal( target:Elements() ) ):Normalized()
  
  local theta, phi = CartesianToSpherical( localPoint:Elements() )
  local gridX, gridY = self:SphericalToGrid( theta, phi )

  -- Note: We always use the grid width at the equator
  local gridHeight = self.gridHeight
  local gridWidth = self.gridWidth[ math.floor(gridHeight / 2) ]

  -- We will span +/- 90 degrees about the y-axis
  local xRange = math.floor(gridWidth / 4)

  gridXEnd = gridX + xRange
  gridX = gridX - xRange
  gridY = math.floor(gridHeight / 2 + 0.5)
  
  local cannonCount = 0
  
  local cannons = {}
  
  while gridX <= gridXEnd do
    for i = -math.floor(gridHeight / 2 + 0.5), math.floor(gridHeight / 2 + 0.5) do
      local tmpX, tmpY = self:WrapGridCoords(gridX, gridY)
      tmpX, tmpY = self:GridMovement(gridX, gridY, 0, i)
      
      local cannon = self:GetStructure(tmpX, tmpY)
      
      if Cannon:IsA( cannon ) and cannon.cooldown <= 0 and cannon.fireDelay <= 0 then
        local found = false
        for i = 1, #cannons do
          if cannons[i] == cannon then
            found = true
            break
          end
        end
        
        if not found then
          table.insert(cannons, cannon)
        end
      end

    end
    
    gridX = gridX + 1
  end
  
  return #cannons
end

function Planet:FireCannons(target)
  if not self.entity then return end
  
  local planetPos = Vec3(self.entity:GetFirstComponent("transform"):GetWorldPosition())
  local targetDir = target - planetPos
  local targetDirMagnitude = targetDir:Magnitude()
  
  local planetTransform = self.entity:GetFirstComponent("transform")
    
  local localPoint = Vec3( planetTransform:WorldToLocal( target:Elements() ) ):Normalized()
  
  local theta, phi = CartesianToSpherical( localPoint:Elements() )
  local gridX, gridY = self:SphericalToGrid( theta, phi )
  
  local gridHeight = self.gridHeight
  local gridWidth = self.gridWidth[ math.floor(gridHeight / 2) ]
  
  local xRange = math.floor(gridWidth / 4)
  
  local delay = serverSettings.rocketStagger
  
  gridXEnd =  gridX + xRange
  gridX = gridX - xRange
  gridY = math.floor(gridHeight / 2 + 0.5)
  
  while gridX <= gridXEnd do
    for i = -math.floor(gridHeight / 2 + 0.5), math.floor(gridHeight / 2 + 0.5) do
      local tmpX, tmpY = self:WrapGridCoords(gridX, gridY)
      tmpX, tmpY = self:GridMovement(gridX, gridY, 0, i)
      
      local cannon = self:GetStructure(tmpX, tmpY)
      
      if Cannon:IsA( cannon ) and cannon.cooldown <= 0 and cannon.fireDelay <= 0 then
        -- local v = { gridX - i, gridY - j }
        
        -- if v[1]*v[1] + v[2]*v[2] <= xRange*xRange then
          local actualTarget = Vec3(
            target.x + (-1000 + 1000 * math.random()),
            target.y + (-1000 + 1000 * math.random()),
            target.z + (-1000 + 1000 * math.random()) )
            
            cannon:FireDelayed( actualTarget, math.random() * delay * 0.5 + delay * 0.5 )
            delay = delay + 0.5 * serverSettings.rocketStagger
        -- end
      end

    end
    
    gridX = gridX + 1
  end
end

function Planet:WrapGridY(gridY)
  if gridY < 0 then
    gridY = -gridY
  end
  
  if gridY > self.gridHeight then
    gridY = self.gridHeight - (gridY - self.gridHeight)
  end
  
  return gridY
end

function Planet:WrapGridX(gridX, gridY)
  if self.gridWidth[gridY] == 0 then
    gridX = 0
  else
    while gridX < 0 do 
      gridX = gridX + self.gridWidth[gridY]
    end
    
    gridX = gridX % self.gridWidth[gridY]
  end

  return gridX
end

function Planet:WrapGridCoords(gridX, gridY)
  if gridY < 0 then
    gridY = -gridY
    gridX = gridX + math.floor(self.gridWidth[gridY] / 2 + 0.5)
  end
  
  if gridY > self.gridHeight then
    gridY = self.gridHeight - (gridY - self.gridHeight)
    gridX = gridX + math.floor(self.gridWidth[gridY] / 2 + 0.5)
  end
  
  if self.gridWidth[gridY] == 0 then
    gridX = 0
  else
    while gridX < 0 do 
      gridX = gridX + self.gridWidth[gridY]
    end
    
    gridX = gridX % self.gridWidth[gridY]
  end
  
  return gridX, gridY
end

function Planet:SphericalToGrid(theta, phi)
  local gridY = math.floor((phi + math.pi/2) / math.pi * self.gridHeight + 0.5)
  local gridX = math.floor(theta / (2*math.pi) * self.gridWidth[gridY] + 0.5)
  
  return self:WrapGridCoords(gridX, gridY)
end

function Planet:GridToSpherical(gridX, gridY)
  gridX, gridY = self:WrapGridCoords(gridX, gridY)
  
  if gridY == 0 then
    return 0, -math.pi / 2
  elseif gridY == self.gridHeight then
    return 0, math.pi / 2
  else
    local phi = -math.pi / 2 + (gridY / self.gridHeight) * math.pi
    local theta = gridX / self.gridWidth[gridY] * 2 * math.pi
    
    return theta, phi
  end
end

function Planet:GridMovement(gridX, gridY, dx, dy)
  if self.gridWidth[gridY] == 0 then
    gridX, gridY = self:WrapGridCoords(gridX + dx, gridY + dy)
  elseif(self.gridWidth[ self:WrapGridY(gridY + dy) ] > self.gridWidth[gridY]) then
    -- For many-to-one, we move y before x
    local x0 = gridX / self.gridWidth[gridY]
    
    gridY = self:WrapGridY(gridY + dy)
    gridX = math.floor( x0 * self.gridWidth[gridY] + 0.5 )
    gridX = self:WrapGridX(gridX + dx, gridY)
  else
    -- For one-to-many, we move x before y
    gridX = self:WrapGridX(gridX + dx, gridY)
    gridX = math.floor( (gridX) / self.gridWidth[gridY] * self.gridWidth[ self:WrapGridY(gridY + dy) ] + 0.5 )
    gridY = self:WrapGridY(gridY + dy)
  end
  
  return gridX, gridY
end

function Planet:MsgPhysicsCollision( stack )
  if self.playerIndex ~= MyPlayerIndex() and not (game.players[self.playerIndex].AI and IsServer()) then 
    return
  end
  
  -- PhysicsCollision stack: myID, collidedWithID, x, y, z
  local proj = game.projectiles[ stack[2] ]
  
  if not proj or not proj.alive then
    return
  end

  -- world frame points
  local wx = stack[3]
  local wy = stack[4]
  local wz = stack[5]
  
  -- local frame points
  local planetTransform = self.entity:GetFirstComponent("transform")
  local lx, ly, lz = planetTransform:WorldToLocal(wx, wy, wz)

  -- default, for spaceDebris
  local deformPower = 1.5 + math.random()*2
  
  local planetComp = self.entity:GetFirstComponent("planet")
  local maxR = planetComp:GetMaxRadius()
  
  local fromIndex = proj.playerIndex
  if not fromIndex then
    fromIndex = -1
  end
  
  if not proj.isDebris then
    local cx, cy, cz = planetTransform:GetWorldPosition()
    
    local ev = Vec3( wx - cx, wy - cy, wz - cz )
    ev:Normalize()
    local sp = ev * maxR*1.1
         
    local planetPhy = self.entity:GetFirstComponent("physics")
    local planetVel = Vec3( planetPhy:GetVelocity() )
    
    local pos = Vec3( cx + sp.x, cy + sp.y, cz + sp.z )
    
    local maxGibs = math.random(3,6)
    for i = 1, maxGibs do
      SpaceDebris.Spawn( pos, ev, planetVel, 2 )
    end
    
    

    -- Does return number destroyed, if ever needed
    self:DestroyStructures( Vec3(wx, wy, wz), 200 )
    
    local h = GetPlanetSurfaceHeight(self.entity, lx, ly, lz)
    local planetComp = self.entity:GetFirstComponent("planet")
    
    if h - planetComp:GetMinRadius() < 0.0001 then
      GameLib.AddMessage("global", "PlanetDeath", 0, MSG_ALL, self.playerIndex, fromIndex)
    end
    
    deformPower = 4 + math.random() * 3
  end

  
  
  GameLib.AddMessage("global", "Deform", 0, MSG_ALL, self.ID, lx, ly, lz, deformPower, fromIndex)
end

function Planet:MsgDeform( stack )
  -- DeformMessage stack: local x, local y, local z, deform power
  if self.ID == stack[1] then
    local planetComp = self.entity:GetFirstComponent( "planet" )
    planetComp:Deform( stack[2], stack[3], stack[4], stack[5] )
    
    local fromIndex = stack[6]
    
    if fromIndex ~= -1 then
      local fromName = game.players[fromIndex].name
      local toName = game.players[self.playerIndex].name
      local c1 = GetPlayerColor(fromIndex)
      local c2 = GetPlayerColor(self.playerIndex)
      GraphicsEngine:Action(0, fromName, c1[1], c1[2], c1[3], toName, c2[1], c2[2], c2[3])
    end
  end
end

function Planet:MsgMiniExplosion( stack )
  --Stack: 1 = planetID
  local explosion = EntityFactory:Create("SMALL_EXPLOSION")
  local explosionTransform = explosion:GetFirstComponent("transform")

  local planet = EntityFactory:GetEntity( stack[1] )
  local planetGraphic = planet:GetFirstComponent("planet")
  local planetPhysics = planet:GetFirstComponent("physics")
  local planetTransform = planet:GetFirstComponent("transform")

  local theta, phi = math.random() * math.pi, math.random() * 2 * math.pi
  local r = planetGraphic:GetMaxRadius() * 0.9
  local pos = SphericalToCartesian(theta, phi) * r
  
  explosionTransform:SetParent(planetTransform)
  explosionTransform:SetPosition( pos:Elements() )
  
  local emitters = explosion:GetComponentsByType("emitter")

  for i = 0, #emitters, 1 do
    if emitters[i] and emitters[i].SetVelocity then
      emitters[i]:SetVelocity( planetPhysics:GetVelocity() )
    end
  end
end

function Planet:MsgBigExplosion( stack )
  --Stack: 1 = planetID
  local planet = EntityFactory:GetEntity( stack[1] )
  local planetPhysics = planet:GetFirstComponent("physics")
  local planetTransform = planet:GetFirstComponent("transform")

  local explosion = EntityFactory:Create("PLANET_EXPLOSION")
  local explosionTransform = explosion:GetFirstComponent("transform")
  
  explosionTransform:SetPosition( planetTransform:GetWorldPosition() ) 
  explosionTransform:SetRotation( 0, 0, 0 )
end

function Planet:HandleMessage( msgType, stack )
  local func = self["Msg" .. msgType]
  
  if func then
    func( self, stack )
  end
end

function PlanetAssignment()
  local planetPositions = {}
  
  for i = 1, #game.players do
    table.insert( planetPositions, i )
  end
  
  for i = 1, #game.players do
    local pos = math.random(1,#planetPositions)
    
    game.players[i].planet:SetPosition( planetPositions[pos] )
    
    table.remove( planetPositions, pos )
  end
end

function PlanetDeath( playerIndex, fromIndex )
  
  if game.players[playerIndex].planet.exploding then
    return
  end
    
  game.players[playerIndex].planet.exploding = true
  
  local planetID = game.players[playerIndex].planet.ID
  
  
  local t = 0
  while t < 2800 do
    GameLib.AddMessage("entity", "MiniExplosion", t, MSG_LOCAL, planetID, planetID )
    
    t = t + 100 + 100 * math.random()
  end

  GameLib.AddMessage("entity", "BigExplosion", 3000, MSG_LOCAL, planetID, planetID )
  GameLib.AddMessage("global", "DeletePlanet", 3100, MSG_LOCAL, playerIndex )
  
  EntityFactory:GetEntity(planetID):GetFirstComponent("physics"):SetTimeScale(0)
  
  if game.camera.entityFocus == planetID then
    game.camera:Orphan()
  end
  
  local fromName = ""
  local toName = game.players[playerIndex].name
  local c1 = GetPlayerColor(1)
  local c2 = GetPlayerColor(playerIndex)
  
  if game.players[fromIndex] then
    fromName = game.players[fromIndex].name
    c1 = GetPlayerColor(fromIndex)
  end
  GraphicsEngine:Action(1, fromName, c1[1], c1[2], c1[3], toName, c2[1], c2[2], c2[3])
end

function DeletePlanet(playerIndex)
  local planet = game.players[ playerIndex ].planet
  local center = Vec3( planet.entity:GetFirstComponent('transform'):GetWorldPosition() )
  
  planet:Delete()
  
  --
  -- Lots 'o gibs
  --
  local velAdd = Vec3()
  local temp = Vec3()

  for i = 1, 75 do
    temp.x = -1 + 2 * math.random()
    temp.y = -1 + 2 * math.random()
    temp.z = -1 + 2 * math.random()
    SpaceDebris.Spawn( center, temp, velAdd, 1.25, 1.5 + math.random()*2 )
  end
  

  game.livingPlayers = game.livingPlayers - 1
  
  if game.livingPlayers == 1 and not game.over then
    game.over = true
    GameLib.AddMessage("global", "EndGame", 1000, MSG_LOCAL )
  end
end

function ChunkUpdate( entityID )
  return function() 
    coroutine.yield( YIELD_TIME, 5000 )
    EntityFactory:DeleteEntity(entityID)
  end
end

