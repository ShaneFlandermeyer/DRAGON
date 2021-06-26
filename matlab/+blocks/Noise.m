classdef Noise < runtime.SyncBlock
  
  properties (SetAccess = private)
    power;
  end
  
  methods
    function obj = Noise(parent,power,varargin)
      % For this block, the number of input ports must be the same as the number
      % of output ports. We can pass everything else to the superclass
      % constructors
      p = inputParser();
      p.KeepUnmatched = true;
      p.addParameter('nInputPorts',1);
      p.addParameter('nOutputPorts',1);
      p.parse(varargin{:});
      if p.Results.nInputPorts ~= p.Results.nOutputPorts
        error('The number of input ports must equal the number of output ports')
      end
      obj@runtime.SyncBlock(parent,varargin{:});
      
      validateattributes(power,{'numeric'},{'scalar','finite','nonnan','nonnegative'})
      obj.power = power;
    end
    
    function outputItems = work(obj,nOutputItems,inputItems)
      % Inject additive white gaussian noise (AWGN) into the input signal, where
      % the noise in each input port is assumed to be mutually uncorrelated
      
      noise = zeros(size(inputItems));
      nInputItems = size(inputItems,1);
      if isreal(inputItems)
        % If the signal is real, noise does not split between any channels
        for iPort = 1 : size(inputItems,2)
          noise(:,iPort) = sqrt(obj.power)*randn(nInputItems,1);
        end
      else
        % Split the noise between the real and imaginary channels
        for iPort = 1 : size(inputItems,2)
          noise(:,iPort) = sqrt(obj.power/2)*...
            (randn(nInputItems,1) + 1i*randn(nInputItems,1));
        end
      end
      
      outputItems = inputItems + noise;
          
    end

  end
end
