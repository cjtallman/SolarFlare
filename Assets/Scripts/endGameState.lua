-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

EndGameMenu = ClassSimple(  function( self )
                              self.root = nil
                              self.scoreMenu = nil
                            end )
                            
function EndGameMenu:Create()
  self.scoreMenu = ScoreMenu()
  
  self.scoreMenu:Create()
  
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment("CENTER", "TOP")
    self.root:SetLayout("TopToBottom")
    self.root:SetMargin(25,0,0,0)
  
  local backButton = self.root:CreateButton()
    backButton:SetText("Main Menu")
    backButton:SetCallback("OnClick", ReturnToTitleScreen)

  local quitButton = self.root:CreateButton()
    quitButton:SetText("Exit Game")
    quitButton:SetCallback("OnClick", QuitGame)
  
    
  self.root:Fade(0,1)
  
  MenuSystem:Format()
end

function EndGameMenu:Delete()
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
  
  if self.scoreMenu then
    self.scoreMenu:Delete()
    self.scoreMenu = nil
  end
end

function InitEndGameState()
  local scoreMenu = ScoreMenu()
  
  endGameState = GameState()
  
  endGameState.OnPush =
    function()
      GraphicsEngine:ShowCursor(true)
      game.over = true
      menu.HUD:Delete()
      EndGameMenu:Create()
    end
    
  endGameState.OnPop =
    function()
      EndGameMenu:Delete()
    end
    
end
