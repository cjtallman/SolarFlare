-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--serverBrowser.lua

ServerBrowser = ClassSimple(
  function( self )
    self.root = nil
    self.timeDelay = 1000
    self.serverRegion = nil
  end
)

function ServerBrowser:Create()
  Network:Mode( "FindServer" )
  
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment("CENTER", "MIDDLE")
    self.root:SetLayout("TopToBottom")
  
  local header = self.root:CreateHeaderText("LAN Servers")
    header:SetMargin(0,0,25,0)

  self.serverRegion = self.root:CreateRegion()
    self.serverRegion:SetAlignment("CENTER", "MIDDLE")
    self.serverRegion:SetLayout("TopToBottom")

  local backButton = self.root:CreateButton()
    backButton:SetText("Back")
    backButton:SetCallback("OnClick", PopGameState)
    backButton:SetMargin(40,0,0,0)

  self.root:Fade(0,1)
  MenuSystem:Format()
end

function ServerBrowser:Delete()
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
end

function ServerBrowser:Update()
  local region = self.serverRegion
  
  self.timeDelay = self.timeDelay + Game:FrameElapsed()
  
  if self.timeDelay > 1000 then
    self.timeDelay = 0
    
    for i = 1, #region.children do
      region.children[i]:Delete()
    end

    local servers = Network:ServerListLAN();
    
    for i = 1, #servers, 1 do
      local function JoinServer()
        if( Network:ConnectID( servers[i].ID ) ) then
          print( "Connecting to " .. servers[i].Address .. " ..." )
          
          gameStateManager:Push( MenuState( SetupMPMenu() ) )
        else
          print( "Unable to find server: " .. servers[i].Name )
        end
      end
      
      local button = region:CreateButton()
        button:SetText(servers[i].Name)
        button:SetCallback("OnClick", JoinServer)
    end
  end
end
