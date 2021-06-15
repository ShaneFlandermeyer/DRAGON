classdef OutputPort < runtime.Port
  
  methods
    
    function obj = OutputPort(parent)
      obj@runtime.Port(parent);
    end
    
    function connect(obj,port)
      validateattributes(port,'runtime.InputPort',{})
      obj.connections = [obj.connections; port];
      port.connections = obj;
    end
    
    function disconnect(obj,port)
      validateattributes(port,'runtime.InputPort',{})
      % Find the connection in this object's list and delete it
      idx = find(obj.connections == port);
      obj.connections(idx) = [];
      
      % Find the connection in the input port's list and delete it
      idx = find(port.connections == obj);
      port.connections(idx) = [];
    end
    
  end
  
end
