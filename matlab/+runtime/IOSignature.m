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
    %% Constructor
    function obj = IOSignature(varargin)
      
      % Validation functions
      validNonnegativeNumber = @(x) isnumeric(x) && isscalar(x) && (x >= 0);
      validPositiveNumber = @(x) isnumeric(x) && isscalar(x) && (x > 0);
      
      % Parse inputs
      p = inputParser;
      p.addOptional('minPorts',validNonnegativeNumber);
      p.addOptional('maxPorts',validNonnegativeNumber);
      p.addOptional('vectorLength',validPositiveNumber);
      p.parse(varargin{:})
      
      % Assign parsed values
      obj.minPorts = p.Results.minPorts;
      obj.maxPorts = p.Results.maxPorts;
      obj.vectorLength = p.Results.vectorLength;
      
      
    end
  end

end
