-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--menuElement.lua
--TODO: Clean this up a bit
--      Make an inheritance tree of different menu element types
--      Start using the Base() function wherever relevant.

local defaultFont     = "stentiga"
local defaultFontSize = 20

MenuElement = ClassSimple( function( self )
                             self.children = {}
                             self.entity = nil
                             self.deleteTimer = 0
                             self.disabled = false
                             self.ID = nil
                             self.messageHandler = nil
                             self.parent = nil
                             self.updateFunction = nil
                           end )

function CreateElement(elementType)
  local element = MenuElement()
  
  element.entity = EntityFactory:Create(elementType)
  element.ID = element.entity:ID()
  
  local script  = element.entity:GetFirstComponent("script")
  
  if script then
    script:SetMessageHandler( function(type, stack)
                                element:HandleMessage(type, stack)
                              end )
  end

  return element
end

function MenuElement:CreateChildElement(elementType)
  MenuSystem:SetActiveElement( self.ID )
  
  local element = CreateElement(elementType)
  
  table.insert( self.children, element )
  element.parent = self
  
  MenuSystem:ResetActiveElement()
  
  return element
end

function MenuElement:CreateButton(string)
  local button = self:CreateChildElement("MENUBUTTON")
  
  button:SetDimensions(256*1.25, 64*1.25)
  button:SetGraphic("menu_up", "UP")
  button:SetGraphic("menu_over", "OVER")
  button:SetMessageHandler( ButtonMessageHandler )
  
  if string then
    button:SetText(string)
  end
  
  return button
end

function MenuElement:CreateHudButton()
  local button = self:CreateChildElement("HUD_BUTTON")
  button:SetDimensions(64, 64)
  button:SetGraphic("menu_up", "UP")
  button:SetGraphic("menu_over", "OVER")
  button:SetMessageHandler( ButtonMessageHandler )
  return button
end

function MenuElement:CreateImage()
  return self:CreateChildElement("MENUIMAGE")
end

function MenuElement:CreateHeaderText(string)
  local header = self:CreateText()
    header:SetText(string)
    header:SetFontSize(40)
    header:SetMargin(0,0,65,0)
  
  return header
end

function MenuElement:CreateText(string)
  local text = self:CreateChildElement("MENUTEXT")
  
  local textCmp = text.entity:GetFirstComponent( "text" )
  textCmp:SetFont(defaultFont)
  
  textCmp:SetHorizontalAlignment("CENTER")
  textCmp:SetVerticalAlignment("MIDDLE")
  --textCmp:SetColor(255,255,0,255,0,255,255,255)
  
  if string then
    text:SetText(string)
  end
  
  return text
end

function MenuElement:CreateTooltip()
  local text = self:CreateChildElement("TOOLTIP")
  
  local gfx = text.entity:GetFirstComponent( "ui_graphic" )
  gfx:SetFont(defaultFont)
  
  gfx:SetHorizontalAlignment("CENTER")
  gfx:SetVerticalAlignment("MIDDLE")
  self:SetPadding(20,20,20,20)
  --textCmp:SetColor(255,255,0,255,0,255,255,255)
  
  return text
end


function MenuElement:CreateTextBox()
  local input = self:CreateChildElement( 'MENUINPUTREGION' )
    input:SetDimensions(128,24)
    input:SetAlignment( 'LEFT', 'MIDDLE' )
    input:SetLayout( 'LeftToRight' )
    input:SetPadding( 0, 3, 0, 3 )
    input:SetGraphic("menu_up", "UP")
    input:SetGraphic("menu_over", "OVER")
  
  local text = input:CreateChildElement( 'MENUTEXT' )
    local textCmp = text.entity:GetFirstComponent( 'text' )
      textCmp:SetHorizontalAlignment( 'LEFT' )
      textCmp:SetVerticalAlignment( 'MIDDLE' )
      textCmp:SetFontSize( defaultFontSize )
  
  input.entity:GetFirstComponent("ui_element"):TextComponent( textCmp )
  
  return input
end

function MenuElement:CreatePanel()
  return self:CreateChildElement("MENUPANEL")
end

function MenuElement:CreateRegion()
  return self:CreateChildElement("MENUREGION")
end

