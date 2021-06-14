classdef (Abstract) SyncBlock < runtime.Block
  
  properties (Dependent)
    nInputItems
    nOutputItems
  end
  
  properties (Access = protected)
    d_nInputItems = 4096
    d_nOutputItems = 4096
  end
  
  %% Constructor
  methods
    
    function obj = SyncBlock()
      obj.addInputPort();
      obj.addOutputPort();
    end
    
  end
  
  %% Setters and getters
  methods
    
    function set.nInputItems(obj,n)
      obj.d_nInputItems = n;
      obj.d_nOutputItems = n;
    end
    
    function set.nOutputItems(obj,n)
      obj.d_nInputItems = n;
      obj.d_nOutputItems = n;
    end
    
    function n = get.nInputItems(obj)
      n = obj.d_nInputItems;
    end
    
    function n = get.nOutputItems(obj)
      n = obj.d_nOutputItems;
    end
    
  end
  
  %% Abstract Methods
  methods (Abstract)
    work(obj)
  end
  
end