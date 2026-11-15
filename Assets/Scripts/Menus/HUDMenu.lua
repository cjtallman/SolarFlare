-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.


HUDMenu = ClassSimple(  function( self )
                          self.topRegion = nil
                          self.bottomRegion = nil
                          self.cameraButton = nil
                        end )

local BUTTON_W = 74
local BUTTON_H = 74

function TextMaker( inputVal, text )
  return string.format('%s\n(%s)', text, InputValToName(inputVal) )
end

function HUDMenu:Create()
  if not self.topRegion then
    self.topRegion = menu.root:CreateRegion()
      self.topRegion:SetAlignment("CENTER", "MIDDLE")
      self.topRegion:SetLayout("TopToBottom")
      self.topRegion:SetPositionMode("ABSOLUTE")
      self.topRegion:SetPosition("CENTER", "TOP")
      self.topRegion:SetMargin(10,0,0,0)

    self.gameTimer = self.topRegion:CreateText()
    self.gameTimer:SetUpdateFunction( UpdateGameTimer )
    self.gameTimer:SetFontSize(36)

    self.topRegion:FadeIn()
    MenuSystem:Format()
  end
  
  local kb = clientSettings.keyBinds
  
  if not self.bottomRegion then
    self.bottomRegion = menu.root:CreateRegion()
      self.bottomRegion:SetAlignment("CENTER", "BOTTOM")
      self.bottomRegion:SetLayout("LeftToRight")
      self.bottomRegion:SetPositionMode("ABSOLUTE")
      self.bottomRegion:SetPosition("CENTER", "BOTTOM")
      self.bottomRegion:SetMargin(0,0,10,0)

    local fireButton = self.bottomRegion:CreateHudButton()
      fireButton:SetDimensions(BUTTON_W,BUTTON_H)
      fireButton:SetGraphic("fire_button", "UP")
      fireButton:SetGraphic("fire_button_over", "OVER")
      fireButton:SetGraphic("fire_button_disabled", "DISABLED")
      fireButton:SetCallback("OnClick", function() game.crosshair:Toggle() end )
      fireButton.toolTip = ToolTip( TextMaker(kb.fireMode, "Fire"), fireButton)

    local cannonButton = self.bottomRegion:CreateHudButton()
      cannonButton:SetDimensions(BUTTON_W,BUTTON_H)
      cannonButton:SetGraphic("cannon_button", "UP")
      cannonButton:SetGraphic("cannon_button_over", "OVER")
      cannonButton:SetGraphic("cannon_button_disabled", "DISABLED")
      cannonButton:SetCallback("OnClick", StartCannonPlacement)
      cannonButton:SetUpdateFunction( UpdateResourceRequirement(serverSettings.cannonCost) )
      cannonButton:SetMargin(0,0,0,10)
      cannonButton.toolTip = ToolTip( TextMaker(kb.placeCannon, "Build Cannon" ), cannonButton)

    local shieldButton = self.bottomRegion:CreateHudButton()
      shieldButton:SetDimensions(BUTTON_W,BUTTON_H)
      shieldButton:SetGraphic("shield_button", "UP")
      shieldButton:SetGraphic("shield_button_over", "OVER")
      shieldButton:SetGraphic("shield_button_disabled", "DISABLED")
      shieldButton:SetCallback("OnClick", StartShieldPlacement)
      shieldButton:SetUpdateFunction( UpdateResourceRequirement(serverSettings.shieldCost) )
      shieldButton.toolTip = ToolTip( TextMaker(kb.placeShield, "Build Shield" ), shieldButton)

    local energyButton = self.bottomRegion:CreateHudButton()
      energyButton:SetDimensions(BUTTON_W,BUTTON_H)
      energyButton:SetGraphic("power_button", "UP")
      energyButton:SetGraphic("power_button_over", "OVER")
      energyButton:SetGraphic("power_button_disabled", "DISABLED")
      energyButton:SetCallback("OnClick", StartPowerPlantPlacement)
      energyButton:SetUpdateFunction( UpdateResourceRequirement(serverSettings.powerPlantCost) )
      energyButton:SetMargin(0,10,0,0)
      energyButton.toolTip = ToolTip( TextMaker(kb.placePower, "Build Power" ), energyButton)

    local homeButton = self.bottomRegion:CreateHudButton()
      homeButton:SetDimensions(BUTTON_W,BUTTON_H)
      homeButton:SetGraphic("home_button", "UP")
      homeButton:SetGraphic("home_button_over", "OVER")
      homeButton:SetCallback("OnClick", FocusHomePlanet)
      homeButton.toolTip = ToolTip( TextMaker(kb.focusHome, "Focus Home"), homeButton)

    local cameraButton = self.bottomRegion:CreateHudButton()
      cameraButton:SetDimensions(BUTTON_W,BUTTON_H)
      cameraButton:SetGraphic("camera_button", "UP")
      cameraButton:SetGraphic("camera_button_over", "OVER")
      cameraButton:SetCallback("OnClick",
        function()
          clientSettings.cameraAssist = not clientSettings.cameraAssist
          
          if clientSettings.cameraAssist then
            cameraButton:SetGraphic("camera_button", "UP")
          else
            cameraButton:SetGraphic("camera_button_disabled", "UP")
          end
        end )
      cameraButton.toolTip = ToolTip( TextMaker(kb.cameraAssist, "Camera Assist"), cameraButton)
      
    self.cameraButton = cameraButton
    
    self.bottomRegion:FadeIn()
    MenuSystem:Format()
  end
