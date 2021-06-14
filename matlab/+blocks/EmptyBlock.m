% A block that does nothing, used for testing purposes
% This is equivalent to the "nop" block in runtime
% See: https://www.runtime.org/doc/doxygen/classgr_1_1runtime_1_1nop.html
classdef EmptyBlock < runtime.SyncBlock
    
    methods
      % TODO: This should not stay public once the flowgraph structure is
      % well defined
      function work(obj)
        % TODO: Data passing should not be done inside the work function
        obj.inputPorts(1).buffer.push(obj.inputPorts(1).connections(1).buffer.pop(obj.nInputItems));
        inputData = obj.inputPorts(1).buffer.pop(obj.nInputItems);
        obj.outputPorts(1).buffer.push(inputData);
      end
      
    end
    
    methods 
      function obj = EmptyBlock(parent,varargin)
        obj@runtime.SyncBlock(parent,varargin{:})
      end
    end
end