-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

local gameState = {}
local popped = false

function CurrentGameState()
  return gameState[ #gameState ]
end

function PushGameState(state)
  table.insert(gameState, state)
  
  if gameState[#gameState].Enter then
    gameState[#gameState].Enter()
  end
end

function PopGameState()
  if gameState[#gameState].Leave then
    gameState[#gameState].Leave()
  end

  table.remove(gameState)
  
  popped = true
end

function GameStateCount()
  return #gameState
end

function DoGameStateUpdates(stateIndex)
  local updateIndex = 1

  if popped then
    return
  end

  if not gameState[ stateIndex ].Updates then
    return
  end

  while #gameState > 0 and updateIndex <= #gameState[ stateIndex ].Updates do
    gameState[ stateIndex ].Updates[ updateIndex ]()
    
    if popped then
      break
    end
    
    updateIndex = updateIndex + 1
  end
end

function DoGameStateGlobalUpdates(stateIndex)
  local updateIndex = 1

  if popped then
    return
  end
  
  if not gameState[ stateIndex ].GlobalUpdates then
    return
  end

  while #gameState > 0 and updateIndex <= #gameState[ stateIndex ].GlobalUpdates do
    gameState[ stateIndex ].GlobalUpdates[ updateIndex ]()
    
    if popped then
      break
    end
    
    updateIndex = updateIndex + 1
  end
end

function Main()
  PushGameState( baseState )
  PushGameState( introState )

  while 1 do
    -- Global Updates
    for i = 1, GameStateCount(), 1 do
      DoGameStateGlobalUpdates(i)
    end
    
    DoGameStateUpdates(#gameState)
    
    popped = false
    
    coroutine.yield( YIELD_FRAME, 1 )
  end
  
end