end


function HUDMenu:Delete()
  if self.topRegion then
    self.topRegion:FadeOut()
    self.topRegion:Close()
    self.topRegion = nil
  end
  
  if self.bottomRegion then
    self.bottomRegion:FadeOut()
    self.bottomRegion:Close()
    self.bottomRegion = nil
  end
  
  if menu.tooltip then
    menu.tooltip:Hide()
  end
  
  self.cameraButton = nil
end

function HUDMenu:Update()
  if Input:IsTriggered( clientSettings.keyBinds.placeCannon ) then
    StartCannonPlacement()
  elseif Input:IsTriggered( clientSettings.keyBinds.placeShield ) then
    StartShieldPlacement()
  elseif Input:IsTriggered( clientSettings.keyBinds.placePower ) then
    StartPowerPlantPlacement()
  end
end

function HUDMenu:Hide()
  if self.bottomRegion then
    self.bottomRegion:FadeOut()
  end
end

function HUDMenu:Show()
  if self.bottomRegion then
    self.bottomRegion:FadeIn()
  end
end

function FireMyCannons()
  game.players[MyPlayerIndex()]:FireCannons()
end

function StartCannonPlacement()
  game.crosshair:Disable()

  local playerIndex = MyPlayerIndex()
  
  if game.players[playerIndex].resources >= serverSettings.cannonCost then
    towerPlacementState.placementFunction = Planet.CreateCannon
    towerPlacementState.placementModelType = "CANNON_BUILD"
    
    if game.players[playerIndex] and CurrentGameState() ~= towerPlacementState then
      PushGameState( towerPlacementState )
    end
  end
end

function StartPowerPlantPlacement()
  game.crosshair:Disable()

  local playerIndex = MyPlayerIndex()

  if game.players[playerIndex].resources >= serverSettings.powerPlantCost then  
    towerPlacementState.placementFunction = Planet.CreatePowerPlant
    towerPlacementState.placementModelType = "POWERPLANT_BUILD"
    
    if game.players[playerIndex] and CurrentGameState() ~= towerPlacementState then
      PushGameState( towerPlacementState )
    end
  end
end

function StartShieldPlacement()
  game.crosshair:Disable()

  local playerIndex = MyPlayerIndex()
  
  if game.players[playerIndex].resources >= serverSettings.shieldCost then
    towerPlacementState.placementFunction = Planet.CreateShield
    towerPlacementState.placementModelType = "SHIELD_BUILD"
    
    if game.players[playerIndex] and CurrentGameState() ~= towerPlacementState then
      PushGameState( towerPlacementState )
    end
  end
end

function FocusHomePlanet()
  local playerIndex = MyPlayerIndex()
  
  -- Zoom to home if looking at another planet
  -- If already home, toggle geosync
  if game.players[ playerIndex ] and game.players[ playerIndex ].planet.entity then
    if game.players[playerIndex].planet.entity:ID() ~= game.camera.entityFocus then
      game.camera:FocusEntity( game.players[ playerIndex ].planet.entity, 1000 )
    else
      game.camera:ToggleViewMode()
    end
  end
end

function UpdateGameTimer(element)
  element:SetText( string.format("%02i:%02i", game.timer / 1000 / 60, (game.timer / 1000) % 60) )
end

function UpdateResourceRequirement(requirement)
  return  function(element)
            local playerIndex = MyPlayerIndex()
            if playerIndex then
              -- Constant fill, exact percentage
              element:SetFillPercent( game.players[playerIndex].resources / requirement )
            end
          end
end
