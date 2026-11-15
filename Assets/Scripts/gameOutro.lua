-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

-- gameOutro.lua

function UpdateGameOutro()
  gameOutroState.t = gameOutroState.t - Game:FrameElapsed()
  
  if gameOutroState.t <= 0 then
    while GameStateCount() > 1 do
      PopGameState()
    end

    PushGameState( MenuState( MainMenu ) )
  end
end

function EnterGameOutro()
  gameOutroState.t = serverSettings.introCameraTime * 0.4
  
  game.camera:Orphan()
  game.camera:SetMode(Camera.IDLE_MODE)
  
  GraphicsEngine:GetActionRegion():Clear()
  
  menu.HUD:Delete()
end

function LeaveGameOutro()
end

function InitGameOutroState()
  gameOutroState = GameState()
  
  gameOutroState.OnPush = EnterGameOutro
  gameOutroState.OnPop = LeaveGameOutro
  
  gameOutroState.GlobalUpdates = {
    UpdateGameOutro
  }
  
  gameOutroState.t = 0
end
