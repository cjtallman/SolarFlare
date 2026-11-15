-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--game.lua

local msgHandlers = { }

function HandleMessage(msgType, stack)
  local func = msgHandlers[msgType]
  
  if func then
    func( unpack( stack ) )
  end
end

function ConnectSyncIn()
  cdebug( "ConnectSyncIn: myindex = ", tostring(game.myIndex) )
  
  -- Isn't needed, set to not ready by default
  --GameLib.AddMessage("global", "SetNetState", 0, MSG_ALL, game.myIndex, NET_STATE_NOT_READY )
end

function ConnectSyncOut(peerID)
  cdebug( "ConnectSyncOut: peerID = ", peerID )
  
  for i = 1, #game.players do
    GameLib.AddMessage("global", "SetNetState", 0, MSG_ALL, i, game.players[i].netState )
  end
  
  GameLib.AddMessage("global", "ConnectSyncIn", 50, MSG_SINGLE_PEER, peerID )
end

function GameStartSyncIn()
  cdebug( "GameStartSyncIn: myindex = ", game.myIndex )
  
  GameLib.AddMessage("global", "SetNetState", 0, MSG_ALL, game.myIndex, NET_STATE_READY )
end

function GameStartSyncOut()
  cdebug( "GameStartSyncOut" )

  for i = 1, #game.players do
    GameLib.AddMessage("global", "SyncEntity", 0, MSG_ALL_PEERS, game.players[i].planet.ID )
  end
  
  GameLib.AddMessage("global", "GameStartSyncIn", 0, MSG_ALL_PEERS )
end

function AddPeer(peerID)
  local index = #game.players + 1
  Player.Create(index, peerID, false)
  
  -- This is called when MP menu is created to add ourselves, so avoid that
  if peerID ~= Network:ID() then
    ConnectSyncOut(peerID)
  end
end

function AddAIPeer()
  local index = #game.players + 1

  local peerID = GeneratePeerID()
  Player.Create(index, peerID, true)
end

function RemoveAIPeer()
  local index = nil
  
  for i = 1, #game.players do
    if game.players[i].AI then
      index = i
      break
    end
  end
  
  if index then
    GameLib.AddMessage("global", "RemovePlayer", 0, MSG_ALL, index )
  end
end

function RemovePeer(peerID)
  local playerIndex = PlayerIndexFromPeerID( peerID )
  
  if playerIndex then
    cdebug( "RemovePeer: peerID = ", peerID, " playerIndex = ", playerIndex )
    GameLib.AddMessage("global", "RemovePlayer", 0, MSG_ALL, playerIndex )
  else
    cdebug( "RemovePeer: called with unknown peerID = ", peerID )
  end
end

function PrintEngineTimes(textEntity)
  local textComponent = textEntity:GetFirstComponent("text")
  local textTransform = textEntity:GetFirstComponent("transform")
  
  local sx, sy = GraphicsEngine:GetScreenSize()
  
  textTransform:SetPosition(125, 10, 0)
  
  local textStr = ""
  local engineTimes = Game:EngineTimes()
  
  local totalTime = 0
  
  for i, v in ipairs( engineTimes ) do
    totalTime = totalTime + v.Time
    textStr = string.format( "%s%s: %8.5f\n", textStr, v.Engine, v.Time )
  end
  
  textStr = string.format( "%s \nTotal: %8.5f\n", textStr, totalTime )
  textStr = string.format( "%sFPS: %d      ", textStr, GraphicsEngine:GetFPS() )
  
  textComponent:SetText(textStr)
end

tmpPeerIDs = {}

function PeerIDExists(id)
  for i = 1, #game.players do
    if game.players[i].peerID == id then
      return true
    end
  end
  
  for i = 1, #tmpPeerIDs do
    if tmpPeerIDs[i] == id then
      return true
    end
  end
  
  return false
end

function GeneratePeerID()
  local peerID = 1
  
  while PeerIDExists(peerID) do
    peerID  = peerID + 1
  end
  
  table.insert( tmpPeerIDs, peerID )
  
  return peerID
end

