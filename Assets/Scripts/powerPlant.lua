-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--powerPlant.lua

local function PowerPlantConstructor( self, ID, playerIndex, gridX, gridY )
  Structure.Init( self, ID, playerIndex, gridX, gridY )
  
  local script = self.entity:GetFirstComponent("script")
  
  script:SetUpdate( function() self:Main()  end )
  --script:SetMessageHandler( function(msgType, stack) self:HandleMessage(msgType, stack) end )

  local color = GetPlayerColor(playerIndex)
  local model = self.entity:GetFirstComponent("model")
  model:SetGlowColor(color[1],color[2],color[3])
  model:SetGlowAmount( 1 )
  
  local connectionEmitter = self.entity:GetComponentByName( "emitter", "connectionEmitter" )
  
  connectionEmitter:SetColor( color[1], color[2], color[3] )
end

PowerPlant = ClassInheritable( Structure, PowerPlantConstructor )

function PowerPlant:AddConnection(connection)
  Structure.AddConnection( self, connection )
  
  if not PowerPlant:IsA( connection ) then
    local connectionEmitter = self.entity:GetComponentByName( "emitter", "connectionEmitter" )
    
    connectionEmitter:AddConnection( self.ID, connection.ID )
  end
end

function PowerPlant:RemoveConnection(connection)
  Structure.RemoveConnection( self, connection )
  
  local connectionEmitter = self.entity:GetComponentByName( "emitter", "connectionEmitter" )

  connectionEmitter:RemoveConnection( self.ID )
end

function PowerPlant.OnCreate( entity, playerIndex, gridX, gridY )
  local powerPlant = PowerPlant( entity:ID(), playerIndex, gridX, gridY )
  game.players[playerIndex].planet:AddStructure( powerPlant, gridX, gridY )
  game.players[playerIndex].resources = game.players[playerIndex].resources - serverSettings.powerPlantCost
  return powerPlant
end

function PowerPlant.Create(playerIndex)
  local entity = EntityFactory:Create("POWERPLANT", playerIndex)
  return entity:GetLuaData()
end

EntityFactory:AddOnCreate("POWERPLANT", PowerPlant.OnCreate )

function PowerPlant:Main()

  local pulseTimer = 0
  local model = self.entity:GetFirstComponent("model")
  
  while(1) do
    -- If cannons/shields take care of their own recharge rate based on their "connections", we dont need to do anything here...
    
    
    pulseTimer = pulseTimer + .005 * Game:FrameElapsed()
      
    --model:SetGlowColor(color[1],color[2],color[3])
    model:SetGlowAmount( .8 + .4 * math.sin( pulseTimer ) )
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
end

function PowerPlant:HandleMessage(msgType, stack)

end
