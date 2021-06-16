classdef SinkBlock < runtime.Block
  
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
    
    function obj = SinkBlock(parent,varargin)
      nPortsDefault = 1;
      nInputItems = 4096;
      
      p = inputParser;
      p.addParameter('nInputPorts',nPortsDefault);
      p.addParameter('nInputItems',nInputItems);
      p.parse(varargin{:});
      obj@runtime.Block(parent);
      
      obj.nOutputItems = 0;
      for ii = 1:p.Results.nInputPorts
        obj.addInputPort();
      end
      
      obj.nInputItems = p.Results.nInputItems;
    end
    
  end
  
  %% Setters/Getters
  methods
    
    function set.nInputItems(obj,n)
      obj.d_nInputItems = n;
     
    end
    
    function set.nOutputItems(obj,n)
      if (n ~= 0)
        error('Cannot set nInputItems for Source Block')
      end
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
