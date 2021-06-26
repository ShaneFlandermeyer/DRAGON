classdef LinearFMSource < runtime.SourceBlock
  
  properties (SetAccess = private)
    sampleRate
    bandwidth
    pulsewidth
    dutyCycle
    waveform
    % TODO: Do something with this parameter. For now, only upchirps can be
    % generated
    chirpDirection
  end
  
  properties (Access = private)
    iWaveform
  end
  
  methods
    %% Constructor
    function obj = LinearFMSource(parent,sampleRate,bandwidth,pulsewidth,dutyCycle,varargin)

      % Block can only have one output port (until message functionality is
      % added)
      p = inputParser();
      p.KeepUnmatched = true;
      p.addParameter('nInputPorts',0);
      p.addParameter('nOutputPorts',1);
      p.parse(varargin{:});
      if length(p.Results.nOutputPorts) ~= 1
        error('This block can only have one output port')
      end
      obj@runtime.SourceBlock(parent,varargin{:});
      
      % Waveform parameters
      
      % Validate required positional arguments
      validateattributes(sampleRate,{'numeric'},{'scalar','finite','nonnan','positive'})
      validateattributes(bandwidth,{'numeric'},{'scalar','finite','nonnan','positive'})
      validateattributes(pulsewidth,{'numeric'},{'scalar','finite','nonnan','positive'})
      validateattributes(dutyCycle,{'numeric'},{'scalar','finite','nonnan','nonnegative','>=',0,'<=',1})
      
      obj.sampleRate = sampleRate;
      obj.bandwidth = bandwidth;
      obj.pulsewidth = pulsewidth;
      obj.dutyCycle = dutyCycle;
      obj.iWaveform = 1;
      obj.createWaveform();
    end
    
    function outputItems = work(obj,nOutputItems,inputItems)
      % Continuously output the waveform, one sample at a time. This could
      % easily be vectorized, but it is left in a loop to make it directly
      % portable to C++
      
      outputItems = zeros(nOutputItems,1);
      for iOutput = 1 : nOutputItems
        outputItems(iOutput) = obj.waveform(obj.iWaveform);
        % Increment the waveform sample index, resetting every time the end of
        % the waveform is reached
        obj.iWaveform = mod(obj.iWaveform,length(obj.waveform))+1;
      end

    end
    
    
  end
  
  methods (Access = private)
    function createWaveform(obj)
        % Create the LFM waveform, which is defined to sweep its bandwidth over
        % the time interval [0:tau], where tau is the pulsewidth
        Ts = 1/obj.sampleRate;
        t = (0:Ts:obj.pulsewidth-Ts).';
        obj.waveform = exp(1i*pi*obj.bandwidth*t.*(t/obj.pulsewidth-1));
        
        % Zero pad to the number of samples in a PRI. 
        pri = obj.pulsewidth/obj.dutyCycle;
        nZeros = floor((pri-obj.pulsewidth)*obj.sampleRate);
        obj.waveform = [obj.waveform; zeros(nZeros,1)];
    end
  end
  
end
