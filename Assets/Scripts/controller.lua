-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

-- controller.lua

local keyBinds = clientSettings.keyBinds

Controller = ClassSimple(
  function( self )
    self.enabled = true
  end
)

function Controller:Enable()
  self.enabled = true
end

function Controller:Disable()
  self.enabled = false
end


controller = Controller()


function GetEntity(sx, sy, group)
  local ray = {}
  
  ray.x, ray.y, ray.z, ray.dx, ray.dy, ray.dz = GraphicsEngine:ScreenToWorld(sx, sy)
  
  if not group then
    return PhysicsEngine:GetEntity(ray.x, ray.y, ray.z, ray.dx, ray.dy, ray.dz)
  else
    return PhysicsEngine:GetEntity(ray.x, ray.y, ray.z, ray.dx, ray.dy, ray.dz, group)
  end
end

-- Includes only menu controls (not HUD)
function UpdateMenuControls()
  if not controller.enabled then
  	return
  end
  
  if Input:IsTriggered( InputMap.Escape ) and not MainMenu:IsA(menu:Top()) then
    if menu:Top().back == nil or menu:Top().back == true then
      PopGameState()
      
      if( gameStateManager:Top() == gamePlayState ) then
        menu.HUD:Create()
      end
    end
  end
end

-- Includes all GUI controls (HUD, Menus, etc)
function UpdateGUIControls()
  if not controller.enabled then
  	return
  end
  
  MenuSystem:HandleInput()
end

function DevOn()
  clientSettings.developerControls = true
end

function DevOff()
  clientSettings.developerControls = false
end

function DevToggle()
  clientSettings.developerControls = not clientSettings.developerControls
end

function UpdateDeveloperGlobalControls()
  if Input:IsPressed( InputMap.LeftControl ) and Input:IsTriggered( InputMap.Enter ) then
    clientSettings.fullscreen = not clientSettings.fullscreen
    Game:FullscreenOn( clientSettings.fullscreen )
  end
  
  if Input:IsTriggered( InputMap.F1 ) then
    GraphicsEngine:Wireframe(true)
  end
  
  if Input:IsTriggered( InputMap.F2 ) then
    GraphicsEngine:Wireframe(false)
  end
  
  if Input:IsTriggered( InputMap.F9 ) then
    clientSettings.debugInfo = not clientSettings.debugInfo
  end
end

function UpdateDeveloperGameControls()
  if Input:IsTriggered( InputMap.P ) then
    GameLib.PausePhysicsToggle()
    SoundEngine:SoundFXPaused( not SoundEngine:SoundFXPaused() )
  end
  
  local func = Input.IsTriggered
 
  if clientSettings.rapidFire then
    func = Input.IsPressed
  end
  
  if func( Input, keyBinds.cameraFire ) then
    CreateRocketAtCursor()
  end
end

function UpdateGlobalControls()
  if not controller.enabled then
    return
  end
  
  if clientSettings.developerControls then
    UpdateDeveloperGlobalControls()
  end
end

function UpdateGameControls()
  if not controller.enabled then
  	return
  end
  
  if clientSettings.developerControls then
    UpdateDeveloperGameControls()
  end
  
  local lastEntityHover = game.entityHover
  
  local mouseX, mouseY = Input:MousePosition()
  game.entityHover, game.hoverPosition.x, game.hoverPosition.y, game.hoverPosition.z = GetEntity( mouseX, mouseY, "planets" )
  
--[[
  if Input:IsTriggered( InputMap.Tab ) then
    local menuState = MenuState( ScoreMenu() )
    
    menuState:AddCallback( "Updates",
      function()
        if not Input:IsPressed( InputMap.Tab ) then
          gameStateManager:Pop()
        end
      end )
      
    gameStateManager:Push( menuState )
  end
  --]]
  
  if Input:IsTriggered( InputMap.Escape ) then
    if game.crosshair then
      game.crosshair:Disable()
    end
    
    gameStateManager:Push( MenuState( PauseMenu() ) )
    menu.HUD:Delete()
  end
  
  if Input:IsTriggered( keyBinds.cameraAssist ) then
    if menu.HUD.cameraButton then
      clientSettings.cameraAssist = not clientSettings.cameraAssist
      
      if clientSettings.cameraAssist then
        menu.HUD.cameraButton:SetGraphic("camera_button", "UP")
      else
        menu.HUD.cameraButton:SetGraphic("camera_button_disabled", "UP")
      end
    end
  end

  if Input:IsTriggered( keyBinds.focusHome ) then
    FocusHomePlanet()
  end
  
  if Input:IsTriggered( keyBinds.fireMode ) then
    game.crosshair:Enable()
    game.crosshair.ctrl = true
  elseif game.crosshair.ctrl == true then
    if not Input:IsPressed( keyBinds.fireMode ) then
      game.crosshair:Disable()
    end
  end
  
  if game.crosshair:Enabled() then
    if Input:IsTriggered( InputMap.MouseLeft ) and not MenuSystem:ProcessedClick() then
      game.players[MyPlayerIndex()].planet:FireCannons( game.crosshair:Position() )
      
      game.crosshair:FadeOut()
      
      
      if Input:IsPressed( keyBinds.fireMode ) then
        game.crosshair = Crosshair()
        game.crosshair:Enable()
        game.crosshair.ctrl = true
        
      elseif not game.crosshair.ctrl then
      
        game.crosshair = Crosshair()
        game.crosshair:Enable()
        game.crosshair.ctrl = false
      else
        game.crosshair = Crosshair()
      end
    end
  else
    if Input:IsTriggered( InputMap.MouseLeft ) and not MenuSystem:ProcessedClick() then
      local mx, my = Input:MousePosition()
      local entity, x, y, z = GetEntity( mx, my, "planets" )
      
      if entity
      then
        game.camera:FocusEntity(entity, 1000)
      end
    end
  end
  
  if game.hoverText and (not game.entityHover or not game.entityHover:GetFirstComponent("planet")) then
    EntityFactory:DeleteEntity( game.hoverText:ID() )
    game.hoverText = nil
  end
  
  if game.entityHover and game.entityHover:GetFirstComponent("planet") then
    if not game.hoverText then
      game.hoverText = EntityFactory:Create("TEXT")
      
      local text = game.hoverText:GetFirstComponent("text")
      
      text:SetHorizontalAlignment("CENTER")
      text:SetFontSize("24")
    end
    
    local text = game.hoverText:GetFirstComponent("text")
    local textTransform = game.hoverText:GetFirstComponent("transform")
    local planetComponent = game.entityHover:GetFirstComponent("planet")
    local planetTransform = game.entityHover:GetFirstComponent("transform")
    
    local planetPos = Vec3( planetTransform:GetWorldPosition() )
    local upVector = Vec3( GraphicsEngine:GetCamera():GetUpVector() )
    planetPos = planetPos + upVector * planetComponent:GetMaxRadius() * 1.25
    
    local x, y = GraphicsEngine:ProjectVector( planetPos:Elements() )
    
    local str = "Unknown"
    local hoverid = game.entityHover:ID()
    
    local playerIndex = PlayerIndexFromPlanetID( hoverid )
    
    if playerIndex then 
      str = game.players[playerIndex].name
      local color = GetPlayerColor( playerIndex )
      text:SetTextColor( 255, 255, 255, 255, color[1], color[2], color[3] )
    end
    
    text:SetText( str )
    
    textTransform:SetPosition( x, y, 0 )
  end
  
  if game.hoverText and CurrentGameState() ~= gamePlayState then
    EntityFactory:DeleteEntity( game.hoverText:ID() )
    game.hoverText = nil
  end
end
