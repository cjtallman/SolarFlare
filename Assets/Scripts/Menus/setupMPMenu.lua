-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--setupMPMenu.lua

local gameReady = true
local buttonReady = false
local gameLoading = false

local netStateStrings = { [NET_STATE_READY]     = '',
                          [NET_STATE_SYNCING]   = ' (Syncing)',
                          [NET_STATE_LOADING]   = ' (Loading)',
                          [NET_STATE_NOT_READY] = ' (Not Ready)'
                        }


SetupMPMenu = ClassSimple(
  function( self )
    gameReady = true
    buttonReady = false
    gameLoading = false
    
    self.root = nil
    self.peerRegion = nil
    self.button = nil
    
    PeersUpdate = nil
    StartFunction = nil
    peerCount = 0
  end
)

function SetupMPMenu:Create()
  ClearPlayers()
  
  if IsServer() then
    AddPeer( Network:ID() )
    game.players[1].netState = NET_STATE_READY
    
    Network:Settings( "AllowConnect", true )
  end
  
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment("CENTER", "MIDDLE")
    self.root:SetLayout("TopToBottom")
    
  self.root:CreateHeaderText( "Multiplayer Lobby" )
  
  self.peerRegion = self.root:CreateRegion()
    self.peerRegion:SetLayout("TopToBottom")
    self.peerRegion:SetAlignment("CENTER", "MIDDLE")
    self.peerRegion:SetMargin(0,0,50,0)
  
  self.button = self.root:CreateButton()
    self.button:SetCallback("OnClick", SendReadyMessage )
    self.button:SetText( "Connecting" )
  
  local backButton = self.root:CreateButton()
    backButton:SetText("Back")
    backButton:SetCallback("OnClick", PopGameState)
    backButton:SetMargin(40,0,0,0)
    
  self.root:FadeIn()
  MenuSystem:Format()
end

function SetupMPMenu:Delete()
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
end

function SetupMPMenu:Update()
  if not game.myIndex or not game.players[game.myIndex] then
    return
  end
  
  if gameLoading then
    -- Game is loading, all clients initially synced, server pressed 'start'
    -- Just waiting for all clients to get loaded up
    
    for i = 1, #game.players do
      if game.players[i].netState ~= NET_STATE_READY then
        return
      end
    end
    
    self.button:SetText( "Loading" )
    GameLib.AddMessage("global", "StartMultiplayerGame", 0, MSG_ALL )
  else
    
    while #self.peerRegion.children < #game.players do
      local playerText = self.peerRegion:CreateText()
      playerText:SetFontSize(32)
    end
    
    while #self.peerRegion.children > #game.players do
      self.peerRegion.children[#self.peerRegion.children]:Delete()
      table.remove( self.peerRegion.children )
    end
    
    for i = 1, #game.players do
      if game.players[i].AI then
        self.peerRegion.children[i]:SetText( "Computer" )
      else
        local c = GetPlayerColor( i )
        local state = game.players[i].netState
        
        local str = string.format( "%s%s", game.players[i].name, netStateStrings[state] )
        
        self.peerRegion.children[i]:SetText( str, Vec3(c[1],c[2],c[3]) )
      end
    end
    
    local buttonStr = ''
    local state = game.players[game.myIndex].netState
    
    if state == NET_STATE_READY then
      buttonStr = 'Not Ready'
    else
      buttonStr = 'Ready'
    end
    
    if IsServer() then
      buttonStr = "Waiting"
      
      -- Don't be 'ready' when no clients are connected
      gameReady = (#game.players > 1)
      
      for i = 1, #game.players do
        gameReady = gameReady and (game.players[i].netState == NET_STATE_READY)
      end
      
      if gameReady then
        buttonStr = "Start Game"
      end
    end

    self.button:SetText( buttonStr )
  end
  
  MenuSystem:Format()
end

function SendReadyMessage()
  if IsServer() then
    if gameReady and not gameLoading then
      -- Start at 2, don't change ourselves
      for i = 2, #game.players do
        game.players[i].netState = NET_STATE_LOADING
      end
      
      gameLoading = true
      CreateGameEntities()
      Network:Settings( "AllowConnect", false )
      GameLib.AddMessage("global", "GameStartSyncOut", 0, MSG_LOCAL )
    end
  else
    buttonReady = not buttonReady
    
    local state
    
    if buttonReady then
      state = NET_STATE_READY
    else
      state = NET_STATE_NOT_READY
    end
    
    game.players[game.myIndex].netState = state

    GameLib.AddMessage( "global", "SetNetState", 0, MSG_ALL_PEERS, game.myIndex, state )
  end
end

function StartMultiplayerGame()
  PopGameState( baseState )

  PushGameState( gamePlayState )
  PushGameState( gameIntroState )
end
