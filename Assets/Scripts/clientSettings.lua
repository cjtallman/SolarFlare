-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--clientSettings.lua

clientSettingsSetters = { ['displayParticles'] = function(b) if b then GameLib.EnableParticles() else GameLib.DisableParticles() end end,
                          ['fullscreen'] = function(b) Game:FullscreenOn(b) end,
                          ['music'] = function(b) SoundEngine:MusicPaused( not b ) end,
                          ['postProcessing'] = function(b) GraphicsEngine:PostProcess(b) end,
                          ['soundFX'] = function(b) SoundEngine:SoundFXMuted( not b ) end,
                          ['playerName'] = function(str) Network:ClientName(str) end,
                          ['serverName'] = function(str) Network:ServerName(str) end,
                          ['masterVolume'] = function(num) SoundEngine:MasterVolume(num) end,
                          ['musicVolume'] = function(num) SoundEngine:MusicVolume(num) end,
                          ['soundFXVolume'] = function(num) SoundEngine:SoundFXVolume(num) end
                        }

clientSettings = {}

--
-- Various settings
--
clientSettings.cameraAssist         = true
clientSettings.constrainCameraRoll  = true
clientSettings.displayParticles     = true
clientSettings.fullscreen           = true
clientSettings.music                = true
clientSettings.postProcessing       = true
clientSettings.soundFX              = true
clientSettings.playerName           = 'Player'
clientSettings.serverName           = 'Server'
clientSettings.masterVolume         = 0.5
clientSettings.musicVolume          = 0.25
clientSettings.soundFXVolume        = 0.75

clientSettings.keyBinds = { }

-- 
-- Controls
--
clientSettings.keyBinds.placeCannon  = InputMap['1']
clientSettings.keyBinds.placeShield  = InputMap['2']
clientSettings.keyBinds.placePower   = InputMap['3']
clientSettings.keyBinds.cameraAssist = InputMap.LeftShift
clientSettings.keyBinds.fireMode     = InputMap.Spacebar
clientSettings.keyBinds.focusHome    = InputMap.Tab
clientSettings.keyBinds.zoomIn       = InputMap.MouseWheelUp
clientSettings.keyBinds.zoomOut      = InputMap.MouseWheelDown

-- Note: non-configurable at the moment
clientSettings.keyBinds.placeUnits   = InputMap.MouseLeft
clientSettings.keyBinds.rotateCamera = InputMap.MouseRight


--
-- Developer settings / controls
--
clientSettings.developerControls  = false
clientSettings.debugInfo          = false
clientSettings.rapidFire          = false

clientSettings.keyBinds.console      = InputMap['~']
clientSettings.keyBinds.cameraFire   = InputMap.RightControl
