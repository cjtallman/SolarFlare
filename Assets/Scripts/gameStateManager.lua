-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--gameStateManager.lua

--TODO: Make all callback types a table (OnEnter, OnLeave, etc.)
GameState = ClassSimple( function( self )
                           self.OnEnter = nil
                           self.OnLeave = nil
                           
                           self.OnPush = nil
                           self.OnPop = nil
                           
                           self.GlobalUpdates = {}
                           self.Updates = {}
                         end
                       )

function GameState:AddCallback(cbtype, callback)
  if type(self[cbtype]) == "table" then
    table.insert( self[cbtype], callback )
  end
end

GameStateManager = ClassSimple( function( self )
                                  self.stateStack = Stack()
                                  self.stateChange = false
                                end
                              )

function GameStateManager:Push(gameState)
  if not self.stateStack:Empty() then
    local top = self.stateStack:Top()
    
    if top.OnLeave then
      top.OnLeave()
    end
  end
  
  self.stateStack:Push( gameState )
  
  if gameState.OnEnter then  
    gameState.OnEnter()
  end
  
  if gameState.OnPush then
    gameState.OnPush()
  end
  
  self.stateChange = true
end

function GameStateManager:Pop(toState)
  -- Note: If toState is _not_ in the gameState stack, self.stateStack:Top()
  --       will error on empty. So ya, don't do that.
  repeat
    local top = self.stateStack:Pop()
    
    if top.OnLeave then
      top.OnLeave()
    end
  
    if top.OnPop then
      top.OnPop()
    end
  until( toState == nil  or self.stateStack:Top() == toState )

  if not self.stateStack:Empty() then
    local top = self.stateStack:Top()
    
    if top.OnEnter then
      top.OnEnter()
    end
  end
  
  self.stateChange = true
end

function GameStateManager:Top()
  return self.stateStack:Top()
end

function GameStateManager:Count()
  return self.stateStack:Size()
end

function GameStateManager:Update()
  self:DoGlobalUpdates()
  self:DoLocalUpdates()
  self.stateChange = false
end

function GameStateManager:DoGlobalUpdates()
  for i = 1, self.stateStack:Size() do
    if self.stateChange then return end
    
    local state = self.stateStack:Get(i)
    
    for j = 1, #state.GlobalUpdates do
      if self.stateChange then return end
      
      state.GlobalUpdates[j]()
    end
  end
end

function GameStateManager:DoLocalUpdates()
  local state = self.stateStack:Top()
  
  for i = 1, #state.Updates do
    if self.stateChange then return end
    
    state.Updates[i]()
  end
end

function CurrentGameState()
  return gameStateManager:Top()
end

function PushGameState(state)
  gameStateManager:Push(state)
end

function PopGameState(toState)
  gameStateManager:Pop(toState)
end

function GameStateCount()
  return gameStateManager:Count()
end

function Main()
  gameStateManager = GameStateManager()
  
  PushGameState( baseState )
  
  PushGameState( splashTeam )               -- Start like normal
  --PushGameState( MenuState( MainMenu() ) )  -- Start at main menu
  --PushGameState( gameplayState )            -- Start in singleplayer game
  --PushGameState( prototypeGameState )       -- Start in prototype game

  while 1 do
    gameStateManager:Update()
    coroutine.yield( YIELD_FRAME, 1 )
  end
  
end

function InitGameStates()
  InitAINames()
  InitBaseState()
  InitEndGameState()
  InitGameplayState()
  InitGameIntroState()
  InitGameOutroState()
  InitIntroState()
  --InitPrototypeGameState()
  InitTowerPlacementState()
  InitSupernovaState()
end
