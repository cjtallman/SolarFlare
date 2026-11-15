-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--vec3.lua

Vec3 = ClassSimple( function( self, x, y, z )
                      self.x = x or 0
                      self.y = y or 0
                      self.z = z or 0
                    end
                  )

--
-- Operator overloads
--
function Vec3:__add( rhs )
  return Vec3( self.x + rhs.x, self.y + rhs.y, self.z + rhs.z )
end

function Vec3:__div( rhs )
  return Vec3( self.x / rhs, self.y / rhs, self.z / rhs )
end

function Vec3:__eq( rhs )
  return (self.x == rhs.x) and (self.y == rhs.y) and (self.z == rhs.z)
end

function Vec3:__mul( rhs )
  if Vec3:IsA( self ) and Vec3:IsA( rhs ) then
    return self:Dot( rhs )
  elseif Vec3:IsA( rhs ) then
    return Vec3( self * rhs.x, self * rhs.y, self * rhs.z )
  else
    return Vec3( self.x * rhs, self.y * rhs, self.z * rhs )
  end
end

function Vec3:__pow( rhs )
  return Vec3( self.x ^ rhs, self.y ^ rhs, self.z ^ rhs )
end

function Vec3:__sub( rhs )
  return Vec3( self.x - rhs.x, self.y - rhs.y, self.z - rhs.z )
end

function Vec3:__tostring()
  return string.format( "(%g,%g,%g)", self.x, self.y, self.z )
end

function Vec3:__unm()
  return Vec3( -self.x, -self.y, -self.z )
end


--
-- Class methods
--
function Vec3:Clone()
  return Vec3( self.x, self.y, self.z )
end

function Vec3:Cross( rhs )
  local u = ( self.y * rhs.z ) - ( self.z * rhs.y )
  local v = ( self.z * rhs.x ) - ( self.x * rhs.z )
  local w = ( self.x * rhs.y ) - ( self.y * rhs.x )
  
  return Vec3( u, v, w )
end

function Vec3:Dot( rhs )
  return (self.x * rhs.x) + (self.y * rhs.y) + (self.z * rhs.z)
end

function Vec3:Elements()
  return self.x, self.y, self.z
end

function Vec3:Magnitude()
  return math.sqrt( self:Dot( self ) )
end

function Vec3:MagnitudeSquared()
  return self:Dot( self )
end

function Vec3:Normalize()
  local mag = self:Magnitude()
  self.x = self.x / mag
  self.y = self.y / mag
  self.z = self.z / mag
end

function Vec3:Normalized()
  local copy = self:Clone()
  copy:Normalize()
  return copy
end
