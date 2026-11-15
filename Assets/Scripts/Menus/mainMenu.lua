-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--mainMenu.lua

MainMenu = ClassSimple(
  function( self )
    self.titleRegion = nil
    self.menuRegion = nil
  end )

function MainMenu:Create()
  game.skyBox:GetFirstComponent("skybox"):SetBrightness(1)
  GameLib.PausePhysics()
  
  GraphicsEngine:ShowCursor(true)

  Network:Mode("Idle")
  
  self.titleRegion = menu.root:CreateRegion()
    self.titleRegion:SetPositionMode("ABSOLUTE")
    self.titleRegion:SetPosition("CENTER", "TOP")
  
  local title = self.titleRegion:CreateImage()
    title:SetDimensions(548,160)
    title:SetGraphic("title")
  
  self.menuRegion = menu.root:CreateRegion()
    self.menuRegion:SetAlignment("CENTER", "MIDDLE")
    self.menuRegion:SetLayout("TopToBottom")
    self.menuRegion:SetMargin(100,0,0,0)
    
  local spButton = self.menuRegion:CreateButton()
    spButton:SetText("Single Player")
    spButton:SetCallback("OnClick", function() gameStateManager:Push( MenuState( SinglePlayerMenu() ) ) end  )
    
  local mpButton = self.menuRegion:CreateButton()
    mpButton:SetText("Multiplayer")
    mpButton:SetCallback("OnClick", function() gameStateManager:Push( MenuState( MultiPlayerMenu() ) ) end )
   
  local optionsButton = self.menuRegion:CreateButton()
    optionsButton:SetText("Options")
    optionsButton:SetCallback("OnClick", function() gameStateManager:Push( MenuState( OptionsMenu() ) ) end )
  
  local optionsButton = self.menuRegion:CreateButton()
    optionsButton:SetText("Credits")
    optionsButton:SetCallback("OnClick", function() gameStateManager:Push( MenuState( CreditsMenu() ) ) end )
  
  local quitButton = self.menuRegion:CreateButton()      
    quitButton:SetText("Quit")
    quitButton:SetCallback("OnClick", QuitGame )
    
  self.titleRegion:FadeIn()
  self.menuRegion:FadeIn()
  
  MenuSystem:Format()
end

function MainMenu:Delete()
  if self.menuRegion then
    self.menuRegion:FadeOut()
    self.menuRegion:Close()
    self.menuRegion = nil
  end
  
  if self.titleRegion then
    self.titleRegion:FadeOut()
    self.titleRegion:Close()
    self.titleRegion = nil
  end
end

function MainMenu:Update()
end

function QuitGame()
  Game:Quit()
end
