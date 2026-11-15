-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--hacks.lua

function LoadNathans()
  game.players[MyPlayerIndex()].resources = 5000
end

function LoadPatricks()
  for i = 1, 5 do game.players[ MyPlayerIndex() ].planet:CreateSatellite() end
end

function ToggleRadidFire()
  clientSettings.rapidFire = not clientSettings.rapidFire
end


local cr

function InitSupernovaState()
  supernovaState = GameState()
  
  supernovaState.OnPush =
    function()
      GraphicsEngine:ShowCursor(false)
      GraphicsEngine:ClearActions()
      game.crosshair:Disable()
      game.over = true
      menu.HUD:Delete()
      game.camera:SetMode( Camera.MENU_MODE )
      game.camera:FocusEntity( game.sun, 10000 )
      game.camera:SetRadius( serverSettings.sunRadius * 10, 10000 )
      cr = coroutine.create( DoSupernova )
      coroutine.resume(cr)
    end
  
  supernovaState.OnPop =
    function()
      if game.nova then
        local novaSFX = game.nova:GetFirstComponent("sound")
        
        if novaSFX then
          novaSFX:Stop()
        end
        
        EntityFactory:DeleteEntity( game.nova:ID() )
        
        game.nova = nil
      end
    end
  
  supernovaState.Updates = { function() coroutine.resume(cr,Game:FrameElapsed()) end }
end

function DoSupernova()
  local sunSize = serverSettings.sunRadius
  local dSize = 1
  
  SoundEngine:MusicPaused( true )
  
  local sunSFX = game.sun:GetFirstComponent("sound")
  
  if sunSFX then
    sunSFX:Play()
  end
  
  Wait( 2000 )
  

  
  while sunSize > 0 do
    sunSize = sunSize - dSize

	local t = sunSize / serverSettings.sunRadius
    GraphicsEngine:SetLightPercent( t )
    
    if dSize < 0 then dSize = 0 end
    
    if game.sun then
      game.sun:GetFirstComponent( "sun" ):SetMinRadius( sunSize )
      game.sun:GetFirstComponent( "sun" ):SetMaxRadius( sunSize )
    end
    
    for i = 1, #game.players do
		if game.players[i].planet.entity then
			local ellipse = game.players[i].planet.entity:GetFirstComponent("ellipse")
			ellipse:SetBrightness( t )
		end
	end
    
    if game.skyBox then
      game.skyBox:GetFirstComponent("skybox"):SetBrightness( 0.2 + t )
    end
    
    dSize = dSize + 0.5
    
    Wait(47)
  end
  
  
  Wait( 2500 )
  
  game.nova = EntityFactory:Create("SUPERNOVA")
  
  local novaSFX = game.nova:GetFirstComponent("sound")
  
  if novaSFX then
    novaSFX:Play()
  end
  
  for i = 1, #game.players do
    if game.players[i].planet.entity then
      GameLib.AddMessage("global", "PlanetDeath", 2000 + i * math.random() * 1000, MSG_ALL, i, -1)
    end
  end
  
  PhysicsEngine:SetGravitySource("sun", "asteroids", serverSettings.asteroidGravity*-120)
  
  for i = 1, 100 do
    if game.skyBox then
      game.skyBox:GetFirstComponent("skybox"):SetBrightness( 0.2 + i / 100 )
    end
    
    GraphicsEngine:SetLightPercent(i/25)
    
    Wait(1)
  end
  

  Wait(15000)
  
  
  DeleteSun()
  PushGameState( endGameState )
end
