classdef (Abstract) SourceBlock < runtime.Block
  
  properties (Dependent)
    nInputItems
    nOutputItems
  end
  
  properties (Access = protected)
    d_nInputItems
    d_nOutputItems
  end

  %% Abstract methods
  methods (Abstract)
    work(obj)
  end
  
  %% Constructor
  methods
    
    function obj = SourceBlock(parent,varargin)
      nPortsDefault = 1;
      nOutputItemsDefault = 4096;
      
      p = inputParser;
      p.addParameter('nOutputPorts',nPortsDefault);
      p.addParameter('nOutputItems',nOutputItemsDefault);
      p.parse(varargin{:});
      obj@runtime.Block(parent);
      
      obj.nInputItems = 0;
      for ii = 1:p.Results.nOutputPorts
        obj.addOutputPort();
      end
      obj.nOutputItems = p.Results.nOutputItems;
    end
    
  end
  
  %% Setters/Getters
  methods
    function set.nInputItems(obj,n)
      if (n ~= 0)
        error('Cannot set nInputItems for Source Block')
      end
      obj.d_nInputItems = n;
     
    end
    
    function set.nOutputItems(obj,n)
      obj.d_nOutputItems = n;
    end
    
    function n = get.nInputItems(obj)
      n = obj.d_nInputItems;
    end
    
    function n = get.nOutputItems(obj)
      n = obj.d_nOutputItems;
    end
  end

  
  
end
