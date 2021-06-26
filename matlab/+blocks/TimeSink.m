classdef TimeSink < runtime.SinkBlock
  
  properties
    % TODO: Implement me!
  end

  methods

    function obj = TimeSink(parent,varargin)
      obj@runtime.SinkBlock(parent,varargin{:});
    end
    
    function outputItems = work(obj,nOutputItems,inputItems)
      % TODO: This will cause problems if the flowgraph contains more than one
      % plotting blocks
      figure(1)
      plot(inputItems,'Linewidth',2);
      drawnow % Continuously update the plot
      outputItems = [];
      
    end
    
  end % methods
  
end % class
