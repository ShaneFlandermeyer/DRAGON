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

      p = inputParser();
      p.KeepUnmatched = true;
      p.addParameter('nInputPorts',1);
      p.addParameter('nOutputPorts',0);
      p.addParameter('nInputPortsMin',1);
      p.addParameter('nInputPortsMax',inf);
      p.addParameter('nOutputPortsMin',0);
      p.addParameter('nOutputPortsMax',0);
      p.addParameter('vectorLength',1);
      p.parse(varargin{:});
      
      if p.Results.nOutputPorts ~= 0
        error('Sink blocks cannot have output ports')
      end
      
      results = namedargs2cell(p.Results);
      obj@runtime.Block(parent,results{:});
      
      p.addParameter('nInputItemsMax',4096);
      p.parse(varargin{:})
      obj.nOutputItemsMax = 0;
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
