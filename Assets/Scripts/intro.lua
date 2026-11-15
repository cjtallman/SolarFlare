-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--intro.lua

function Wait( time, fn )
  while time > 0 do
    if fn then
      fn( time )
    end
    
    time = time - coroutine.yield()
  end
end

-- Silly hack, just so we can skip easier
-- TODO: Centalize wait/coroutine usage
local hackShouldSkip = false

function WaitHack( time )
  while (time > 0) and (not hackShouldSkip) do
    time = time - coroutine.yield()
  end
end

SplashMenu = ClassSimple(
  function( self, image, width, height )
    self.imageName = image
    self.root = nil
    self.image = nil
    self.timer = 0
    self.width = width
    self.height = height
    self.coroutine = nil
  end
)

function SplashMenu:Create()
  hackShouldSkip = false
  
  if not self.root then
    self.root = menu.root:CreateRegion()
      self.root:SetAlignment("CENTER", "MIDDLE")
      
    self.coroutine = coroutine.create(SplashMenu.DoStuff)
    coroutine.resume(self.coroutine, self)
  end
end

function SplashMenu:Delete()
  if self.root then
    self.root = nil
  end

  game.skyBox:GetFirstComponent("skybox"):SetBrightness(1.2)
end

function SplashMenu:DoStuff()
  self.width = 1000
  self.height = 264
  self.image = self.root:CreateImage()
  self.image:SetGraphic("intro_digipen", "UP")
  self.image:SetDimensions(self.width, self.height)
  
  self.root:SetAlpha(0)
  
  WaitHack( 4000 )
  
  self.root:FadeIn(4000)
  
  WaitHack( 4000 )
  
  self.root:FadeOut(2000)
  
  WaitHack( 2000 )
    
  self.width = 1024
  self.height = 768
  self.image:SetGraphic("intro_team", "UP")
  self.image:SetDimensions(self.width, self.height)
  self.root:FadeIn(4000)
  
  WaitHack( 4000 )
  
  self.root:FadeOut(2000)
  
  WaitHack( 2000 )
  
  self.root:Delete()
  
  gameStateManager:Pop()
  gameStateManager:Push( MenuState( MainMenu() ) )
end

function SplashMenu:DoMoreStuff()
  
end


function SplashMenu:Update()
  local t = math.min( self.timer / 10000, 1 )
  game.skyBox:GetFirstComponent("skybox"):SetBrightness(1.2 * t)

  coroutine.resume(self.coroutine, Game:FrameElapsed())

  if Input:IsAnyTriggered() then
    hackShouldSkip = true
  end
    
  local sw, sh = GraphicsEngine:GetScreenSize()
  
  if sw ~= self.width then
    local aspect = self.width / self.height
    self.image:SetDimensions(sw, sw / aspect)
  end
  
  self.timer = self.timer + Game:FrameElapsed()
end

function InitIntroState()
  splashDigipen = MenuState( SplashMenu("intro_digipen", 1000, 264) )
  splashTeam = MenuState( SplashMenu("intro_team", 1024, 768) )
end
