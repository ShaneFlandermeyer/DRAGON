% A block that does nothing, used for testing purposes
% This is equivalent to the "nop" block in blocks
% See: https://www.blocks.org/doc/doxygen/classgr_1_1blocks_1_1nop.html
classdef EmptyBlock < blocks.SyncBlock
    
    methods
      function work(obj)
      end
    end
    
end