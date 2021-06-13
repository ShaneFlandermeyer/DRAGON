classdef ConstantSource < runtime.SourceBlock
  
  properties
    constant;
  end
  
  methods
    
    function obj = ConstantSource(constant)
      % TODO: Use inputparser and varargin
      obj.constant = constant;
    end
    
    function work(obj)
      
      % Push the constant value to the output buffer
      outData = repmat(obj.constant,obj.nOutputItems,1);
      for iPort = 1:length(obj.outputPorts)
        obj.outputPorts(iPort).buffer.enqueue(outData);
      end
      
    end
    
  end % methods
  
end % class