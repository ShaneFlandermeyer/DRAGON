classdef OutputPort < blocks.Port
  
  methods
    
    function addConnection(obj,port)
      
      if ~isa(port,'blocks.InputPort')
        error('Output port can only connect to an input port')
      end
      % Output ports can have multiple connections, so apppend the new port
      % to the list
      obj.connections = [obj.connections; port];
      % Input ports can only have one connection, so replace the existing
      % one
      port.connections = obj;
    end
    
    function deleteConnection(obj,port)
      
      if ~isa(port,'blocks.InputPort')
        error('Output port can only connect to an input port')
      end
      
      % Find the connection in this object's list and delete it
      idx = find(obj.connections == port);
      obj.connections(idx) = [];
      
      % Find the connection in the input port's list and delete it
      idx = find(port.connections == obj);
      port.connections(idx) = [];
    end
    
  end
  
end