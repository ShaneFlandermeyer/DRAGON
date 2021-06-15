classdef (Abstract) Port < handle & matlab.mixin.Heterogeneous
  
  properties (SetAccess = protected)
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

  end

end
