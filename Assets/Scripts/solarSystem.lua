-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--solarSystem.lua

function CreateSun()
  local sun = EntityFactory:Create( "SUN" )
  local sunRadius = serverSettings.sunRadius

  local sunPhysics = sun:GetFirstComponent("physics")

  if sunPhysics then
    sunPhysics:SetMass( serverSettings.sunMass )
    sunPhysics:SetBoundingRadius( sunRadius )
    sunPhysics:SetRotationPeriod( 60000 + math.random(0,20) * 10000 )
  end

  local sunComp = sun:GetFirstComponent( "sun" )

  sunComp:SetNoise( "HMF", {width=128, height=128} )
  sunComp:SetMinRadius( serverSettings.sunRadius )
  sunComp:SetMaxRadius( serverSettings.sunRadius + 100 )
  sunComp:StartAnimation( 0.005 )

  game.sun = sun

  GameLib.AddMessage( "global", "SunCreated", 0, MSG_ALL_PEERS, sun:ID() )
end

function DeleteSun()
  if game.sun then
    if game.camera.entityFocus == game.sun:ID() then
      game.camera:Orphan()
    end
    
    EntityFactory:DeleteEntity( game.sun:ID() )
    game.sun = nil
  end
end

function UpdatePlanets()
  for i = 1, #game.players do
    game.players[i].planet:SetPosition(i)
  end
end
