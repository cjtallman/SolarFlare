-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

Satellite = ClassSimple(
  function( self, parent, entityID )
    self.entity = EntityFactory:GetEntity( entityID )
    self.ID = entityID
    
    self.parent = parent
  end )

function Satellite:Delete()
  if self.entity then
    EntityFactory:DeleteEntity( self.entity:ID() )
    
    self.entity = nil
    self.ID = nil
  end
end
