-- Copyright © 2008 DigiPen (USA) Corporation. All rights reserved.

-- queue.lua

Queue = ClassSimple( function( self, copy )
                       self.front = 1
                       self.back = 0
                       self.contents = { }
                       
                       if Queue:IsA( copy ) then
                         local size = copy:Size()
                         for i = front, size do
                           self:Push( copy.contents[i] )
                         end
                       end
                     end
                   )

--
-- Operator overloads
--
function Queue:__tostring()
  return "queue tostring unimplemented"
end


--
-- Class methods
--
function Queue:Back()
  if self:Empty() then
    error( "Attempt to access back of empty queue" )
  end
  
  return self.contents[self.back]
end

function Queue:Contents()
  if self.front == 1 then
    return self.contents
  end
  
  local tbl =  { }
  
  local size = self:Size()
  for i = self.front, size do
    tbl[ #tbl + 1 ] = self.contents[i]
  end
  
  return tbl
end

function Queue:Clone()
  return Queue( self )
end

function Queue:Empty()
  return self.front > self.back
end

function Queue:Front()
  if self:Empty() then
    error( "Attempt to access front of empty queue" )
  end
  
  return self.contents[self.front]
end

function Queue:Pop()
  if self:Empty() then
    error( "Attempt to pop empty queue" )
  end
  
  local size = self:Size()
  
  local temp = self.contents[self.front]
  self.contents[self.front] = nil
  self.front = self.front + 1
    
  if size == 1 then
    self.front = 1
    self.back = 0
  end
  
  return temp
end

function Queue:Push( item )
  self.back = self.back + 1
  self.contents[self.back] = item
end

function Queue:Size()
  return self.back - self.front + 1
end
