-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--audioMenu.lua

AudioMenu = ClassSimple(
  function( self )
    self.root = nil
  end
)

function UpdateMasterVolume(val)
  clientSettings.masterVolume = val
  SoundEngine:MasterVolume(val)
end

function UpdateMusicVolume(val)
  clientSettings.musicVolume = val
  SoundEngine:MusicVolume(val)
end

function UpdateSoundFXVolume(val)
  clientSettings.soundFXVolume = val
  SoundEngine:SoundFXVolume(val)
end

function AudioMenu:Create()
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment("CENTER", "MIDDLE")
    self.root:SetLayout("TopToBottom")
  
  local header = self.root:CreateHeaderText("Audio Options")

  self.root:CreateText("Master Volume")
  local masterSlider = self.root:CreateSlider()
                       masterSlider:SetDimensions(128,64)
                       masterSlider:SetRange(0,1)
                       masterSlider:SetValue(SoundEngine:MasterVolume())
                       masterSlider:SetCallback("OnClick", UpdateMasterVolume)
  
  self.root:CreateText("Music Volume")                     
  local musicSlider = self.root:CreateSlider()
                      musicSlider:SetDimensions(128,64)
                      musicSlider:SetRange(0,1)
                      musicSlider:SetValue(SoundEngine:MusicVolume())
                      musicSlider:SetCallback("OnClick", UpdateMusicVolume)
                       
  self.root:CreateText("SoundFX Volume")
  local soundFXSlider = self.root:CreateSlider()
                       soundFXSlider:SetDimensions(128,64)
                       soundFXSlider:SetRange(0,1)
                       soundFXSlider:SetValue(SoundEngine:SoundFXVolume())
                       soundFXSlider:SetCallback("OnClick", UpdateSoundFXVolume)
  
  local musicButton = self.root:CreateButton()
    musicButton:SetTextBoolean( clientSettings.music, "Music On", "Music Off" )
    musicButton:SetCallback("OnClick", MenuToggleMusic(musicButton))
  
  local sfxButton = self.root:CreateButton()
    sfxButton:SetTextBoolean( clientSettings.soundFX, "SoundFX On", "SoundFX Off" )
    sfxButton:SetCallback("OnClick", MenuToggleSoundFX(sfxButton))
    
  local backButton = self.root:CreateButton()
    backButton:SetText("Back")
    backButton:SetCallback("OnClick", PopGameState )
    backButton:SetMargin(40,0,0,0)
  
  self.root:FadeIn()
  MenuSystem:Format()
end

function AudioMenu:Delete()
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
end

function MenuToggleMusic(button)
  return  function()
            clientSettings.music = not clientSettings.music
            SoundEngine:MusicPaused( not clientSettings.music )
            
            button:SetTextBoolean( clientSettings.music, "Music On", "Music Off" )
          end
end

function MenuToggleSoundFX(button)
  return  function()
            clientSettings.soundFX = not clientSettings.soundFX
            SoundEngine:SoundFXMuted( not clientSettings.soundFX )
            
            button:SetTextBoolean( clientSettings.soundFX, "SoundFX On", "SoundFX Off" )
          end
end