function MenuElement:CreateSlider()
  local slider = self:CreateChildElement("MENUSLIDER")

  local barGraphic = slider.entity:GetComponentByName( "ui_graphic", "barGraphic" )
  local sliderGraphic = slider.entity:GetComponentByName( "ui_graphic", "sliderGraphic" )
    
  -- Ghetto method overriding, for now...
  slider.SetDimensions = slider.SetSliderDimensions
  slider.SetGraphicState = slider.SetGraphicStateSlider
  
  slider:SetDimensions(256, 64)
  slider:SetMessageHandler( ButtonMessageHandler )
  

  if barGraphic then
    barGraphic:SetTexture("UP","sliderbar")
    barGraphic:SetTexture("OVER","sliderbar")
  end
  
  if sliderGraphic then
    sliderGraphic:SetTexture("UP","sliderbar_knob")
    sliderGraphic:SetTexture("OVER","sliderbar_knob_over")
  end
  
  return slider
end

function MenuElement:Delete(time)
  if time then
    self.deleteTimer = time
  else
    for i = 1, #self.children do
      self.children[i]:Delete()
    end
    
    -- TODO: Remove from parent's child list
    -- SOLUTION?: We delete the entity and set entity to nil, effectively marking it "deleted".
    --            The calling function will have to iterate back over and remove deleted ones.
    
    -- Note: This will likely be called WHILE iterating through children from parent
    -- if self.parent then
      -- for i = 0, #self.parent.children do
        -- if self.parent.children[i] == self then
          -- table.remove(self.parent.children, i)
        -- end
      -- end
    -- end
    
    if self.entity then
      EntityFactory:DeleteEntity( self.entity:ID() )
      self.entity = nil
      self.ID = nil
    end
  end
end

function MenuElement:HandleMessage(type, stack)
  if self.messageHandler then
    self:messageHandler(type, stack)
  end
end

function MenuElement:Update()
  if self.deleteTimer > 0 then
    self.deleteTimer = self.deleteTimer - Game:FrameElapsed()
    
    if self.deleteTimer <= 0 then
      self:Delete()
    end
  end
  
  if self.updateFunction then
    self:updateFunction()
  end
  
  self:UpdateChildren()
end

function MenuElement:UpdateChildren()
  -- Update all children
  for i = 1, #self.children do
    self.children[i]:Update()
  end
  
  -- Remove deleted children
  local temp = {}
  
  for i = 1, #self.children do
    if self.children[i].entity then
      table.insert( temp, self.children[i] )
    end
  end
  
  self.children = temp
end

function MenuElement:Enable()
  if self.disabled == true then
    self.disabled = false
    self:SetGraphicState("UP")
  end
end

function MenuElement:Disable()
  if self.disabled == false then
    self.disabled = true
    self:SetGraphicState("DISABLED")
  end
end

function MenuElement:Fade(startAlpha, endAlpha, time)
  if not time then time = menu.fadeTime end
  
  self:SetAlpha( startAlpha )
  self:SetAlpha( endAlpha, time)
end

function MenuElement:FadeIn(time)
  self:Fade(0, 1, time)
end

function MenuElement:FadeOut(time)
  if not time then time = menu.fadeTime end
  
  self:SetAlpha(0, time)
end

function MenuElement:Orphan()
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    if element then
      element:Base():OrphanTransform()
      element:Base():Orphan()
    end
  end
end

function MenuElement:Close(time)
  if not time then time = menu.fadeTime end
  
  self:Orphan()
  
  MenuSystem:AddToUpdateList(self.ID)
  
  self:Delete(time)
end



function MenuElement:SetAlignment(horizontal, vertical)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    if element and element.SetAlignment then
      element:SetAlignment(horizontal, vertical)
    end
    
    local text = self.entity:GetFirstComponent( "text" )
    
    if text then
      text:SetHorizontalAlignment(horizontal)
      text:SetVerticalAlignment(vertical)
    end
  end
end

function MenuElement:SetAlpha(alpha, period)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )

    if element then
      if period then
        element:SetAlpha(alpha, period)
      else
        element:SetAlpha(alpha)
      end
    end
  end
end

function MenuElement:SetCallback(cbType, callback)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    local func = element[cbType]
    
    if func then
      func( element, callback )
    end
  end
end

function MenuElement:SetDimensions(width, height)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    local graphic = self.entity:GetFirstComponent( "ui_graphic" )
    
    if element then
      element:Base():SetDimensions(width, height)
    end
    
    if graphic and graphic.SetWidth and graphic.SetHeight then
      graphic:SetWidth(width)
      graphic:SetHeight(height)
    end
  end
end

function MenuElement:SetMessageHandler(messageHandler)
  self.messageHandler = messageHandler
end

function MenuElement:SetSliderDimensions(width, height)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    local barGraphic = self.entity:GetComponentByName( "ui_graphic", "barGraphic" )
    local sliderGraphic = self.entity:GetComponentByName( "ui_graphic", "sliderGraphic" )
    
    if element then
      element:SetDimensions(width, height)
    end
    
    if barGraphic then
      barGraphic:SetWidth(width)
      barGraphic:SetHeight(height / 10)
    end
    
    if sliderGraphic then
      sliderGraphic:SetWidth(height / 3)
      sliderGraphic:SetHeight(height / 3)  
    end
  end
