classdef Delay < runtime.GeneralBlock
  properties 
    delay = 0
    
  end
  
  properties (Access = private)
    delta = 0
  end
  
  methods
    function obj = Delay(parent,delay,varargin)
      
      % For this block, the number of input ports must be the same as the number
      % of output ports. We can pass everything else to the superclass
      % constructors
      p = inputParser();
      p.KeepUnmatched = true;
      p.addParameter('nInputPorts',1);
      p.addParameter('nOutputPorts',1);
      p.parse(varargin{:});
      obj@runtime.GeneralBlock(parent,varargin{:});
      obj.setDelay(delay);
      % TODO: Do not allow the user to set a negative delay initially to
      % avoid violating causality
      
    end
    
    function setDelay(obj,d)
      if (d ~= obj.delay)
        obj.delta = d - obj.delay;
        obj.delay = d;
      end
      
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
        nDelta = -obj.delta;
        nFromInput = max(0,nOutputItems-nDelta);
        for iPort = 1 : size(inputItems,2)
          outputItems(1:nFromInput,iPort) = inputItems(1+nDelta:nDelta+nFromInput,iPort);
        end
        cons = nOutputItems;
        nDelta = nDelta - min(obj.delta,nOutputItems);
        obj.delta = -nDelta;
      
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
