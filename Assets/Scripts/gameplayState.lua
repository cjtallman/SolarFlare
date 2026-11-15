-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--gameplayState.lua

game = {}

game.AIPlayerCount = 0
game.asteroids = {}
game.camera = nil
game.crosshair = nil
game.entityHover = nil
game.hoverPosition = {}
game.hoverText = nil
game.livingPlayers = 0
game.myIndex = nil
game.over = false
game.sun = nil
game.tracerTimer = 0
game.skyBox = nil
--game.roundTimer = nil
game.timer = serverSettings.roundDuration

game.cannons = {}
game.projectiles = {}
game.players = {}
game.shields = {}

function UpdateRoundTimer()
  if GameLib.PhysicsPaused() or game.over then
    return
  end

  --if game.roundTimer:Update() == false then
    --print( "Round Ended. (10 seconds)" )
  --end
  
  if game.timer > 0 then
    game.timer = game.timer - Game:FrameElapsed()
    
    -- These don't appear to be used - 03/16/2009 @ 18:25:44 - nwilliam
    --local t = 1 - game.timer / serverSettings.roundDuration
    --t = t * t

    if game.timer <= 0 then
      game.timer = 0
      GameLib.AddMessage("global", "EndGameSupernova", 1000, MSG_LOCAL )
    end
  end
end

function CreateGameEntities()
  CreateSun()
  PlanetAssignment()
  
  --[[
  -- Commented out, since we are moving towards creating the through gameplay
  local r = AsteroidDistance()
  local C = 2 * math.pi * r
  
  for i = 1, C / 1000 do
    CreateAsteroid()
  end
  --]]
end

function InitGame()
  PhysicsEngine:SetGravitySource("sun", "projectiles", 1.0)
  --PhysicsEngine:SetGravitySource("sun", "tracers", 1.0)
  PhysicsEngine:SetGravitySource("sun", "asteroids", serverSettings.asteroidGravity*50)
  PhysicsEngine:SetGravitySource("planets", "projectiles", 1.0)
  --PhysicsEngine:SetGravitySource("planets", "tracers", 1.0)
  PhysicsEngine:SetGravitySource("planets", "asteroids", serverSettings.asteroidGravity*1.75)
  --PhysicsEngine:SetGravitySource("asteroids", "projectiles", 1.0)
  
  PhysicsEngine:SetCollision("sun", "projectiles", true)
  --PhysicsEngine:SetCollision("sun", "tracers", true)
  PhysicsEngine:SetCollision("planets", "projectiles", true)
  --PhysicsEngine:SetCollision("planets", "tracers", true)
  --PhysicsEngine:SetCollision("asteroids", "projectiles", true)
  --PhysicsEngine:SetCollision("asteroids", "tracers", true)
  PhysicsEngine:SetCollision("shields", "projectiles", true)
  PhysicsEngine:SetCollision("planets", "asteroids", true)
  PhysicsEngine:SetCollision("sun", "asteroids", true)
    
  game.livingPlayers = #game.players
  game.over = false
  game.timer = serverSettings.roundDuration
  
  if Network:Mode() == "Idle" then
    CreateGameEntities()
  end
  


  --game.roundTimer = Timer( serverSettings.roundTime, true )
  --game.roundTimer:Start()
  
end

function EndGame()
  if game.hoverText then
    EntityFactory:DeleteEntity( game.hoverText:ID() )
    game.hoverText = nil
  end
  
  if game.crosshair then
    game.crosshair:Disable()
  end
  
  game.roundTimer = nil
  
  ClearAsteroids()
  ClearProjectiles()
  ClearPlayers()
  DeleteSun()

  while #game.players > 0 do
    table.remove( game.players )
  end
  
  GraphicsEngine:ResetLights()
  
  GameLib.ClearParticles()
end

function InitGameplayState()
  gamePlayState = GameState()
  
  gamePlayState.OnPush = InitGame
  gamePlayState.OnPop = EndGame
  
  gamePlayState.OnLeave =
    function()
      if game.hoverText then
        EntityFactory:DeleteEntity( game.hoverText:ID() )
        game.hoverText = nil
      end
    end
  
  gamePlayState.GlobalUpdates = {
    UpdateRoundTimer
  }
    
  gamePlayState.Updates = {
    function() menu.HUD:Update() end,
    UpdateMenus,
    UpdateGameControls
  }
  
  game.camera = Camera( Camera.IDLE_MODE )
end

