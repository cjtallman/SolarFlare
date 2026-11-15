-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--playerStats.lua

local dataMembers = { "your_towers_built",
                      "your_towers_destroyed_by_you",
                      "your_towers_destroyed_by_others",
                      "others_towers_destroyed_by_you",
                      "your_shots_fired",
                      "your_shots_that_hit_you",
                      "your_shots_that_hit_others",
                      "your_shots_that_missed",
                      "others_shots_that_hit_you",
                      "your_shots_that_hit_sun",
                      "your_shots_that_hit_asteroid",
                      "highest_multiplier",
                      "planets_destroyed"
                     }



PlayerStats = ClassSimple( function( self )
                             for i, v in ipairs( dataMembers ) do
                               self[v] = 0
                             end
                           end
                         )


function PlayerStats:SyncIn( msgQueue )
  for i, v in ipairs( dataMembers ) do
    self[v] = msgQueue:Pop()
  end
end

function PlayerStats:SyncOut( msgQueue )
  for i, v in ipairs( dataMembers ) do
    msgQueue:Push( self[v] )
  end
end

function IncreaseStat( playerID, statname )
  if playerID and game.players[playerID] then
    local stats = game.players[playerID].stats
    
    if stats[statname] then
      stats[statname] = stats[statname] + 1
    end
  end
end

function DecreaseStat( playerID, statname )
  if playerID and game.players[playerID] then
    local stats = game.players[playerID].stats
  
    if stats[statname] then
      stats[statname] = stats[statname] - 1
    end
  end
end
