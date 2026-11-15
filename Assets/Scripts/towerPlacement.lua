-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--towerPlacement.lua

function UpdateTowerPlacementControls()
  if MenuSystem:ProcessedClick() then
    PopGameState()
    return
  end

  if Input:IsTriggered( InputMap.Escape ) then
    PopGameState()
    return
  end

  if not game.players[ MyPlayerIndex() ].planet.entity then
    PopGameState()
    return
  end
  
  local mx, my = Input:MousePosition()
  
  local entity, x, y, z = GetEntity( mx, my, "planets" )
  
  local placementPos = Vec3(x,y,z)
  
  if entity and entity:GetFirstComponent( "planet" ) then
    local planetEntity = entity:GetFirstComponent( "planet" )
    local planetTransform = entity:GetFirstComponent( "transform" )
    
    local modelTransform = towerPlacementState.placementModel:GetFirstComponent( "transform" )
    local model = towerPlacementState.placementModel:GetFirstComponent( "model" )

    local playerIndex = MyPlayerIndex()
    local planet = game.players[playerIndex].planet


    
    local planetPos = Vec3( planetTransform:GetWorldPosition() )

    local dist = nil
    local h = 0
    local v = Vec3()
    local actualPlacementPos = placementPos
    
    local R = (placementPos - planetPos):Magnitude()
    
    v = Vec3( planetTransform:WorldToLocal(actualPlacementPos.x, actualPlacementPos.y, actualPlacementPos.z) )
    
    local theta, phi = CartesianToSpherical( v.x, v.y, v.z )
    
    gridX, gridY = planet:SphericalToGrid(theta, phi)
    
    theta, phi = planet:GridToSpherical(gridX, gridY)
    
    v = SphericalToCartesian( theta, phi )
    h = GetPlanetSurfaceHeight( entity, v:Elements() )
    
    v = v * h
    
    modelTransform:SetParent( entity:GetFirstComponent( "transform" ) )
    modelTransform:SetPosition( v:Elements() )
    modelTransform:SetOrientationFromPosition( 0, 1, 0 )
        
    if not planet:GetStructure(gridX, gridY) and h - planetEntity:GetMinRadius() > 0.1 and entity:ID() == MyPlanetID()
    then
      model:SetModelColor(0,255,0,0)
      
      if Input:IsTriggered( InputMap.MouseLeft ) then
        towerPlacementState.placementFunction( game.players[playerIndex].planet, gridX, gridY ) --Vec3(modelTransform:GetLocalPosition()) )
        
        gridX, gridY = planet:WrapGridCoords(gridX, gridY)
        
        PopGameState()
        
        local playerIndex = MyPlayerIndex()
        
        --game.players[playerIndex].buildCount = game.players[playerIndex].buildCount - 1
      end
    else
      if Input:IsTriggered( InputMap.MouseLeft ) then
        PopGameState()
      end
      
      model:SetModelColor(255,0,0,0)
    end
    
  else
    if Input:IsTriggered( InputMap.MouseLeft ) then
      PopGameState()
    end
    
    if towerPlacementState.placementModel then
      local model = towerPlacementState.placementModel:GetFirstComponent( "model" )
      local modelTransform = towerPlacementState.placementModel:GetFirstComponent( "transform" )
      
      model:SetModelColor(255,255,255,0)
      
      local ray = {}
      ray.x, ray.y, ray.z, ray.dx, ray.dy, ray.dz = GraphicsEngine:ScreenToWorld( Input:MousePosition() )
      
      modelTransform:SetParent(nil)
      modelTransform:SetPosition( ray.x + ray.dx * 5000, ray.y + ray.dy * 5000, ray.z + ray.dz * 5000 )
      modelTransform:SetRotation( 0, 0, 0 )
    end
  end  

end

function EnterTowerPlacement()
  towerPlacementState.placementModel = EntityFactory:Create( towerPlacementState.placementModelType )
    
  game.camera:EnterBuildMode()
end

function LeaveTowerPlacement()
  EntityFactory:DeleteEntity( towerPlacementState.placementModel:ID() )
  towerPlacementState.placementModel = nil
end

function InitTowerPlacementState()
  towerPlacementState = GameState()
  
  towerPlacementState.OnPush = EnterTowerPlacement
  towerPlacementState.OnPop = LeaveTowerPlacement
  
  towerPlacementState.Updates = {
    UpdateMenus,
    UpdateTowerPlacementControls
  }
  
  towerPlacementState.placementFunction = nil
  towerPlacementState.placementModelType = nil
  towerPlacementState.placementModel = nil
end
