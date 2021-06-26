classdef Delay < runtime.GeneralBlock
  properties 
    delay
    
  end
  
  properties (Access = private)
    delta
  end
  
  methods
    function obj = Delay(parent,delay,varargin)
      obj@runtime.GeneralBlock(parent,varargin{:});
      obj.delta = delay;
      obj.delay = delay;
    end
    
    function forecast()
      % TODO: Implement me
    end
    
    function outputItems = general_work(obj,nInputItems,nOutputItems,inputItems,outputItems)
      if (obj.delta == 0) 
        for iPort = 1 : size(inputItems,2)
          outputItems(:,iPort) = inputItems(:,iPort);
        end
        cons = nOutputItems;
        
      elseif obj.delta < 0
        % Skip over obj.delta input items
        % TODO: Implement me
      
      else % delta > 0
        nFromInput = max(0,nInputItems-obj.delta);
        nPadding = min(obj.delta,nOutputItems);
        for iPort = 1 : size(inputItems,2)
          outputItems(1:nPadding,iPort) = zeros(nPadding,1);
          outputItems(nPadding+1:nPadding+nFromInput,iPort) = inputItems(1:nFromInput,iPort);
        end
        cons = nFromInput;
        obj.delta = obj.delta - nPadding;
      end
      
      obj.consumeEach(cons);
    end
  end
end
