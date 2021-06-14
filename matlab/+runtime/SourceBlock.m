classdef (Abstract) SourceBlock < runtime.Block
  
  properties (Dependent)
    nInputItems
    nOutputItems
  end
  
  properties (Access = protected)
    d_nInputItems = 0
    d_nOutputItems = 4096
  end
  
  %% Abstract methods
  methods (Abstract)
    work(obj)
  end
  
  %% Constructor
  methods
    
    function obj = SourceBlock()
      % Initialize the block to have 1 output port
      obj.addOutputPort();
    end
    
  end
  
  %% Setters/Getters
  methods
    function set.nInputItems(obj,~)
      error('Cannot set nInputItems for Source Block')
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