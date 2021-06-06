classdef (Abstract) Port < handle & matlab.mixin.Heterogeneous
  
  properties
    connections gnuradio.Block
  end
  
  methods (Abstract)
    addConnection(obj,port);
  end
end