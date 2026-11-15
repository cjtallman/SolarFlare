-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--player.lua

local playerColors = 
{
  {255,40,20},    -- Red
  {45,255,0},     -- Green
  {255,255,20},   -- Yellow
  {255,120,10},   -- Orange
  {200,200,200},   -- White
  {10,255,150},   -- Teal
  {255,80,140},    -- Magenta
  {10,60,255}    -- Blue
}


function GetPlayerColor( playerIndex )
  return playerColors[playerIndex]
end

local function PlayerConstructor( self, entityID, playerIndex, peerID, isAI )
  self.AI = isAI
  self.color = GetPlayerColor( playerIndex )
  self.entity = EntityFactory:GetEntity( entityID )
  self.index = playerIndex
  self.name = "Player"
  self.netState = NET_STATE_NOT_READY
  self.peerID = peerID
  self.resources = serverSettings.resourceStart
  self.stats = PlayerStats()
  --self.score = PlayerScore()
  
  local script = self.entity:GetFirstComponent("script")
  
  script:SetUpdate( function() self:Main() end )
  --script:SetMessageHandler( function() self:HandleMessage() end )
end

Player = ClassSimple( PlayerConstructor )

function Player.OnCreate(entity, playerIndex, peerID, isAI)
  local player = Player( entity:ID(), playerIndex, peerID, isAI )
  
  game.players[playerIndex] = player
  
  if not isAI then
    game.players[playerIndex].name = Network:PeerName( peerID )
  else
    game.players[playerIndex].name = GetAIName()
    
    if IsServer() then
      player.AIController = EntityFactory:Create("AICONTROLLER", playerIndex)
    end
  end

  if peerID == Network:ID() then
    game.myIndex = playerIndex
  end
  
  return player
end

function Player.Create(playerIndex, peerID, isAI)
  local playerEntity = EntityFactory:CreateEntity("PLAYER", playerIndex, peerID, isAI)

  local planetEntity = EntityFactory:CreateEntity("PLANET", playerIndex)
  
  return player
end

EntityFactory:AddOnCreate("PLAYER", Player.OnCreate)

function Player:Main()
  while 1 do
    while( GameLib.PhysicsPaused() ) do
      coroutine.yield( YIELD_FRAME, 1 )
    end
    
    self.resources = self.resources + serverSettings.resourceRate * Game:FrameElapsed()
    coroutine.yield( YIELD_FRAME, 1 )
  end
end

function Player:HandleMessage( msgType, msgStack )
end

function Player:Delete()
  if self.AIController then
    EntityFactory:DeleteEntity( self.AIController:ID() )
  end

  EntityFactory:DeleteEntity( self.entity:ID() )
  
  self.planet:Delete()
end

function Player:SetIndex(index)
  self.index = index
  self.planet.playerIndex = index
  
  for kA, vA in pairs(self.planet.structures) do
    for kB, vB in pairs(self.planet.structures[kA]) do
      vB.playerIndex = index
    end
  end
end

function ClearPlayers()
  for i, v in ipairs( game.players ) do
    v:Delete();
    
    if v.AI then
      AddAIName( v.name )
    end
  end
  
  game.players = { }
end


function RemovePlayer(index)  
  if game.players[index].AI then
    AddAIName( game.players[index].name )
  end

  game.players[index]:Delete()
  table.remove(game.players, index)
  
  for i = index, #game.players do
    game.players[i]:SetIndex(i)
  end
end
