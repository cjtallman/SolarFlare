-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--scoreMenu.lua

ScoreMenu = ClassSimple( function( self )
                            self.root = nil
                            self.names = nil
                        end
                        )

local function NumberToCommaString( num )
  local str = tostring( num )
  local len = string.len( str )
  
  local i = len - 4
  while i >= 0 do
    str = string.sub( str, 1, i + 1 ) .. ',' .. string.sub( str, i + 2 )
    i = i - 3
  end
  
  return str
end

function ScoreMenu:Create()
    self.root = menu.root:CreatePanel()
    self.root:SetAlignment("LEFT", "MIDDLE")
    self.root:SetLayout("LeftToRight")
    self.root:SetPadding(5,10,5,10)
  
    self.names = self.root:CreateRegion()
    self.names:SetAlignment("LEFT", "MIDDLE")
    self.names:SetLayout("TopToBottom")
    self.names:SetMargin(0,20,0,0)

    for i = 1, #game.players do
      self.names:CreateText():SetText( game.players[i].name )
      self.names.children[i]:SetFontSize(30)
    end
    
    self:Update()
    
    self.root:SetAlpha(0.8)
end


function ScoreMenu:Update()
    --[[
    local score_data = {}
    for i = 1, #game.players do
        local player =  {
                            score = game.players[i].score.score,
                            name = game.players[i].name
                        }
        if not game.players[i].empty then
            table.insert( score_data, player )
        end
    end
    
    table.sort( score_data, function( A, B ) return A.score > B.score end )

    for i = 1, #score_data do
        self.names.children[i]:SetText( score_data[i].name )
        self.scores.children[i]:SetText( NumberToCommaString( score_data[i].score ) )
    end
    --]]
    
    
    for i = 1, #game.players do
      local name_string = game.players[i].name
      
      if game.players[i].planet.entity == nil then
        name_string = name_string .. " (dead)"
      end

      local c = GetPlayerColor(i)

      self.names.children[i]:SetText( name_string, Vec3(c[1],c[2],c[3]) )

    end
    
    MenuSystem:Format()
    
end

function ScoreMenu:Delete()
    if self.root then
        self.root:Delete()
        self.root = nil
        self.names = nil
    end
end

