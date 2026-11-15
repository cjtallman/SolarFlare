-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

-- gameIntro.lua
-- TODO: rename things so as to not confuse with intro.lua (splash screens)

function RunGameIntro()
  GraphicsEngine:GetCamera():SetTargetOffset( 0.0, 500.0, 0.0, serverSettings.introCameraTime )
  GraphicsEngine:GetCamera():SetRadius( 5000, serverSettings.introCameraTime )
  GraphicsEngine:GetCamera():SetRotation( math.pi / 4, math.pi / 2, 0, serverSettings.introCameraTime )
  game.camera:FocusEntity( EntityFactory:GetEntity( MyPlanetID() ), serverSettings.introCameraTime )
  
  Wait( serverSettings.introCameraTime )
  
  while CurrentGameState() ~= gameIntroState do
    PopGameState()
  end
  
  PopGameState()
end

function UpdateGameIntro()
  coroutine.resume(gameIntroState.coroutine, Game:FrameElapsed())
end

function EnterGameIntro()
  GameLib.UnpausePhysics()

  gameIntroState.t = 1.0
    
  gameIntroState.coroutine = coroutine.create(RunGameIntro)
  coroutine.resume(gameIntroState.coroutine, gameIntroState)
end

function LeaveGameIntro()
  game.camera:SetRoll(not clientSettings.constrainCameraRoll)
  game.camera:SetMode(Camera.GAME_MODE)
    
  menu.HUD:Create()
end

function InitGameIntroState()
  gameIntroState = GameState()
  
  gameIntroState.OnPush = EnterGameIntro
  gameIntroState.OnPop = LeaveGameIntro
    
  gameIntroState.GlobalUpdates = {
    UpdateGameIntro
  }
    
  gameIntroState.t = 0
end
