classdef InputPort < runtime.Port
  
  methods
    
    function connect(obj,port)
      validateattributes(port,'runtime.OutputPort',{})
      obj.connections = port;
      port.connections = [port.connections; obj];
    end
    
    function disconnect(obj,port)
      validateattributes(port,'runtime.OutputPort',{})
      % Find the connection in this object's list and delete it
      idx = find(obj.connections == port);
      obj.connections(idx) = [];
      
      % Find the connection in the input port's list and delete it
      idx = find(port.connections == obj);
      port.connections(idx) = [];
    end

  end
  
end