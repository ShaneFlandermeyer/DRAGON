classdef (Abstract) Port < handle & matlab.mixin.Heterogeneous
  
  properties (SetAccess = private)
    connections runtime.Port
    buffer (1,1) Queue
    parent runtime.Block
  end
  
  methods (Abstract)
    connect(obj,port);
    disconnect(obj,port);
  end
  
  methods
    
    function obj = Port(parent)
      validateattributes(parent,{'runtime.Block'},{})
      obj.buffer = Queue;
      if isempty(parent)
        error('Port cannot exist without an associated block')
      end
      obj.parent = parent;
    end
    
    function addConnection(obj,port)
      assert(isa(port,'runtime.Port'));
      if isempty(obj.connections)
        obj.connections = port;
      else
        obj.connecttions = [obj.connections;port];
      end
    end
    
    function deleteConnection(obj,port)
      assert(isa(port,'runtime.Port'));
      
      % Find the connection in this object's list and delete it
      idx = find(obj.connections == port);
      obj.connections(idx) = [];
      
      % Find the connection in the input port's list and delete it
      idx = find(port.connections == obj);
      port.connections(idx) = [];
    end

  end

end
