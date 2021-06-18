classdef TimeSink < runtime.SinkBlock
  
  properties
    % TODO: Implement me!
  end

  methods

    function obj = TimeSink(parent,varargin)
      obj@runtime.SinkBlock(parent,varargin{:});
    end
    
    function outputItems = work(obj,nOutputItemsMax,inputItems)
      
      plot(inputItems);
      outputItems = [];
      
    end
    
  end % methods
  
end % class
