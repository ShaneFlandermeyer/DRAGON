classdef Add < runtime.SyncBlock

  methods
    
    %% Constructor
    function obj = Add(parent,varargin)
      p = inputParser();
      p.KeepUnmatched = true;
      p.addParameter('nInputPorts',2);
      p.parse(varargin{:});
      
      validateattributes(p.Results.nInputPorts,{'numeric'},{'finite','scalar','>',1})
      
      % If the number of input ports has not been specified by the user, 
      if ~isempty(find(strcmp(p.UsingDefaults,'nInputPorts'), 1))
        varargin{end+1} = 'nInputPorts';
        varargin{end+1} = p.Results.nInputPorts;
      end
      
      obj@runtime.SyncBlock(parent,varargin{:})
    end
    
    function outputItems = work(obj,nOutputItemsMax,inputItems)
      
      outputItems = sum(inputItems,2);
      
    end
    
  end
  
  
end
