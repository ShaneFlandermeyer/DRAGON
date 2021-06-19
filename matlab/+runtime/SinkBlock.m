classdef SinkBlock < runtime.Block
  
  properties (Dependent)
    nInputItemsMax
    nOutputItemsMax
  end
  
  properties (Access = private)
    d_nInputItemsMax
    d_nOutputItemsMax
  end

  
  %% Abstract methods
  methods (Abstract)
    outputItems = work(obj,nOutputItemsMax,inputItems)
  end
  
  %% Constructor
  methods
    
    function obj = SinkBlock(parent,varargin)
      nPortsDefault = 1;
      nInputItemsMax = 4096;
      
      p = inputParser;
      p.addParameter('nInputPorts',nPortsDefault);
      p.addParameter('nInputItemsMax',nInputItemsMax);
      p.parse(varargin{:});
      obj@runtime.Block(parent);
      
      obj.nOutputItemsMax = 0;
      for ii = 1:p.Results.nInputPorts
        obj.addInputPort();
      end
      
      obj.nInputItemsMax = p.Results.nInputItemsMax;
    end
    
  end
  
  %% Setters/Getters
  methods
    
    function set.nInputItemsMax(obj,n)
      obj.d_nInputItemsMax = n;
     
    end
    
    function set.nOutputItemsMax(obj,n)
      if (n ~= 0)
        error('Cannot set nInputItemsMax for Source Block')
      end
      obj.d_nOutputItemsMax = n;
    end
    
    function n = get.nInputItemsMax(obj)
      n = obj.d_nInputItemsMax;
    end
    
    function n = get.nOutputItemsMax(obj)
      n = obj.d_nOutputItemsMax;
    end
    
  end
  
end 
