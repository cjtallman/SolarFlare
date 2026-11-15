-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

--
-- Note: Non-inheritable
-- Nice, friendly wrapper for creating new simple classes. Automagacially adds a
-- metatable, neatly handles methods, overloads () for constructor, and has
-- IsType() method for checking if a table is a given class type
--
function ClassSimple( ctor )
  local newClass = { }
  
  -- Unknown index attemps will look in itself (weird, I know)
  -- Makes since when you consider adding functions/data after this returns
  newClass.__index = newClass

  local mt = { }
  
  -- Overload () so it is now the constructor
  mt.__call = function( class_tbl, ... )
                local obj = { }
                setmetatable( obj, newClass )
                
                if ctor then
                  ctor( obj, ... )
                end
                
                return obj
              end
  
  newClass.IsA = function( self, rhs )
                   if type(rhs) == 'table' then
                     return (newClass == rhs.__index)
                   end
                   
                   return false
                 end
  
  setmetatable( newClass, mt )
  
  return newClass
end


--
-- Same basic interface as ClassSimple, though now inheritable by providing
-- a base class as the first parameter.
-- NOTE: If a derivied class provides a constructor, it must call base.Init
--
function ClassInheritable( baseClass, ctor )
  local newClass = { }
  
  if not ctor and type( baseClass ) == 'function' then
    ctor = baseClass
    baseClass = nil
  elseif type( baseClass ) == 'table' then
    -- Copying everything is simpler than recurvise index lookups, vtable, etc
    for i, v in pairs( baseClass ) do
      newClass[i] = v
    end
    
    newClass._base = base
  end
  
  -- Lookup failed indexes into self (see comment in ClassSimple implementation)
  newClass.__index = newClass

  -- Overload () so it behaves like a construcotr
  local mt = { }
  
  mt.__call = function( class_tbl, ... )
                local obj = { }
                setmetatable( obj, newClass )
                
                if ctor then
                  ctor( obj, ... )
                elseif (baseClass and baseClass.Init) then
                  baseClass.Init( obj, ... )
                end
              
              return obj
            end
  
  newClass.Init = ctor
  
  newClass.IsA = function( self, rhs )
                   if type(rhs) == 'table' then
                     local index = newClass
                     
                     while index do
                       if index == rhs.__index then
                         return true
                       end
                       
                       index = index._base
                     end
                     
                     return false
                   end
                 end
  
  setmetatable( newClass, mt )
  
  return newClass
end
