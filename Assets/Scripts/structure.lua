-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--structure.lua - Base class for all structures

local function StructureConstructor( self, ID, playerIndex, gridX, gridY )
  self.entity = EntityFactory:GetEntity( ID )
  self.ID = ID
  self.playerIndex = playerIndex
  self.gridX = gridX
  self.gridY = gridY

  self.connections = {}
end

Structure = ClassInheritable( StructureConstructor )

function Structure:AddConnection(connection)
  table.insert( self.connections, connection )
end

function Structure:RemoveConnection(connection)
  for i = 1, #self.connections do
    if self.connections[i] == connection then
      table.remove( self.connections, i )
      break
    end
  end
end

function Structure:Connected(otherStructure)
  for i = 1, #self.connections do
    if self.connections[i] == otherStructure then
      return true
    end
  end
  
  return false
end

function Structure:Delete()
  for i = 1, #self.connections do
    if not self then print("self=nil") end
    if not self.connections[i] then print("connection=nil") end
    self.connections[i]:RemoveConnection( self )
  end
  
  self.connections = {}
  
  if self.entity then
    EntityFactory:DeleteEntity( self.entity:ID() )
    
    self.entity = nil
    self.ID = nil
  end
end