end

function MenuElement:SetGraphic(assetName, graphicState)
  if self.entity then
    local graphic = self.entity:GetFirstComponent( "ui_graphic" )
    
    if graphic then
      if not graphicState then
        graphicState = "UP"
      end
      
      graphic:SetTexture(graphicState, assetName)
    end
  end
end

function MenuElement:SetMargin(top, right, bottom, left)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    if element then
      element:SetMargin(top,right,bottom,left)
    end
  end
end

function MenuElement:SetLayout(layout)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    if element then
      element:SetLayout(layout)
    end
  end
end

function MenuElement:SetPadding(top, right, bottom, left)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    if element then
      element:Base():SetPadding(top, right, bottom, left)
    end
  end
end

function MenuElement:SetPosition(horizontal, vertical)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    if element then
      element:SetPosition(horizontal, vertical)
    end
  end
end

function MenuElement:SetPositionMode(mode)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    if element then
      element:SetPositionMode(mode)
    end
  end
end

function MenuElement:SetRange(lower, upper)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    if element then
      element:SetRange(lower, upper)
    end
  end
end

function MenuElement:SetContent(text)
  -- This isn't handled through SetText because the inputregion entity
  -- has a ui_graphic as well, and there is no clean/nonbreaking way to
  -- handle that in SetText...
  
  if self.entity then
    local inputRegion = self.entity:GetFirstComponent( "ui_element" )
     
    if inputRegion then
      inputRegion:Content( text )
    end
  end
end

function MenuElement:SetText(text,colorVec)
  if self.entity then
    local graphic = self.entity:GetFirstComponent( "ui_graphic" )
    local color = colorVec or Vec3( 255,130,0,255 )
     
    if graphic then
      graphic:SetText(text)
      graphic:SetFont(defaultFont)
      graphic:SetTextColor(255,255,255,255, color:Elements() )
    else
      local textCmp = self.entity:GetFirstComponent( "text" )
      if textCmp then
        textCmp:SetText(text)
        textCmp:SetTextColor(255,255,255,255, color:Elements() )
        self:SetDimensions( textCmp:GetRegionWidth(), textCmp:GetRegionHeight() )
      end
    end
  end
end

function MenuElement:SetFontSize(size)
  if self.entity then
    local graphic = self.entity:GetFirstComponent( "ui_graphic" )
    
    if graphic then
      graphic:SetTextSize(size)
    else
      local textCmp = self.entity:GetFirstComponent( "text" )
      
      if textCmp then
        textCmp:SetFontSize(size)
        self:SetDimensions( textCmp:GetRegionWidth(), textCmp:GetRegionHeight() )
      end
    end
  end
end

function MenuElement:SetTextBoolean(bool, textA, textB)
  if bool then
    self:SetText(textA)
  else
    self:SetText(textB)
  end
end

function MenuElement:SetUpdateFunction(fn)
  self.updateFunction = fn
end

function MenuElement:SetValue(value)
  if self.entity then
    local element = self.entity:GetFirstComponent( "ui_element" )
    
    if element then
      element:SetValue(value)
    end
  end
end

function MenuElement:SetFillPercent(value)
  if self.entity then
    local gfx = self.entity:GetFirstComponent( "ui_graphic" )
    if gfx then
      gfx:SetResourcePercent(value)
    end
  end
end

function MenuElement:SetGraphicState(state)
  if self.entity then
    local graphic = self.entity:GetFirstComponent("ui_graphic")
    
    if graphic then
      graphic:Base():SetState( state )
    end
  end
end

function MenuElement:SetGraphicStateSlider(state)
  if self.entity then
    local graphic = self.entity:GetComponentByName("ui_graphic", "sliderGraphic")
    
    if graphic then
      graphic:Base():SetState( state )
    end
  end
end

function ButtonMessageHandler(element, type, stack)
  if element.disabled == false then
    if type == "onMouseOver" then
      element:SetGraphicState("OVER")
      
      if element.toolTip then
        element.toolTip:Show()
      end
      
      local sound = element.entity:GetComponentByName("sound", "onMouseOverSound")

      if sound then
        sound:Play()
      end
    elseif type == "onMouseOut" then
      element:SetGraphicState("UP")
      
      if element.toolTip then
        element.toolTip:Hide()
      end
      
    elseif type == "onClick" then
      local sound = element.entity:GetComponentByName("sound", "onClickSound")

      if sound then
        sound:Play()
      end
    end
  end
end
