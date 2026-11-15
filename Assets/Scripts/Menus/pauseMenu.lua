-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--pauseMenu.lua

PauseMenu = ClassSimple(
  function( self )
    self.root = nil
  end
)

function PauseMenu:Create()
  game.camera:SetMode(Camera.MENU_MODE)
  
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment("CENTER", "MIDDLE")
    self.root:SetLayout("TopToBottom")

  --CreateButton("","Solar System", StartSolarSystemMenu)
  
  local optionsButton = self.root:CreateButton()
    optionsButton:SetText("Options")
    optionsButton:SetCallback("OnClick", function() gameStateManager:Push( MenuState( OptionsMenu() ) ) end )
  
  local quitButton = self.root:CreateButton()
    quitButton:SetText("Quit Game")
    quitButton:SetCallback("OnClick", ReturnToTitleScreen )
    
  local resumeButton = self.root:CreateButton()
    resumeButton:SetText("Resume Game")
    resumeButton:SetCallback(
      "OnClick",
      function()
        PopGameState()
        menu.HUD:Create()
      end )
    resumeButton:SetMargin(40,0,0,0)
    
  self.root:FadeIn()
  MenuSystem:Format()
end

function PauseMenu:Delete()
  game.camera:SetMode(Camera.GAME_MODE)
  
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
end

pauseMenuState = nil
pausedState = nil



function EnterPauseMenuState()
  EnterPausedState()
  
  menu.HUD:Delete()
  
  CreatePauseMenu()
end

function LeavePauseMenuState()
  LeavePausedState()
end

function ReturnToTitleScreen()
  -- TODO: Something a bit nicer...
  while gameStateManager:Top() ~= gamePlayState and
        gameStateManager:Top() ~= prototypeGameState and
        gameStateManager:Top() ~= prototypeRollingGameState do
    gameStateManager:Pop()
  end

  PushGameState( gameOutroState )
end

function EnterPausedState()
  if game.hoverText then
    EntityFactory:DeleteEntity( game.hoverText:ID() )
    game.hoverText = nil
  end
  
  GameLib:PausePhysics()
end

function LeavePausedState()
  GameLib:UnpausePhysics()
end
