-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--timer.lua

function InitFunctions( self )
  if self.countdown then
    self.time         = self.duration
    self.Count        = function(self) self.time = self.time - Game:FrameElapsed() end
    self.IsFinished   = function(self) return self.time < 0 end
    self.Reset        = function(self) self.time = self.duration end
    self.TimeLeft     = function(self) return self.time end
    self.TimeElapsed  = function(self) return self.duration - self.time end
  else
    self.time         = 0
    self.Count        = function(self) self.time = self.time + Game:FrameElapsed() end
    self.IsFinished   = function(self) return self.time > self.duration end
    self.Reset        = function(self) self.time = 0 end
    self.TimeLeft     = function(self) return self.duration - self.time end
    self.TimeElapsed  = function(self) return self.time end
  end
end

Timer = ClassSimple( function( self, duration, repeating, countdown )
                        self.duration = duration or 1000
                        self.repeating = repeating or false
                        self.countdown = countdown or true
                        self.paused = true
                        self:InitFunctions()
                      end
                   )

local dataMembers = { "time", "duration", "repeating", "countdown", "paused" }

function Timer:SyncIn( msgQueue )
  for i, v in ipairs( dataMembers ) do
    self[v] = msgQueue:Pop()
  end
  
  self:InitFunctions()
end

function Timer:SyncOut( msgQueue )
  for i, v in ipairs( dataMembers ) do
    msgQueue:Push( self[v] )
  end
end

function Timer:Update()
  self:Count()
  
  if self:IsFinished() then
    if self:IsRepeating() and not self:IsPaused() then
      self:Reset()
    end
    return false
  else
    return true
  end
end

function Timer:IsCountdown()
  return self.countdown
end

function Timer:Pause()
  self.paused = true
end

function Timer:Resume()
  self.paused = false
end

function Timer:IsPaused()
  return self.paused
end

function Timer:IsRepeating()
  return self.repeating
end

function Timer:Start()
  self.paused = false
end

function Timer:Stop()
  self.paused = true
  self:Reset()
end
