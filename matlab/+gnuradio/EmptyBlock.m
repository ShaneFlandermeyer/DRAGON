% A block that does nothing, used for testing purposes
% This is equivalent to the "nop" block in gnuradio
% See: https://www.gnuradio.org/doc/doxygen/classgr_1_1blocks_1_1nop.html
classdef EmptyBlock < gnuradio.SyncBlock
    
    methods
      function work(obj)
      end
    end
    
end