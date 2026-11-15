-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--serverSettings.lua

serverSettings = {}

function serverSettings:SyncIn(msgStack)
  for i, v in pairs( serverSettings ) do 
    serverSettings[i] = msgStack[i]
  end
end

function serverSettings:SyncInField(field, value)
  serverSettings[field] = value
end

function serverSettings:SyncOut(field, value)
  if field and value then
    GameLib.AddMessage("global", "SyncSingleSetting", 0, MSG_ALL_PEERS, field, value )
  else
    local msgStack = {}
    
    for i, v in pairs( serverSettings ) do 
      msgStack[i] = v
    end
    
    GameLib.AddMessage("global", "SyncAllSettings", 0, MSG_ALL_PEERS, msgStack )
  end
end

function serverSettings:Set(field, value)
  serverSettings[field] = value
  serverSettings:SyncOut(field, value)
end

--  Game Settings

--  Intro settings (camera interp. time and control lock time)
serverSettings.introCameraTime    = 5000  --10000
serverSettings.introControlsTime  = 4000  --8500

--  Cooldown times
serverSettings.firingEnabled = true
serverSettings.firingCooldown = 12000
serverSettings.shieldCooldown = 7000

--  Asteroid Settings
serverSettings.asteroidGravity = 0.015

--  Rocket Settings
serverSettings.rocketLifetime = 7000
serverSettings.rocketMaxVelocity = 13.0
serverSettings.rocketStagger = 150
serverSettings.rocketSpawnOffset = 250
serverSettings.rocketVelocity = 13.0

-- Planet Settings
serverSettings.period = 60
serverSettings.tiltRange = 0.00
serverSettings.planetCount = 12
serverSettings.planetMass = 13.0e4
serverSettings.planetRadius = 900
serverSettings.radialSpacing = serverSettings.planetRadius * 3
serverSettings.minimumPlanetDistance = serverSettings.planetRadius * 10

-- Resource Settings
serverSettings.roundDuration = 15 * 60 * 1000
serverSettings.resourceStart = 50
serverSettings.resourceRate = (100 / 60 / 1000) + .0005

-- Sun Settings
serverSettings.sunMass = 15.0e4
serverSettings.sunRadius = 5000

-- Structure costs
serverSettings.cannonCost = 18
serverSettings.powerPlantCost = 15
serverSettings.shieldCost = 21
