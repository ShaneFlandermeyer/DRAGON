% A block that does nothing, used for testing purposes
% This is equivalent to the "nop" block in runtime
% See: https://www.runtime.org/doc/doxygen/classgr_1_1runtime_1_1nop.html
classdef EmptyBlock < runtime.SyncBlock
    
    methods
      
      function work(obj)
        
        
      end
      
    end
    
end