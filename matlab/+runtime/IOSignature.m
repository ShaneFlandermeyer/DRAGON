classdef IOSignature < handle
  % TODO: I don't think this is really necessary for a matlab
  % implementation, but I'm leaving it just in case

  properties (SetAccess = private)
    minPorts
    maxPorts
    itemSize
  end
  
  methods
    function obj = IOSignature(varargin)
      p = inputParser;
      
      % Validation functions
      validNonnegNum = @(x)validateattributes(x,{'numeric'},...
            {'nonempty','integer','nonnegative'});
      validPosNum = @(x)validateattributes(x,{'numeric'},...
            {'nonempty','integer','positive'});
      
      % Add parameters and default values
      p.addOptional('minPorts',1,validNonnegNum);
      p.addOptional('maxPorts',1,validNonnegNum);
      p.addOptional('itemSize',1,validPosNum);
      p.parse(varargin{:})
      
      obj.minPorts = p.Results.minPorts;
      obj.maxPorts = p.Results.maxPorts;
      obj.itemSize = p.Results.itemSize;
      
      
    end
  end

end