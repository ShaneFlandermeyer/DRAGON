classdef (Abstract) SyncBlock < runtime.Block
  
  properties (Dependent)
    nInputItemsMax
    nOutputItemsMax
  end
  
  properties (Access = protected)
    d_nInputItemsMax = 4096
    d_nOutputItemsMax = 4096
  end

  %% Abstract Methods
  methods (Abstract)
    outputItems = work(obj,nOutputItemsMax,inputItems)
  end
  
  %% Constructor
  methods
    
    function obj = SyncBlock(parent,varargin)
      nPortsDefault = 1;
      nItemsDefault = 4096;
      
      p = inputParser;
      p.addParameter('nInputPorts',nPortsDefault);
      p.addParameter('nOutputPorts',nPortsDefault);
      p.addParameter('nInputItemsMax',nItemsDefault);
      p.addParameter('nOutputItemsMax',nItemsDefault);
      p.parse(varargin{:});
      obj@runtime.Block(parent);
      
      if (obj.nInputItemsMax ~= obj.nOutputItemsMax)
        error('For Sync blocks, nInputItemsMax must equal nOutputItemsMax')
      end
      
      obj.nInputItemsMax = p.Results.nInputItemsMax;
      obj.nOutputItemsMax = p.Results.nOutputItemsMax;
      
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
    
    function set.nInputItemsMax(obj,n)
      obj.d_nInputItemsMax = n;
      obj.d_nOutputItemsMax = n;
    end
    
    function set.nOutputItemsMax(obj,n)
      obj.d_nInputItemsMax = n;
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
