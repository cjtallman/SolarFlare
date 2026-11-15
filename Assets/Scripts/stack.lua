-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

-- stack.lua

Stack = ClassSimple( function( self, copy )
                       self.top = 0
                       self.contents = { }
                       
                       if Stack:IsA( copy ) then
                         for i = 1, copy.top do
                           self:Push( copy.contents[i] )
                         end
                       end
                     end
                   )

--
-- Operator overloads
--
function Stack:__tostring()
  return "stack tostring unimplemented"
end


--
-- Class methods
--
function Stack:Clone()
  return Stack( self )
end

function Stack:Contents()
  return self.contents
end

function Stack:Empty()
  return self.top == 0
end

function Stack:Get( index )
  if index <= 0 or index > self:Size() then
    local str = string.format( "Tried to get invalid index %d (size %d)", index, self:Size() )
    error( str )
  end
  
  return self.contents[index]
end

function Stack:Pop()
  if self:Empty()  then
    error( "Attempt to pop empty stack"  )
  end
  
  local temp = self.contents[self.top]
  self.contents[self.top] = nil
  self.top = self.top - 1
  
  return temp
end

function Stack:Push( item )
  self.top = self.top + 1
  self.contents[self.top] = item
end

function Stack:Size()
  return self.top
end

function Stack:Top()
  if self:Empty() then
    error( "Tried to acces top of empty stack" )
  end
  
  return self.contents[self.top]
end
