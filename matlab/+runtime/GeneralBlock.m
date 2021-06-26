classdef (Abstract) GeneralBlock < runtime.Block
  
  properties (Dependent)
    nInputItemsMax
    nOutputItemsMax
  end
  
  properties (Access = private)
    d_nInputItemsMax
    d_nOutputItemsMax
  end
  
  methods (Abstract)
    outputItems = general_work(obj,nInputItems,nOutputItems,inputItems,outputItems)
    forecast() % TODO: Implement this
  end
  
  methods
    
    function obj = GeneralBlock(parent,varargin)
      % Parse parameters needed for runtime.Block superclass
      p = inputParser();
      p.KeepUnmatched = true;
      p.addParameter('nInputPorts',1);
      p.addParameter('nOutputPorts',1);
      p.addParameter('nInputPortsMin',1);
      p.addParameter('nInputPortsMax',inf);
      p.addParameter('nOutputPortsMin',1);
      p.addParameter('nOutputPortsMax',inf);
      p.addParameter('vectorLength',1);
      p.parse(varargin{:});
      results = namedargs2cell(p.Results);
      obj@runtime.Block(parent,results{:});
      
      % Parse private class members
      p.addParameter('nInputItemsMax',4096);
      p.addParameter('nOutputItemsMax',4096);
      p.parse(varargin{:});
      if p.Results.nInputItemsMax ~= p.Results.nOutputItemsMax
        error('Number of input items must equal number of output items')
      end
      obj.nInputItemsMax = p.Results.nInputItemsMax;
      obj.nOutputItemsMax = p.Results.nOutputItemsMax;
      
      
    end
  end
  
  %% Setters and getters
  methods
    
    function set.nInputItemsMax(obj,n)
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
