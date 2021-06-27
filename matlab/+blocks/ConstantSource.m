classdef ConstantSource < runtime.SourceBlock
  
  properties
    constant;
  end

  methods
    
    function obj = ConstantSource(parent,constant,varargin)
      obj@runtime.SourceBlock(parent,varargin{:});
      validateattributes(constant,{'numeric'},{'scalar','finite','nonnan'})
      obj.constant = constant;
    end
    
    function outputItems = work(obj,nOutputItems,inputItems)
      
      % enqueue the constant value to the output buffer
      outputItems = repmat(obj.constant,nOutputItems,length(obj.outputPorts));
      
    end
    
  end % methods
  
end % class
