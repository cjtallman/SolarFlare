-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

-- creditsMenu.lua

CreditsMenu = ClassSimple(
  function( self )
    self.root = nil
    self.coroutine = nil
    self.movedFlags = { }
    self.textEntities = { }
  end
)

function CreditsMenu:Create()
  GameLib.UnpausePhysics()
  
  game.camera.cameraObj:SetRadius(1000)
  game.camera:SetMode(Camera.GAME_MODE)
  
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment('CENTER', 'BOTTOM')
    self.root:SetLayout('LeftToRight')
    self.root:SetPositionMode("ABSOLUTE")
    self.root:SetPosition("CENTER", "BOTTOM")
      
 --[[ 
  local backButton = self.root:CreateButton()
    backButton:SetText('Back')
    backButton:SetCallback('OnClick', PopGameState )
    backButton:SetMargin(40,0,0,0)
 --]]
 
  self.root:FadeIn()
  MenuSystem:Format()
  
  GraphicsEngine:ShowCursor(false)
  
  PhysicsEngine:SetGravitySource("sun", "asteroids", serverSettings.asteroidGravity)
  
  CreateSun()
  
  local r = 8000
  local C = 2 * math.pi * r
  local num = C / 1000

  local two_pi = 2 * math.pi
  local dt = two_pi / num

  game.camera.cameraObj:SetTarget( 0, 0, r + 3000, serverSettings.introCameraTime )
  game.camera.cameraObj:SetRotation( 0, math.pi * 0.5, 0, serverSettings.introCameraTime )
  
  for theta = 0, two_pi, dt do
    local cnt = math.random(3,6)

      local x = math.sin( theta )
      local z = math.cos( theta )

    for i = 1, cnt do
      local rand = math.random( 1, 3 )
      local entity = EntityFactory:Create( 'ASTEROID' .. rand )
      
      game.asteroids[ #game.asteroids + 1 ] = entity
      
      local physics = entity:GetFirstComponent( 'physics' )
      
      local rOff = r + math.random() * 2000 + 1500 * i
      
      physics:SetPosition( rOff * x, -1000 + math.random() * 2000, rOff * z )
      physics:SetMass( 1e4 )
      physics:SetOrientation( two_pi * math.random(), two_pi* math.random(), two_pi * math.random() )
      
      local G = serverSettings.asteroidGravity
      local v = math.sqrt( G * serverSettings.sunMass / rOff )
      
      physics:SetVelocity(v * -z, 0, v * x)
      
      local torque = Vec3( -1 + 2 * math.random(), -1 + 2 * math.random(), -1 + 2 * math.random() )
      torque:Normalize()
      torque = torque * (1e-5 + math.random() * 1e-5)
      physics:Torque( torque:Elements() )
      
      local scale = 200 + math.random() * 300
      physics:SetBoundingRadius( scale )
      entity:GetFirstComponent('model'):SetScale( scale )
    end
  end
  
  local hdrSize = 40
  local roleSize = 35
  local nrmlSize = 25
  local spaceSize = 40
  
  local credTexts = { { 'Scorched Muffin Entertainment', 65 },
                      { ' ', spaceSize },
                      { 'Presents', 30 },
                      { 'Solar Flare', 100 },
                      { ' ', spaceSize },
                      { 'Designer', roleSize }, 
                      { 'Patrick Mera', nrmlSize },
                      { ' ', spaceSize },
                      { 'Technical Director', roleSize },
                      { 'Chris Tallman', nrmlSize },
                      { ' ', spaceSize },
                      { 'Producer', roleSize },
                      { 'Nathan Williams', nrmlSize },
                      { ' ', spaceSize },
                      { 'Executive Producers', roleSize },
                      { 'Ben Ellinger', nrmlSize },
                      { 'Chris Peters', nrmlSize },
                      { 'Rachel Rutherford', nrmlSize },
                      { ' ', spaceSize },
                      { 'Special Thanks', hdrSize },
                      { 'Dave Saunders', nrmlSize },
                      { 'Allen Richter', nrmlSize },
                      { 'Sad Turtle Productions', nrmlSize },
                      { ' ', spaceSize },
                      { ' ', spaceSize },
                      { ' ', spaceSize },
                      { ' ', spaceSize },
                      { ' ', spaceSize },
                      { ' ', spaceSize },
                      { ' ', spaceSize },
                      { ' ', spaceSize }
                    }
  
  local sx, sy = GraphicsEngine:GetScreenSize()
  local hsx = sx / 2
  local accumHeight = sy
  
  local pos
  
  for i,v in ipairs(credTexts) do
    local entity = EntityFactory:CreateEntity('TEXT')
    local textCmp = entity:GetFirstComponent('text')
      textCmp:SetFont('stentiga')
      textCmp:SetFontSize( v[2] )
      textCmp:SetText( v[1] )
      textCmp:SetHorizontalAlignment("CENTER")
      textCmp:SetVerticalAlignment("TOP")
      textCmp:SetTextColor( 255,255,255,255,255,130,0,255 )
    
    local trans = entity:GetFirstComponent('transform')
      local height = textCmp:GetRegionHeight()
      trans:SetPosition( hsx, accumHeight, 0 )
      accumHeight = accumHeight + height + 5
    
    self.movedFlags[i] = 0
    self.textEntities[#self.textEntities + 1] = entity
  end
end

function CreditsMenu:Update()
  local sx, sy = GraphicsEngine:GetScreenSize()
  local hsx = sx/2
  
  local dy = Game:FrameElapsed() * -0.025
  
  local lasty = 0
  local lastHeight = sy
  
  local maxi = #self.movedFlags
  
  for i,v in ipairs( self.textEntities ) do
    local textCmp = v:GetFirstComponent('text')
      local width = textCmp:GetRegionWidth()
      local height = textCmp:GetRegionHeight()
    
    local trans = v:GetFirstComponent('transform')
    local x, y, z = trans:GetLocalPosition()
      
    y = y + dy
    
    if y < -height then
      self.movedFlags[i] = 1
      
      if i == 1 then
        if self.movedFlags[maxi] == 1 then 
          y = lasty + lastHeight
          self.movedFlags[i] = 2
        end
      elseif self.movedFlags[i-1] == 2 then
        y = lasty + lastHeight
        self.movedFlags[i] = 2
        
        if i == maxi then
          for j,k in ipairs(self.movedFlags) do
            self.movedFlags[i] = 0
          end
        end
      end
    end
    
    lasty = y
    lastHeight = height
    trans:SetPosition( hsx, y, 0 )
  end
end

function CreditsMenu:Delete()
  GameLib.PausePhysics()

  DeleteSun()

  for i,v in ipairs(game.asteroids) do
    EntityFactory:DeleteEntity( v:ID() )
  end
  
  game.asteroids = { }
  
  for i,v in ipairs(self.textEntities) do
    EntityFactory:DeleteEntity( v:ID() )
  end
  
  self.textEntities = { }

  self.coroutine = nil
  game.camera:SetMode( Camera.IDLE_MODE )

  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
end
