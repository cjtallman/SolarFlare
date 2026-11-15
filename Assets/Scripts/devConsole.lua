-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--devConsole.lua

local function CheckTriggered( key, action, param )
  if Input:IsTriggered( key ) then
    Console[action]( Console, param )
  end
end

local function CheckRepeatable( key, action, param )
  if Input:IsRepeating( key ) then
    Console[action]( Console, param )
  end
end

function Main()
  local keys = InputMap
  local toggled = false
  local controllerLastState

  while 1 do
    if Input:IsTriggered( keys['~'] ) then
      Console:Toggle()
      toggled = not toggled
      
      if not toggled then
        controller.enabled = controllerLastState
        Console:PopLeft()   -- Removes ~ character
      else
        controllerLastState = controller.enabled
        controller.enabled = false
      end
    elseif toggled then
      CheckRepeatable( keys.Backspace,  "PopLeft" )
      CheckRepeatable( keys.Delete,     "PopRight" )
      CheckRepeatable( keys.ArrowRight, "MoveCursor", 1 )
      CheckRepeatable( keys.ArrowLeft,  "MoveCursor", -1 )
      CheckRepeatable( keys.PageUp,     "ScrollOutputUp" )
      CheckRepeatable( keys.PageDown,   "ScrollOutputDown" )
      
      CheckTriggered(  keys.Enter,        "Accept" )
      CheckTriggered(  keys.NumpadEnter,  "Accept" )
      CheckTriggered(  keys.ArrowUp,      "InputHistoryUp" )
      CheckTriggered(  keys.ArrowDown,    "InputHistoryDown" )
      CheckTriggered(  keys.Home,         "MoveCursor", -200 )
      CheckTriggered(  keys.End,          "MoveCursor", 200 )

      if Input:IsPressed( keys.LeftControl ) and Input:IsTriggered( keys.V ) then
        local text = GameLib:GetClipboardText()
        Console:Push( text )
      end
    end

    coroutine.yield( YIELD_FRAME, 1 )
  end -- while(1)
end -- Main
