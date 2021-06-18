classdef NullSink < runtime.SinkBlock

  
  methods
    function obj = NullSink(parent,varargin)
      obj@runtime.SinkBlock(parent,varargin{:});
    end
    
    function outputItems = work(obj,nOutputItemsMax,inputItems)
      % Do nothing
      outputItems = [];
    end
    
  end
end
