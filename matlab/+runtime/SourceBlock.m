classdef (Abstract) SourceBlock < runtime.Block
  
  properties (Dependent)
    nInputItemsMax
    nOutputItemsMax
  end
  
  properties (Access = protected)
    d_nInputItemsMax
    d_nOutputItemsMax
  end

  %% Abstract methods
  methods (Abstract)
    outputItems = work(obj,nOutputItemsMax,inputItems)
  end
  
  %% Constructor
  methods
    
    function obj = SourceBlock(parent,varargin)
      nPortsDefault = 1;
      nOutputItemsMaxDefault = 4096;
      
      p = inputParser;
      p.addParameter('nOutputPorts',nPortsDefault);
      p.addParameter('nOutputItemsMax',nOutputItemsMaxDefault);
      p.parse(varargin{:});
      obj@runtime.Block(parent);
      
      obj.nInputItemsMax = 0;
      for ii = 1:p.Results.nOutputPorts
        obj.addOutputPort();
      end
      obj.nOutputItemsMax = p.Results.nOutputItemsMax;
    end
    
  end
  
  %% Setters/Getters
  methods
    function set.nInputItemsMax(obj,n)
      if (n ~= 0)
        error('Cannot set nInputItemsMax for Source Block')
      end
      obj.d_nInputItemsMax = n;
     
    end
    
    function set.nOutputItemsMax(obj,n)
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