function MyPlanetID()
  local myIndex = MyPlayerIndex()
  
  if myIndex then
    return game.players[ myIndex ].planet.ID
  else
    return nil
  end
end

function MyPlayerIndex()
  if game.players[ game.myIndex ] then
    return game.myIndex
  else
    return nil
  end
end

function PlayerIndexFromPlanetID(planetID)
  for i = 1, #game.players do
    if planetID == game.players[i].planet.ID then
      return i
    end
  end
  
  return nil
end

function PlayerIndexFromPeerID(peerID)
  for i = 1, #game.players do
    if peerID == game.players[i].peerID then
      return i
    end
  end
  
  return nil
end

function PlayerIndexFromCannonID(cannonID)
  for i = 1, #game.players do
    for j = 1, #game.players[i].planet.cannons do
      if game.players[i].planet.cannons[j].ID == cannonID then
        return i
      end
    end
  end
  
  return nil
end

function SetNetState(playerIndex,state)
  cdebug( "SetNetState: ", playerIndex, " = ", state )
  game.players[playerIndex].netState = state
end

-- Anything needed to be done on game exit
function OnGameShutdown()
  if game.sun then
    local sunComp = game.sun:GetFirstComponent( "sun" )
    sunComp:StopAnimation()
  end
end


function Main()
  local myID = ScriptEngine:CurUpdateID()
  
  --
  -- Using explicit ['string'] constructor to avoid confusion between string 
  -- indicies and functions with the same name. It works by default, but appears
  -- strange (plus you can index with functions so it could be mildly confusing)
  --
  msgHandlers = { ['DeletePlanet']          = DeletePlanet,
                  ['EndGameSupernova']      = function() PushGameState(supernovaState) end,
                  ['EndGame']               = function() PushGameState(endGameState) end,
                  ['ConnectSyncIn']         = ConnectSyncIn,
                  --['ConnectSyncOut']        = ConnectSyncOut,    --Handled by AddPer() function
                  ['GameStartSyncIn']       = GameStartSyncIn,
                  ['GameStartSyncOut']      = GameStartSyncOut,
                  ['GameShutdown']          = OnGameShutdown,
                  ['PeerAdded']             = AddPeer,
                  ['PeerRemoved']           = RemovePeer,
                  ['PlanetDeath']           = PlanetDeath,
                  ['RemovePlayer']          = RemovePlayer,
                  ['RemoveProjectile']      = RemoveProjectile,
                  ['RemoveShield']          = RemoveShield,
                  ['RemoveStructure']       = function(playerIndex, structureID) game.players[playerIndex].planet:RemoveStructure(structureID) end,
                  ['ResetPlayerSlot']       = ResetPlayerSlot,
                  ['SetNetState']           = SetNetState,
                  ['SetPlayerSlot']         = SetPlayerSlot,
                  ['StartMultiplayerGame']  = StartMultiplayerGame,
                  ['SunCreated']            = function(id) game.sun = EntityFactory:GetEntity( id ) end,
                  ['SyncAllSettings']       = function(...) serverSettings:SyncIn(arg) end,
                  ['SyncSingleSetting']     = function(name,value) serverSettings:SyncInField(name,value) end,
                  ['SyncShield']            = SyncShield
                }
  

  for k,v in pairs(msgHandlers) do
    GameLib.AddMessageSubscription( k, myID )
  end
  
  --TODO: Cleanup on isle 7! (aka game.lua, the game table, etc)
  game.crosshair = Crosshair()

  LoadPersistentSettings()

  EntityFactory:GetEntity(myID):GetFirstComponent("sound"):Play()

  local textEntity = EntityFactory:Create("TEXT")
  local textComponent = textEntity:GetFirstComponent("text")
  
  textComponent:SetFontSize(14)
  textComponent:SetHorizontalAlignment("RIGHT")
  
  local frame = 20
   
  while 1 do
    if clientSettings.debugInfo then
      if frame == 20 then 
        frame = 0
        PrintEngineTimes(textEntity)
      else
        frame = frame + 1
      end
    else
      textComponent:SetText("")
    end
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
end
