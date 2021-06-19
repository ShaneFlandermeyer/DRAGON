classdef OutputPort < runtime.Port
  
  methods
    
    function obj = OutputPort(parent)
      obj@runtime.Port(parent);
    end
    
    function connect(obj,port)
      validateattributes(port,'runtime.InputPort',{})
      obj.addConnection(port);
      port.addConnection(obj);
    end
    
    function disconnect(obj,port)
      validateattributes(port,'runtime.InputPort',{})
      obj.deleteConnection(port);
      port.deleteConnection(obj);
    end
    
  end
  
end
