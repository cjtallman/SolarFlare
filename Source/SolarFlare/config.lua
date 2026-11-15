-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

-- config.lua

local assetDir  = "../../Assets/"
local scriptDir = assetDir .. "Scripts/"

dofile( scriptDir .. "inputMap.lua" )
dofile( scriptDir .. "serverSettings.lua" )
dofile( scriptDir .. "clientSettings.lua" )
dofile( scriptDir .. "helperFunctions.lua" )
dofile( scriptDir .. "classFunctions.lua" )
dofile( scriptDir .. "hacks.lua" )

GameLib.SetAssetDir( assetDir )
GameLib.LoadAssets( assetDir .. "XML/assets.xml" )
GameLib.LoadEntityDefs( assetDir .. "XML/entityDefs.xml" )

EntityFactory:Create( "DEV_CONSOLE" )
EntityFactory:Create( "GAMESTATE_MANAGER" )
EntityFactory:Create( "GAME" )

InitMenus()
InitGameStates()
