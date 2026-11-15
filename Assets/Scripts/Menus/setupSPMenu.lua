-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--setupSPMenu.lua

SinglePlayerMenu = ClassSimple(
  function( self )
    self.root = nil
  end
)

function SinglePlayerMenu:Create()
  Network:Mode("Idle")
  
  serverSettings.roundDuration = 10 * 60 * 1000
  serverSettings.resourceRate = 100 / 60 / 1000 + .0005
  serverSettings.firingCooldown = 9000
  
  ClearPlayers()
  AddPeer(Network:ID())
  
  game.AIPlayerCount = 0
  SetAIPlayerCount( 7 )
  
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment("CENTER", "MIDDLE")
    self.root:SetLayout("TopToBottom")

  self.root:CreateHeaderText("Single Player")
  
  local settingsRegion = self.root:CreateRegion()
    settingsRegion:SetLayout("LeftToRight")

  local AISliderRegion = settingsRegion:CreateRegion()
    AISliderRegion:SetAlignment("CENTER", "MIDDLE")
    AISliderRegion:SetLayout("TopToBottom")
    AISliderRegion:SetPadding(0,15,0,15)
  
  local AIText = AISliderRegion:CreateText("AI Players")
    AIText:SetFontSize(24)
    AIText:SetMargin(0,0,0,0)
    
  local AISlider = AISliderRegion:CreateSlider()
    AISlider:SetDimensions(128,64)
    AISlider:SetRange(1,7)
    AISlider:SetValue(game.AIPlayerCount)
    AISlider:SetMargin(0, 20, 0, 0)
    
  local countText = AISliderRegion:CreateText()
    countText:SetText(game.AIPlayerCount)
    countText:SetFontSize(24)

    AISlider:SetCallback("OnClick", UpdateAIPlayerCount(countText))
  
  local timeSliderRegion = settingsRegion:CreateRegion()
    timeSliderRegion:SetAlignment("CENTER", "MIDDLE")
    timeSliderRegion:SetLayout("TopToBottom")  
    timeSliderRegion:SetPadding(0,15,0,15)

  local timeText = timeSliderRegion:CreateText("Time Limit")
    timeText:SetFontSize( 24 )
    timeText:SetMargin(0,0,0,0)

  local timeSlider = timeSliderRegion:CreateSlider()
    timeSlider:SetDimensions(128,64)
    timeSlider:SetRange(4 * 60 * 1000, 15 * 60 * 1000)
    timeSlider:SetValue(serverSettings.roundDuration)
    
  local timeText = timeSliderRegion:CreateText()
    timeText:SetText("10:00")
    timeText:SetFontSize(24)
      
    timeSlider:SetCallback("OnClick", UpdateTimeLimit(timeText))

      
  local startButton = self.root:CreateButton()
    startButton:SetMargin(40,0,0,0)
    startButton:SetText("Start")
    startButton:SetCallback("OnClick", StartSinglePlayerGame)
    
  local backButton = self.root:CreateButton()
    backButton:SetText("Back")
    backButton:SetCallback("OnClick", PopGameState )

  self.root:FadeIn()
end

function SinglePlayerMenu:Delete()
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
end

function StartSinglePlayerGame()
  PopGameState( baseState )

  PushGameState( gamePlayState )
  PushGameState( gameIntroState )
end

function UpdateAIPlayerCount(textElement)
  return function( aiCount )
    aiCount = math.floor( (aiCount) + 0.5 )

    SetAIPlayerCount(aiCount)

    textElement:SetText(aiCount)
  end
end

function UpdateTimeLimit(textElement)
  return function( timeLimit )
    serverSettings.roundDuration = timeLimit
    
    local minTime = 4 * 60 * 1000
    local maxTime = 15 * 60 * 1000
    local baseRate = 100 / 60 / 1000
        
    local t = (maxTime - timeLimit) / (maxTime - minTime)    
    serverSettings.resourceRate = baseRate + t / 1000
    serverSettings.firingCooldown = 12000 - 6000 * t
    
    local minutes = serverSettings.roundDuration / 1000 / 60
    local seconds = serverSettings.roundDuration / 1000 % 60
    
    textElement:SetText( string.format("%i:%02i", minutes, seconds) )
  end
end

function SetAIPlayerCount(aiCount)
  aiCount = math.floor( (aiCount) + 0.5 )

  while game.AIPlayerCount > aiCount do
    RemoveAIPeer()
    game.AIPlayerCount = game.AIPlayerCount - 1
  end
  
  while game.AIPlayerCount < aiCount do
    AddAIPeer()
    game.AIPlayerCount = game.AIPlayerCount + 1
  end
end
