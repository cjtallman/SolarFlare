-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.


ToolTip = ClassSimple(
  function( self, text, srcElement )
    self.panel = nil
    self.text = text
    self.srcElement = srcElement
  end )

  -- Show the tooltip, and hide any other tooltips
function ToolTip:Show()
  if menu.tooltip then
    menu.tooltip:Hide()
  end
  
  menu.tooltip = self
  
  if not self.srcElement.entity then
    self.srcElement = nil
    self:Hide()
    menu.tooltip = nil
    return
  end
  
  if not self.panel then
    self.panel = CreateElement("TOOLTIP")
    
    table.insert( menu.looseElements, self.panel )
    
    local gfx = self.panel.entity:GetFirstComponent( "ui_graphic" )
    gfx:SetHorizontalAlignment("CENTER")
    gfx:SetAnchor("BOTTOM")

    local srcGraphic = self.srcElement.entity:GetFirstComponent("ui_graphic")
    local srcTransform = self.srcElement.entity:GetFirstComponent("transform")
    
    local x, y, z = srcTransform:GetWorldPosition()
    
    self.panel:SetPosition(x, y - 64)
    self.panel:SetPadding(10,10,10,10)
    self.panel:SetText( self.text )
    
    self.panel:Fade(0,0.8,300)
    MenuSystem:Format()
  end
end

-- Destroy the tooltip panel and stuff
function ToolTip:Hide()
  if self.panel then
    self.panel:FadeOut()
    self.panel:Close()
    self.panel = nil
  end
end
