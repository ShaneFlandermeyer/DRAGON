% A block that does nothing, used for testing purposes
% This is equivalent to the "nop" block in runtime
% See: https://www.runtime.org/doc/doxygen/classgr_1_1runtime_1_1nop.html
classdef EmptyBlock < runtime.SyncBlock

  
  methods

    function outputItems = work(obj,nOutputItemsMax,inputItems)
      
      outputItems = inputItems;
      
    end
    
  end
  
  methods
    
    function obj = EmptyBlock(parent,varargin)

      p = inputParser();
      p.KeepUnmatched = true;
      % TODO: Input validation
      p.addParameter('nInputPorts',1);
      p.addParameter('nOutputPorts',1);
      p.parse(varargin{:});
      
      if p.Results.nInputPorts ~= p.Results.nOutputPorts
        error('The number of input ports must equal the number of output ports')
      end
      obj@runtime.SyncBlock(parent,varargin{:})
    end
    
  end
end
