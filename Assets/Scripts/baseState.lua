-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--baseState.lua

function CreateSkyBox()
  DestroySkyBox()
  game.skyBox = EntityFactory:Create( "SKYBOX" )
  game.skyBox:GetFirstComponent("skybox"):SetBrightness(0)
end

function DestroySkyBox()
  if game.skyBox then
    EntityFactory:DeleteEntity( game.skyBox:ID() )
  end
  
  game.skyBox = nil
end

function EnterBaseState()
  CreateSkyBox()
  --InitTimeScale()
end

function LeaveBaseState()
  DestroySkyBox()
end

function InitBaseState()
  baseState = GameState()
  
  baseState.OnPush = EnterBaseState
  baseState.OnPop = LeaveBaseState
  
  baseState.GlobalUpdates = {
    UpdateGlobalControls,
    UpdateGUIControls,
    UpdateCamera
    --UpdateTimeScale
  }
end
