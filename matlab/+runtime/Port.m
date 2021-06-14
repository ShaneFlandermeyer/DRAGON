classdef (Abstract) Port < handle & matlab.mixin.Heterogeneous
  
  properties (SetAccess = protected)
    connections runtime.Port
    buffer (1,1) Queue
  end
  
  methods (Abstract)
    connect(obj,port);
    disconnect(obj,port);
  end
  
  methods
    
    function obj = Port()
      obj.buffer = Queue;
    end

  end

end