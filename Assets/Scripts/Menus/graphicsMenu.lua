-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--graphicsMenuState.lua

GraphicsMenu = ClassSimple(
  function( self )
    self.root = nil
  end
)

function GraphicsMenu:Create()
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment("CENTER", "MIDDLE")
    self.root:SetLayout("TopToBottom")
  
  local header = self.root:CreateHeaderText("Graphics Options")
    
  local fullscreenButton = self.root:CreateButton()
    fullscreenButton:SetText("Toggle Fullscreen")
    fullscreenButton:SetCallback("OnClick", MenuToggleFullscreen)
    
  local particlesButton = self.root:CreateButton()
    particlesButton:SetTextBoolean(clientSettings.displayParticles, "Particles On", "Particles Off")
    particlesButton:SetCallback("OnClick", MenuToggleParticles(particlesButton))
    
  local postProcessButton = self.root:CreateButton()
    postProcessButton:SetTextBoolean(clientSettings.postProcessing, "Post Process On", "Post Process Off")
    postProcessButton:SetCallback("OnClick", MenuTogglePostProcessing(postProcessButton))
  
  local cameraRollButton = self.root:CreateButton()
    cameraRollButton:SetTextBoolean(clientSettings.constrainCameraRoll, "Camera Roll Off", "Camera Roll On")
    cameraRollButton:SetCallback("OnClick", MenuToggleCameraRoll(cameraRollButton))
  
  local backButton = self.root:CreateButton()
    backButton:SetText("Back")
    backButton:SetCallback("OnClick", PopGameState )
    backButton:SetMargin(40,0,0,0)
    
  self.root:FadeIn()
  MenuSystem:Format()
end

function GraphicsMenu:Delete()
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
end

function MenuToggleFullscreen()
  clientSettings.fullscreen = not clientSettings.fullscreen
  Game:FullscreenOn(clientSettings.fullscreen)
end

function MenuToggleParticles(button)
  return  function()
            clientSettings.displayParticles = not clientSettings.displayParticles
            
            button:SetTextBoolean(clientSettings.displayParticles, "Particles On", "Particles Off")
            
            if clientSettings.displayParticles then
              GameLib.EnableParticles()
            else
              GameLib.DisableParticles()
            end
          end
end

function MenuTogglePostProcessing(button)
  return  function()
            clientSettings.postProcessing = not clientSettings.postProcessing
            
            button:SetTextBoolean(clientSettings.postProcessing, "Post Process On", "Post Process Off")

            if clientSettings.postProcessing then
              GraphicsEngine:PostProcess(true)
            else
              GraphicsEngine:PostProcess(false)
            end
          end
end

function MenuToggleCameraRoll(button)
  return  function()
            clientSettings.constrainCameraRoll = not clientSettings.constrainCameraRoll
            
            button:SetTextBoolean(clientSettings.constrainCameraRoll, "Camera Roll Off", "Camera Roll On")
            game.camera:SetRoll( not clientSettings.constrainCameraRoll )
          end
end
