-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--multiplayerMenu.lua

MultiPlayerMenu = ClassSimple(
  function( self )
    self.root = nil
  end
)

function MultiPlayerMenu:Create()
  Network:Mode("Idle")
  
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment("CENTER", "MIDDLE")
    self.root:SetLayout("TopToBottom")
    
  local header = self.root:CreateHeaderText("Multiplayer")
  
  local hostButton = self.root:CreateButton()
    hostButton:SetText("Host")
    hostButton:SetCallback("OnClick",
      function()
        Network:Mode( "Server" )
        gameStateManager:Push( MenuState( SetupMPMenu() ) )
      end )
  
  local refreshButton = self.root:CreateButton()
    refreshButton:SetText("LAN Servers")
    refreshButton:SetCallback("OnClick", function() gameStateManager:Push( MenuState( ServerBrowser() ) ) end )
    
  local backButton = self.root:CreateButton()
    backButton:SetText("Back")
    backButton:SetCallback("OnClick", PopGameState )
    backButton:SetMargin(40,0,0,0)
    
  self.root:FadeIn()
  MenuSystem:Format()
end

function MultiPlayerMenu:Delete()
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
end

function MultiPlayerMenu:Update()
  --TODO: Whatever your heart desires!
end
