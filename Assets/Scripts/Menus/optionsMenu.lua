-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--optionsMenu.lua

OptionsMenu = ClassSimple(
  function( self )
    self.root = nil
  end
)

function OptionsMenu:Create()
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment("CENTER", "MIDDLE")
    self.root:SetLayout("TopToBottom")
  
  local header = self.root:CreateHeaderText("Options")

  local gameButton = self.root:CreateButton()
    gameButton:SetText("Game")
    gameButton:SetCallback("OnClick", function() gameStateManager:Push( MenuState( GameOptionsMenu() ) ) end ) 
  
  local graphicsButton = self.root:CreateButton()
    graphicsButton:SetText("Graphics")
    graphicsButton:SetCallback("OnClick", function() gameStateManager:Push( MenuState( GraphicsMenu() ) ) end ) 

  local audioButton = self.root:CreateButton()
    audioButton:SetText("Audio")
    audioButton:SetCallback("OnClick", function() gameStateManager:Push( MenuState( AudioMenu() ) ) end )
    
  local backButton = self.root:CreateButton()
    backButton:SetText("Back")
    backButton:SetCallback("OnClick", PopGameState )
    backButton:SetMargin(40,0,0,0)
    
  self.root:FadeIn()
  MenuSystem:Format()
end

function OptionsMenu:Delete()
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end

  SavePersistentSettings()
end
