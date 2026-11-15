-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

-- camera.lua

local function CameraConstructor( self, mode )
  self.mode = mode
  self.geoSyncOn = false
  self.entityFocus = nil
  self.cameraObj = GraphicsEngine:GetCamera()
  
  self.cameraObj:SetTarget( 0, 60000, 0, 0 )
  self.cameraObj:SetRadius( 0, 0 )
  self.cameraObj:SetRotation( 0, 0, 0, serverSettings.introCameraTime )
end

Camera = ClassSimple( CameraConstructor )

--
-- 'static' class data members, enum, what-have-you
--
Camera.IDLE_MODE = 1
Camera.FPS_MODE  = 2
Camera.GAME_MODE = 3
Camera.MENU_MODE = 4


function Camera:DisableFPSCam()
  self.mode = Camera.GAME_MODE
  Game:MouseCentered( false )
  self.cameraObj:EnableTargetMode()
end

function Camera:EnableFPSCam()
  self.mode = Camera.FPS_MODE
  Game:MouseCentered( true );
  self.cameraObj:DisableTargetMode()  
end

function Camera:IsFocusedOn( entity )
  if entity then
    return self.entityFocus == entity:ID()
  end
  
  return false
end

function Camera:FocusEntity( entity, interpolateTime )
  if entity and entity:ID() ~= self.entityFocus then
    interpolateTime = interpolateTime or 0
    
    local transform = entity:GetFirstComponent( "transform" )

    if transform then
      self.cameraObj:SetTarget( transform, interpolateTime )
      self.cameraObj:SetRotationTime( interpolateTime )
      self.entityFocus = entity:ID()
    end
  end
end

function Camera:Orphan()
  self.cameraObj:SetTarget( self.cameraObj:Transform():GetWorldPosition() )
  self.cameraObj:Transform():SetParent( nil )
  self.entityFocus = nil
end

function Camera:SetMode( mode )
  self.mode = mode
  
  if mode == Camera.IDLE_MODE then
    self.cameraObj:SetTarget( 0, 60000, 0, serverSettings.introCameraTime )
    self.cameraObj:SetRadius( 0, serverSettings.introCameraTime )
    self.cameraObj:SetRotation( 0, 0, 0, serverSettings.introCameraTime )
  end
end

function Camera:SetRadius( radius, time )
  time = time or 0
  self.cameraObj:SetRadius( radius, time )
end

function Camera:SetRoll( enabled )
  if enabled then
    self.cameraObj:EnableRoll()
  else
    self.cameraObj:DisableRoll()
  end
end

function Camera:ToggleViewMode()
  self.geoSyncOn = not self.geoSyncOn

  if self.geoSyncOn then
    self.cameraObj:EnableGeosynchOrbit()
    self.cameraObj:SetTargetOffset( 0.0, 0.0, 0.0, 500 )
    self.cameraObj:SetRotationTime( 500 )
  else
    self.cameraObj:DisableGeosynchOrbit()
    
    -- TODO: Get the planet's radius and use it instead of magic number
    self.cameraObj:SetTargetOffset( 0.0, 500.0, 0.0, 500 )
    self.cameraObj:SetRotationTime( 5000 )
  end
end

function Camera:EnterFireMode()
  if not clientSettings.cameraAssist then
    return
  end
  
  if not game.players[ MyPlayerIndex() ].planet.entity then
    return
  end

  if self.geoSyncOn then
    self.geoSyncOn = false
    self.cameraObj:DisableGeosynchOrbit()
  end
  
  -- local myPos = Vec3( game.players[ MyPlayerIndex() ].planet.entity:GetFirstComponent("transform"):GetWorldPosition() )
  
  -- local minDist = 0
  -- local minVector
  
  -- for i = 1, #game.players do
    -- if i ~= MyPlayerIndex() and game.players[i].planet.entity then
      -- local otherPos = Vec3( game.players[i].planet.entity:GetFirstComponent("transform"):GetWorldPosition() )
      
      -- local v = otherPos - myPos
      
      -- if minDist == 0 or v * v < minDist then
        -- minDist = v * v
        -- minVector = v
      -- end
    -- end
  -- end
  
  local yaw, pitch, roll = self.cameraObj:GetRotationAngles()
  
  -- if minIndex ~= -1 then
    -- pitch = math.acos( minVector:Normalized() * Vec3(0,0,1) )
    
    -- local cross = minVector:Cross( Vec3(0,0,1) )
    
    -- if cross * cross > 0 then
      -- pitch = -pitch
    -- end
  -- end
  
  if self.cameraObj:GetRadius() < 5000 then
    self.cameraObj:SetRadius( 5000, 500 )
  end
    
  if yaw > 3*math.pi/4 or yaw < -math.pi/4 then
    if yaw < -math.pi + math.pi / 12 or yaw > -math.pi + 3*math.pi / 10 then
      self.cameraObj:Rotate( -math.pi + math.pi/6 - yaw, 0, 0, 500 )
    end
  else
    if yaw < math.pi / 12 or yaw > 3*math.pi / 10 then
      self.cameraObj:Rotate( math.pi/6 - yaw, 0, 0, 500 )
    end
  end
  
  -- if yaw < 0 then
    -- pitch = pitch + math.pi
  -- end
  
  --self.cameraObj:SetRotation( yaw, pitch, roll, 500 )
  --self.cameraObj:Rotate( 0, pitch, 0, 500 )

  self.cameraObj:SetTargetOffset( 0.0, 750.0, 0.0, 500 )
  
