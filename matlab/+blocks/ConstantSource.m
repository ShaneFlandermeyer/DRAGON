classdef ConstantSource < runtime.SourceBlock
  
  properties
    constant;
  end
  
  methods

    function work(obj)
      
      for iPort = 1:length(obj.outputPorts)
        % Push the constant value to the output buffer
      end
      
    end
    
  end
  
end