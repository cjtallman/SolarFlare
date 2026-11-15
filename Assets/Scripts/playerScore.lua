-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--playerScore.lua

scoreValues = {}
scoreValues.sun = 100
scoreValues.asteroid = 200
scoreValues.enemy_planet = 500
scoreValues.enemy_tower = 1000
scoreValues.friendly_planet = -250
scoreValues.friendly_tower = -500

local dataMembers = { "score", "tempScore", "multiplier", "multiplierTimer", "multiplierActive" }
                    
PlayerScore = ClassSimple( function( self )  
                             self.score = 0
                             self.tempScore = 0
                             self.multiplier = 1
                             self.multiplierTimer = serverSettings.multiplierTimeout
                             self.multiplierActive = false
                           end
                          )

function PlayerScore:SyncIn( msgQueue )
  for i, v in ipairs( dataMembers ) do
    self[v] = msgQueue:Pop()
  end
end

function PlayerScore:SyncOut( msgQueue )
  for i, v in ipairs( dataMembers ) do
    msgQueue:Push( self[v] )
  end
end

function PlayerScore:StartMultiplier()
  self.multiplierTimer = serverSettings.multiplierTimeout
  self.multiplierActive = true
end


function PlayerScore:EndMultiplier()
  self.score = self.score + math.max(0, self.tempScore * self.multiplier)
  self.tempScore = 0
  self.multiplier = 1
  self.multiplierTimer = 0
  self.multiplierActive = false
end

function PlayerScore:UpdateScore()
  if self.multiplierActive then
    self.multiplierTimer = self.multiplierTimer - Game:FrameElapsed()
    
    if self.multiplierTimer < 0 then
      self:EndMultiplier()
    end
  end
end

function PlayerScore:AddScore( hit_value )
  self:StartMultiplier()
  self.tempScore = self.tempScore + hit_value
  self.multiplier = self.multiplier + 1
end



