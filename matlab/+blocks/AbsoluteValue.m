classdef AbsoluteValue < runtime.SyncBlock
  
  methods
    function obj = AbsoluteValue(parent,varargin)
      p = inputParser();
      p.KeepUnmatched = true;
      p.addParameter('nInputPorts',1);
      p.addParameter('nOutputPorts',1);
      p.parse(varargin{:});
      if p.Results.nInputPorts ~= p.Results.nOutputPorts
        error('The number of input ports must equal the number of output ports')
      end

      obj@runtime.SyncBlock(parent,varargin{:})
    end
    
    function outputItems = work(obj,nOutputItemsMax,inputItems)
      
      outputItems = abs(inputItems);
      
    end
  end
end
