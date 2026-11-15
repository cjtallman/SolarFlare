-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--helperFunctions.lua

function printt( tbl )
  if type(tbl) == "userdata" then
    tbl = getmetatable( tbl )
  end
  
  local printed = false
  
  for k, v in pairs( tbl ) do
    print( "   " .. k .. " = " .. tostring( v ) )
    printed = true
  end
  
  if printed == false then
    print( "   (empty)" )
  end
end

local function PrintWrapper( tbl )
  local str = '   '
  local printed = false
  
  for i, v in ipairs( tbl ) do
    printed = true
    
    if type(v) == "userdata" or (type(v) == "table" and not v.__tostring) then
      printt( v )
    else
      str = str .. tostring( v )
    end
  end
  
  if not printed then
    str = str .. "nil"
  end
  
  return str
end

function IsPaused()
  return GameLib.PhysicsPaused()
end

function IsServer()
  return Network:Mode() == "Server" or Network:Mode() == "Idle"
end

function IsClient()
  return Network:Mode() == "Client" or Network:Mode() == "FindServer"
end

function SetName(name)
  Network:ClientName(name)
end

function CartesianToSpherical(x, y, z)
  local phi = math.pi / 2 - math.acos( y );
  local theta = math.atan2( x, z ) + math.pi;
  
  return theta, phi
end

function SphericalToCartesian(theta, phi)
  theta = theta - math.pi
  
  local x = math.sin(theta) * math.cos(phi)
  local y = math.sin(phi)
  local z = math.cos(theta) * math.cos(phi)
  
  return Vec3(x, y, z)
end

-- TODO: Move this function to a more appropriate place?
-- Take a normalized local point and get the actual surface height
function GetPlanetSurfaceHeight(planetEntity, x, y, z)
  local theta, phi = CartesianToSpherical( x, y, z )
 	
 	local planet = planetEntity:GetFirstComponent( "planet" )

  local maxRadius = planet:GetMaxRadius()
  local minRadius = planet:GetMinRadius()

  local height = planet:GetHeight(x,y,z,theta, phi)
  return minRadius + height * (maxRadius - minRadius)
end

-- Override global print to funnel to console
print    = function(...) Console:Message( PrintWrapper( arg ) ) end
cerror   = function(...) Console:Error( PrintWrapper( arg ) ) end
cdebug   = function(...) Console:Debug( PrintWrapper( arg ) ) end
cwarning = function(...) Console:Warning( PrintWrapper( arg ) ) end


-- Override default random function
math.random = GameLib.RRand

math.clamp = function( val, min, max )
               if val < min then
                 return min
               elseif val > max then
                 return max
               end
               
               return val
             end


YIELD_FRAME = 0
YIELD_TIME  = 1


MSG_ALL         = 0
MSG_LOCAL       = 1
MSG_ALL_PEERS   = 2
MSG_SINGLE_PEER = 3


NET_STATE_READY     = 0 
NET_STATE_SYNCING   = 1
NET_STATE_LOADING   = 2
NET_STATE_NOT_READY = 3
