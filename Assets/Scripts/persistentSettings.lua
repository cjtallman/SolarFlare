-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--persistentSettings.lua

local function GetPersistentSettingsPath()
  return GameLib.GetUserPath() .. "/.SolarFlare"
end

local function GetPersistentSettingsFilename()
  return GetPersistentSettingsPath() .. "/settings.lua"
end

local function TypeConvertError(typeName)
  cwarning( 'Error converting to type: ', typeName )
  return nil
end

local loadFuncs = { ['number']  = tonumber,
                    ['string']  = tostring,
                    ['boolean'] = function(str) return str == 'true' end
                  }

local saveFuncs = { ['number']  = function(val) return tostring(val) end,
                    ['string']  = function(val) return val end,
                    ['boolean'] = function(val) return tostring(val) end
                  }

local ignoreFields = { [clientSettings.keyBinds] = true,
                       [serverSettings.SyncIn] = true,
                       [serverSettings.SyncInField] = true,
                       [serverSettings.SyncOut] = true,
                       [serverSettings.Set] = true
                     }

local function SerializeTable( file, tbl )
  for k, v in pairs(tbl) do
    if not ignoreFields[v] then
      local func = saveFuncs[type(v)]
      if func then
        file:write( k, ' = ', func(v), '\n' )
      else
        cwarning( '(SaveSetting) Skipping field: ', k )
      end
    end
  end
end


function LoadPersistentSettings()
  local file, errorVal = io.open( GetPersistentSettingsFilename(), 'r' )
  
  if not file then
    cwarning( '(LoadSetting) Error: ', errorVal )
  else
    local tbl = nil
    local tblMap = { ['cs'] = clientSettings,
                     ['kb'] = clientSettings.keyBinds,
                     ['ss'] = serverSettings
                   }

    for line in file:lines() do
      if not (string.byte(line,1) == 45 and string.byte(line,2) == 45) then
        local it = string.gmatch( line, '(%w+) = (.+)' )
        local k, v = it()

        if k == 'tbl' then
          tbl = tblMap[v]
        else
          if tbl then
            if not k and not v then
              if #line ~= 0 then
                cwarning( '(LoadSetting) Invalid line: ', line )
              end
            elseif k and not v then
              cwarning( '(LoadSetting) Key read with no value: ', k )
            else
              local element = tbl[k]
              
              if element ~= nil then
                local tn = type( element )
                local func = loadFuncs[tn]
                
                if func then
                  tbl[k] = func( v )
                else
                  cwarning( '(LoadSetting) No load func for: ', k, ' (type: ', tn, ')' )
                end
              else
                cwarning( '(LoadSetting) Key not in specified table: ', k )
              end
            end
          else
            cwarning( '(LoadSetting) Key read with no tbl set: ', k )
          end
        end
      end
    end
    
    file:close()
  end
  
  for k,v in pairs(clientSettingsSetters) do
    v( clientSettings[k] )
  end
  
  --
  -- Slight hack, music volume has to be 0 for 3D sounds to obey mute
  -- make sure enable/disable soundFX is done last (after volume set)
  --
  if( clientSettingsSetters['soundFX'] ) then
    clientSettingsSetters['soundFX']( clientSettings['soundFX'] )
  end
end

function SavePersistentSettings()
  local file = nil
  local errorVal = nil
  
  if( GameLib.CreateFolder( GetPersistentSettingsPath() ) ) then
    file, errorVal = io.open( GetPersistentSettingsFilename(), 'w+' )
  end
  
  if not file then
    cwarning( '(SaveSetting) Error: ', errorVal )
    return
  end
  
  file:write( '--\n-- Client Settings\n--\n' )
  file:write( 'tbl = cs\n' )
  SerializeTable( file, clientSettings )
  
  file:write( '\n--\n-- Key Bindings \n--\n' )
  file:write( 'tbl = kb\n' )
  SerializeTable( file, clientSettings.keyBinds )
  
  file:write( '\n--\n-- Server Settings\n--\n' )
  file:write( 'tbl = ss\n' )
  SerializeTable( file, serverSettings )
  
  file:close()
end
