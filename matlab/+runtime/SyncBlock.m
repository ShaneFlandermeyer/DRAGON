classdef (Abstract) SyncBlock < runtime.Block
  
  properties (Dependent)
    nInputItems
    nOutputItems
  end
  
  properties (Access = protected)
    d_nInputItems = 4096
    d_nOutputItems = 4096
  end
  %% Abstract Methods
  methods (Abstract)
    work(obj)
  end
  
  %% Constructor
  methods
    
    function obj = SyncBlock(parent,varargin)
      nPortsDefault = 1;
      nItemsDefault = 4096;
      
      p = inputParser;
      p.addParameter('nInputPorts',nPortsDefault);
      p.addParameter('nOutputPorts',nPortsDefault);
      p.addParameter('nInputItems',nItemsDefault);
      p.addParameter('nOutputItems',nItemsDefault);
      p.parse(varargin{:});
      obj@runtime.Block(parent);
      
      if (obj.nInputItems ~= obj.nOutputItems)
        error('For Sync blocks, nInputItems must equal nOutputItems')
      end
      
      obj.nInputItems = p.Results.nInputItems;
      obj.nOutputItems = p.Results.nOutputItems;
      
      % Create the specified number of input ports
      for iPort = 1:p.Results.nInputPorts
        obj.addInputPort();
      end
      
      % Create the specified number of output ports
      for iPort = 1:p.Results.nOutputPorts
        obj.addOutputPort();
      end
      
      
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
  
end