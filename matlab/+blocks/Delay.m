classdef Delay < runtime.GeneralBlock
  properties 
    delay
  end
  
  methods
    function obj = Delay(parent,delay,varargin)
      obj@runtime.GeneralBlock(parent,varargin{:});
      obj.delay = delay;
    end
    
    function forecast()
      % TODO: Implement me
    end
    
    function outputItems = general_work(obj,nInputItems,nOutputItems,inputItems,outputItems)
      outputItems = delayseq(inputItems,obj.delay);
      for iPort = 1 : length(obj.inputPorts)
        obj.consume(nInputItems,iPort);
      end
    end
  end
end
