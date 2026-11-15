-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--gameOptionsMenu.lua

GameOptionsMenu = ClassSimple(
  function( self )
    self.root = nil
    self.notifyText = nil
    
    self.bindMap = nil      -- keyBind that is being processed (clientSettings.keyBind[foo])
    self.bindButton = nil   -- actual UI button that needs updating
    self.doKeyBind = 0      -- 3 state (0 - no, 1 - reset, 2 - waiting on input)
  end
)

function UpdatePlayerName(str)
  if #str > 0 then
    local index = MyPlayerIndex()
    if game.players and game.players[index] then
      game.players[index].name = str
    end
    
    clientSettings.playerName = str
    Network:ClientName( str )
  end
end

function UpdateServerName(str)
  if #str > 0 then
    clientSettings.serverName = str
    Network:ServerName( str )
  end
end


function GameOptionsMenu:Create()
  self.root = menu.root:CreateRegion()
    self.root:SetAlignment( 'CENTER', 'MIDDLE' )
    self.root:SetLayout( 'TopToBottom' )
  
  self.root:CreateHeaderText( 'Game Options' )
  local keyBindText = self.root:CreateText( 'Key Bindings' )
    keyBindText:SetFontSize( 28 )
    keyBindText:SetMargin( 0, 0, 25, 0 )
  
  local keyBindRegion = self.root:CreateRegion()
    keyBindRegion:SetAlignment( 'CENTER' , 'MIDDLE' )
    keyBindRegion:SetLayout( 'LeftToRight' )
  
  local labelText = { 'Place Cannon', 'Place Shield', 'Place Power',
                      'Fire Mode',    'Focus Home',   'Camera Assist',
                      'Zoom In',      'Zoom Out' }

  local buttonText = { 'placeCannon', 'placeShield', 'placePower',
                       'fireMode',    'focusHome',   'cameraAssist',
                       'zoomIn',      'zoomOut' }
  
  local cols = { }
  
  for i = 1, 4 do
    cols[i] = keyBindRegion:CreateRegion()
    cols[i]:SetAlignment('LEFT','MIDDLE')
    cols[i]:SetLayout('TopToBottom')
  end
  
  cols[1]:SetPadding(0,25,0,0)
  cols[2]:SetPadding(0,50,0,0)
  cols[3]:SetPadding(0,25,0,0)
  cols[4]:SetPadding(0,0,0,0)
  
  for i = 1, 8 do
    local c1 = 1
    local c2 = 2
    
    if i > 4 then
      c1 = 3
      c2 = 4
    end
    
    local label = cols[c1]:CreateText( labelText[i] )
      label:SetFontSize(24)
      
    local inputName = InputValToName( clientSettings.keyBinds[buttonText[i]] )
    
    local button = cols[c2]:CreateButton( inputName )
      button:SetDimensions( 200, 28 )
      button:SetFontSize( 24 )
      button:SetCallback( 'OnClick', function() StartKeyBind(self,buttonText[i],button) end )
  end
  
  self.notifyText = self.root:CreateText( ' ' )
  self.notifyText:SetMargin( 25, 0, 25, 0 )
  
  local otherSetRegion = self.root:CreateRegion()
    otherSetRegion:SetAlignment( 'CENTER', 'MIDDLE' )
    otherSetRegion:SetLayout( 'TopToBottom' )
  
  local otherSettingText = otherSetRegion:CreateText( 'Other Settings' )
    otherSettingText:SetFontSize( 28 )
    otherSettingText:SetMargin( 0, 0, 25, 0 )
  
  local tempReg = otherSetRegion:CreateRegion()
    tempReg:SetAlignment( 'CENTER', 'MIDDLE' )
    tempReg:SetLayout( 'LeftToRight' )
    
  tempReg:CreateText( 'Player Name' ):SetMargin( 0, 10, 0, 0 )
  local tbBox = tempReg:CreateTextBox()
    tbBox:SetContent( clientSettings.playerName )
    tbBox:SetDimensions(240,24)
    tbBox.entity:GetFirstComponent("ui_element"):MaxLength(15)
    tbBox:SetCallback( 'OnKeyPress', UpdatePlayerName )
    tbBox:SetMargin( 0, 50, 0, 0 )
 
  tempReg:CreateText( 'Server Name' ):SetMargin( 0, 10, 0, 0 )
  tbBox = tempReg:CreateTextBox()
    tbBox:SetContent( clientSettings.serverName )
    tbBox:SetDimensions(240,24)
    tbBox.entity:GetFirstComponent("ui_element"):MaxLength(15)
    tbBox:SetCallback( 'OnKeyPress', UpdateServerName )
 
  local backButton = self.root:CreateButton()
    backButton:SetText('Back')
    backButton:SetCallback('OnClick', PopGameState )
    backButton:SetMargin(40,0,0,0)
  
  self.root:FadeIn()
  MenuSystem:Format()
end

function GameOptionsMenu:Update()
  if self.doKeyBind == 0 then
    return
  end
  
  if self.doKeyBind == 1 then
    self.doKeyBind = 2
    self.bindButton:SetText(' ')
    self.notifyText:SetText('Press Any Key To Rebind ' .. self.bindMap)
  elseif Input:IsTriggered( InputMap.Backspace ) then
    local name = InputValToName( clientSettings.keyBinds[self.bindMap] )
    self.bindButton:SetText( name )
    self:ResetBindState( 0 )
  elseif Input:IsAnyTriggered() then
    local val = Input:GetLastTriggered()
    
    if clientSettings.keyBinds[self.bindMap] ~= val then
      for k, v in pairs(clientSettings.keyBinds) do
        if v == val then
          self.notifyText:SetText( InputValToName(val) .. ' Already Bound To ' .. k )
          return
        end
      end
    end
    
    clientSettings.keyBinds[self.bindMap] = val
    self.bindButton:SetText( InputValToName(val) )
    
    self:ResetBindState( 0 )
  end
end

function GameOptionsMenu:Delete()
  if self.root then
    self.root:FadeOut()
    self.root:Close()
    self.root = nil
  end
end

function GameOptionsMenu:ResetBindState( doKeyBind, bindMap, bindButton, notifyStr )
  self.doKeyBind = doKeyBind
  self.bindMap = bindMap
  self.bindButton = bindButton
  self.notifyText:SetText( notifyStr or ' ' )
end

function StartKeyBind( menu, mapName, button )
  if menu.doKeyBind == 0 then
    menu:ResetBindState( 1, mapName, button )
  end
end
