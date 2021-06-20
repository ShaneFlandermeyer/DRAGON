% Defines the IO properties of each block. These properties include the maximum
% and minimum number of ports and the size of each input item (in samples)

classdef IOSignature < handle
  
  properties (SetAccess = private)
    minPorts
    maxPorts
    % NOTE: In gnuradio this describes the item SIZE (# items * size in bytes),
    % but matlab handles byte level stuff automatically so only # items is used
    vectorLength
  end
  
  methods
    function obj = IOSignature(varargin)
      p = inputParser;

      % Add parameters and default values
      p.addOptional('minPorts',1);
      p.addOptional('maxPorts',1);
      p.addOptional('vectorLength',1);
      p.parse(varargin{:})
      
      obj.minPorts = p.Results.minPorts;
      obj.maxPorts = p.Results.maxPorts;
      obj.vectorLength = p.Results.vectorLength;
      
      
    end
  end

end
