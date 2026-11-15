-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--crosshair.lua

Crosshair = ClassSimple(
  function( self )
    self.entity = nil
    self.transform = nil
    self.position = Vec3()
    
    self.textEntity = nil
    
    self.fade = false
    self.ctrl = false
  end )

function Crosshair:Enable()
  if not self.entity then
    self.entity = EntityFactory:Create("CROSSHAIR")
    self.transform = self.entity:GetFirstComponent("transform")
    
    local script = self.entity:GetFirstComponent("script")
    
    script:SetUpdate( function() self:Update() end )
  end
  
  if not self.textEntity then
    self.textEntity = EntityFactory:Create("TEXT")
  end
  
  game.camera:EnterFireMode()
end

function Crosshair:Disable()
  if self.entity then
    EntityFactory:DeleteEntity( self.entity:ID() )
    
    self.entity = nil
    self.transform = nil
    self.ctrl = false
  end
  
  if self.textEntity then
    EntityFactory:DeleteEntity( self.textEntity:ID() )

    self.textEntity = nil
  end

end

function Crosshair:Toggle()
  if self:Enabled() then
    self:Disable()
  else
    self:Enable()
  end  
end

function Crosshair:Enabled()
  if self.entity then
    return true
  else
    return false
  end
end

function Crosshair:Position()
  return self.position
end

function Crosshair:FadeOut()
  self.fade = true
end

function Crosshair:Update()
  local quad = self.entity:GetFirstComponent("world_quad")
  local model = self.entity:GetFirstComponent("model")
  
  quad:SetScale(1000)
  quad:SetColor(100,0,0,200)
  
  model:SetScale(500)
  model:SetModelColor(80,0,0,200)
  
  local updateTimer = 0
  
  while self.fade == false do
    if self.transform then
      local rayStart = Vec3()
      local rayDir = Vec3()
      
      rayStart.x, rayStart.y, rayStart.z, rayDir.x, rayDir.y, rayDir.z = GraphicsEngine:ScreenToWorld( Input:MousePosition() )

      local t = -rayStart.y / rayDir.y
      local target = rayStart + rayDir * t
      
      self.position = target
      self.transform:SetPosition( target.x, target.y, target.z )
      
      if self.textEntity then
        local text = self.textEntity:GetFirstComponent("text")
        local transform = self.textEntity:GetFirstComponent("transform")
        
        local planet = game.players[ MyPlayerIndex() ].planet
        
        local sx, sy  = Input:MousePosition()
        transform:SetPosition( sx + 30, sy + 30, 0 )
          
        
  
        if updateTimer <= 0 then
          if planet then
			
			
			local count = planet:CannonCountInRange(target)
			local t_count = math.clamp(count/10,0,1)
			
			quad:SetScale(1000 + (t_count) * 2000 )
			model:SetScale(500 + (t_count) * 1000 )
			
			quad:SetColor(100 + (t_count)*150,0,0,200)
			model:SetModelColor(80+ (t_count)*150,0,0,200)
			
            text:SetText( count )
          end

          updateTimer = 500
        else
          updateTimer = updateTimer - Game:FrameElapsed()
        end
      end
    end
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
  
  if self.textEntity then
    EntityFactory:DeleteEntity( self.textEntity:ID() )
  end
  
  if quad and model then
    local timer = 750
    
    local transform = self.entity:GetFirstComponent("transform")
    
    while timer > 0 do
      local t = timer  / 750
      
      quad:SetTransparency(t)
      quad:SetScale(1000 + 2000*(1-t))
      
      model:SetScale(600 * t)
      transform:SetRotation(0,math.pi*t,0)
      
      timer = timer - Game:FrameElapsed()
      
      coroutine.yield( YIELD_FRAME, 1 )
    end
  end
    
  if self.entity then
    EntityFactory:DeleteEntity( self.entity:ID() )
  end
  
end
