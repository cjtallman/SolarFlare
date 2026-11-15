-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--inputMap.lua

if not InputMap then
  
  InputMap = {}
  
  InputMap["Invalid"]   = 0
  InputMap["Backspace"] = 8
  InputMap["Tab"]       = 9
  InputMap["Clear"]     = 12
  InputMap["Return"]    = 13
  InputMap["Enter"]     = 13
  InputMap["Pause"]     = 19
  InputMap["Escape"]    = 27
  InputMap["Spacebar"]  = 32
  InputMap["'"]         = 34
  InputMap["-"]         = 45
  InputMap["."]         = 46
  InputMap["/"]         = 47
  InputMap["0"]         = 48
  InputMap["1"]         = 49
  InputMap["2"]         = 50
  InputMap["3"]         = 51
  InputMap["4"]         = 52
  InputMap["5"]         = 53
  InputMap["6"]         = 54
  InputMap["7"]         = 55
  InputMap["8"]         = 56
  InputMap["9"]         = 57
  InputMap[";"]         = 59
  InputMap["="]         = 61
  InputMap["A"]         = 65
  InputMap["B"]         = 66
  InputMap["C"]         = 67
  InputMap["D"]         = 68
  InputMap["E"]         = 69
  InputMap["F"]         = 70
  InputMap["G"]         = 71
  InputMap["H"]         = 72
  InputMap["I"]         = 73
  InputMap["J"]         = 74
  InputMap["K"]         = 75
  InputMap["L"]         = 76
  InputMap["M"]         = 77
  InputMap["N"]         = 78
  InputMap["O"]         = 79
  InputMap["P"]         = 80
  InputMap["Q"]         = 81
  InputMap["R"]         = 82
  InputMap["S"]         = 83
  InputMap["T"]         = 84
  InputMap["U"]         = 85
  InputMap["V"]         = 86
  InputMap["W"]         = 87
  InputMap["X"]         = 88
  InputMap["Y"]         = 89
  InputMap["Z"]         = 90
  InputMap["["]         = 91
  InputMap["\\"]        = 92
  InputMap["]"]         = 93
  InputMap["`"]         = 96
  InputMap["~"]         = 96
  
  InputMap["Numpad0"]     = 97
  InputMap["Numpad1"]     = 98
  InputMap["Numpad2"]     = 99
  InputMap["Numpad3"]     = 100
  InputMap["Numpad4"]     = 101
  InputMap["Numpad5"]     = 102
  InputMap["Numpad6"]     = 103
  InputMap["Numpad7"]     = 104
  InputMap["Numpad8"]     = 105
  InputMap["Numpad9"]     = 106
  InputMap["Numpad."]     = 107
  InputMap["Numpad/"]     = 108
  InputMap["Numpad*"]     = 109
  InputMap["Numpad-"]     = 110
  InputMap["Numpad+"]     = 111
  InputMap["NumpadEnter"] = 112
  
  InputMap["ArrowUp"] = 114
  InputMap["ArrowDown"] = 115
  InputMap["ArrowRight"] = 116
  InputMap["ArrowLeft"] = 117
  
  InputMap["Insert"] = 118
  InputMap["Home"] = 119
  InputMap["End"] = 120
  InputMap["PageUp"] = 121
  InputMap["PageDown"] = 122
  InputMap["Delete"] = 123
  
  InputMap["F1"] = 124
  InputMap["F2"] = 125
  InputMap["F3"] = 126
  InputMap["F4"] = 127
  InputMap["F5"] = 128
  InputMap["F6"] = 129
  InputMap["F7"] = 130
  InputMap["F8"] = 131
  InputMap["F9"] = 132
  InputMap["F10"] = 134
  InputMap["F11"] = 135
  InputMap["F12"] = 136
  InputMap["F13"] = 137
  InputMap["F14"] = 138
  InputMap["F15"] = 139
  
  InputMap["CapsLock"]      = 140
  InputMap["NumLock"]       = 141
  InputMap["ScrollLock"]    = 142
  InputMap["LeftShift"]     = 144
  InputMap["RightShift"]    = 145
  InputMap["LeftControl"]   = 146
  InputMap["RightControl"]  = 147
  InputMap["LeftAlt"]       = 148
  InputMap["RightAlt"]      = 149
  InputMap["LeftMeta"]      = 150
  InputMap["RightMeta"]     = 151

  InputMap["LeftWinKey"]    = 152   -- Left "Windows" key
  InputMap["RightWinKey"]   = 153   -- Right "Windows" key
  InputMap["Mode"]          = 154   -- "Alt Gr" key
  InputMap["Compose"]       = 155   -- Multi-key compose key
  InputMap["Help"]          = 156
  InputMap["Print"]         = 157 
  InputMap["PrintScreen"]   = 158
  InputMap["SysReq"]        = 159
  InputMap["Break"]         = 160
  InputMap["Menu"]          = 161
  
  InputMap["MouseLeft"]     = 162
  InputMap["MouseRight"]    = 163
  InputMap["MouseMiddle"]   = 164
  InputMap["MouseWheelUp"]  = 165
  InputMap["MouseWheelDown"] = 166


  function InputValToName( val )
    for k, v in pairs(InputMap) do
      if v == val then
        return k
      end
    end
    return ''
  end
  
end
