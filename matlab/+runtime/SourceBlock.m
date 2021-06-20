classdef (Abstract) SourceBlock < runtime.Block
  
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
    
    function obj = SourceBlock(parent,varargin)
      p = inputParser();
      p.KeepUnmatched = true;
      p.addParameter('nInputPorts',0);
      p.addParameter('nOutputPorts',1);
      p.addParameter('nInputPortsMin',0);
      p.addParameter('nInputPortsMax',0);
      p.addParameter('nOutputPortsMin',1);
      p.addParameter('nOutputPortsMax',inf);
      p.addParameter('vectorLength',1);
      p.parse(varargin{:});
      
      if p.Results.nInputPorts ~= 0
        error('Source blocks cannot have input ports')
      end
      
      results = namedargs2cell(p.Results);
      obj@runtime.Block(parent,results{:});
      
      p.addParameter('nOutputItemsMax',4096);
      p.parse(varargin{:})
      obj.nInputItemsMax = 0;
      obj.nOutputItemsMax = p.Results.nOutputItemsMax;
      
      
%       
%       obj.nInputItemsMax = 0;
%       for ii = 1:p.Results.nOutputPorts
%         obj.addOutputPort();
%       end
%       obj.nOutputItemsMax = p.Results.nOutputItemsMax;
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
