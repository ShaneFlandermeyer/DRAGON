classdef Add < runtime.SyncBlock

  methods
    
    %% Constructor
    function obj = Add(parent,varargin)
      obj@runtime.SyncBlock(parent,varargin{:})
    end
    
    function outputItems = work(obj,nOutputItemsMax,inputItems)
      
      outputItems = sum(inputItems,2);
      
    end
    
  end
  
  
end