end

function Camera:EnterBuildMode()
  if not clientSettings.cameraAssist or not game.players[ MyPlayerIndex() ].planet.entity then
    return
  end
  
  game.camera:FocusEntity( game.players[ MyPlayerIndex() ].planet.entity, 1000 )
    
  if not self.geoSyncOn then
    self.geoSyncOn = true
    self.cameraObj:EnableGeosynchOrbit()
  end
  
  if self.cameraObj:GetRadius() > 2500 then
    self.cameraObj:SetRadius( 2500, 500 )
  end

  self.cameraObj:SetTargetOffset( 0.0, 0.0, 0.0, 500 )
end


--
-- Mode Update Functions
-- 
function Camera:IdleUpdate( dt )
  self.cameraObj:Rotate( 0, 0.000075 * dt, 0 )
end

function Camera:FPSUpdate( dt )
  local dx, dy = Input:MouseDelta()
  local w, h = GraphicsEngine:GetScreenSize()
  
  self.cameraObj:Rotate( -dy/h * 3.14, dx/w * 6.28, 0, 50 )
  
  if Input:IsPressed( InputMap.W ) then
    self.cameraObj:Move( 0, 0, 3 * dt )
  end
  
  if Input:IsPressed( InputMap.S ) then
    self.cameraObj:Move( 0, 0, -3 * dt )
  end 
  
  if Input:IsPressed( InputMap.A ) then
    self.cameraObj:Move( 3 * dt, 0, 0.0 )
  end
  
  if Input:IsPressed( InputMap.D ) then
    self.cameraObj:Move( -3 * dt, 0, 0.0 )
  end
  
  if Input:IsPressed( InputMap.Q ) then
    self.cameraObj:Rotate( 0, 0, -0.001 * dt, 50 )
  end
  
  if Input:IsPressed( InputMap.E ) then
    self.cameraObj:Rotate( 0, 0, 0.001 * dt, 50 )
  end
  
  if Input:IsTriggered( InputMap.F ) then
    self:DisableFPSCam()
  end
end

function Camera:GameUpdate( dt )
  if clientSettings.developerControls then
    if Input:IsTriggered( InputMap.F ) then
      self:EnableFPSCam()
    end
  end
  
  if Input:IsPressed( clientSettings.keyBinds.rotateCamera ) then
    local dx, dy = Input:MouseDelta()
    self.cameraObj:Rotate( dy * .005, -dx * .005, 0, 500 )
  end

  if Input:IsTriggered( clientSettings.keyBinds.zoomIn ) then
    self.cameraObj:MultiplyRadius( 0.75, 500 )
  elseif Input:IsTriggered( clientSettings.keyBinds.zoomOut ) then
    self.cameraObj:MultiplyRadius( 1.25, 500 )
  end
end


local updates = { [Camera.IDLE_MODE] = Camera.IdleUpdate,
                  [Camera.FPS_MODE] = Camera.FPSUpdate,
                  [Camera.GAME_MODE] = Camera.GameUpdate,
                  [Camera.MENU_MODE] = function() end
                 }

function UpdateCamera()
  if controller.enabled then
    local dt = Game:FrameElapsed()
    updates[game.camera.mode]( game.camera, dt )
  end
end
