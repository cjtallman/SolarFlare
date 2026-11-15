-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--title.lua

function EnterTitle()
  local titleRegion = menu.root:CreateRegion()
    titleRegion:SetAlignment("CENTER", "MIDDLE")
    titleRegion:SetLayout("TopToBottom")
    titleRegion:SetPositionMode("ABSOLUTE")
    titleRegion:SetPosition("CENTER", "TOP")
  
  local title = titleRegion:CreateImage()
    title:SetDimensions(548,160)
    title:SetGraphic("title")
        
  local region = MenuSystem:BeginRegion("IntroTitleRegion")
  MenuSystem:RegionAlignment("CENTER", "MIDDLE")
  MenuSystem:RegionLayout("TopToBottom")
  MenuSystem:RegionMargin(0,0,0,0)
  region:SetAlignment("ABSOLUTE")
  region:SetPosition("CENTER", "TOP")
    local entity = MenuSystem:CreateElement("STATICBUTTON")
    
    local button = entity:GetFirstComponent( "ui_element" )
    local graphic = entity:GetFirstComponent( "ui_graphic" )

    button:SetDimensions(548,160)
    graphic:SetWidth(548)
    graphic:SetHeight(160)
    graphic:SetTexture("UP","title")
    
  MenuSystem:EndRegion()
  
  region = MenuSystem:GetElement("IntroTitleRegion")
  region:SetAlpha(0)
  region:SetAlpha(1, menuSettings.interpolateTime)
end

function LeaveTitle()
  region = MenuSystem:GetElement("IntroTitleRegion")
  
  if region then
    region:SetAlpha(0, menuSettings.interpolateTime)
    MenuSystem:ClearRegion("IntroTitleRegion", menuSettings.interpolateTime)
  end
end 

titleState = 
{
  Enter = EnterTitle,
  Leave = LeaveTitle
}
