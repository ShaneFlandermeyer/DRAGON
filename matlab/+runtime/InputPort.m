classdef InputPort < runtime.Port
  
  methods
    
    function obj = InputPort(parent)
      obj@runtime.Port(parent);
    end
    
    function connect(obj,port)
      validateattributes(port,'runtime.OutputPort',{})
      if ~isempty(obj.connections)
        obj.deleteConnection(obj.connections(1));
      end
      obj.addConnection(port);
      port.addConnection(obj);
%       obj.connections = port;
%       port.connections = [port.connections; obj];
    end
    
    function disconnect(obj,port)
      validateattributes(port,'runtime.OutputPort',{})
      obj.deleteConnection(port);
      port.deleteConnection(obj);
%       % Find the connection in this object's list and delete it
%       idx = find(obj.connections == port);
%       obj.connections(idx) = [];
%       
%       % Find the connection in the input port's list and delete it
%       idx = find(port.connections == obj);
%       port.connections(idx) = [];
    end

  end
  
end
