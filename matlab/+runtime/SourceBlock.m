classdef (Abstract) SourceBlock < runtime.Block
  
  properties
    % TODO: Handle these more dynamically (probably from the Block class)
    nInputItems = []
    nOutputItems = 4096
  end
  
  %% Abstract methods
  methods (Abstract)
    work(obj)
  end
  
  %% Constructor
  methods
    
    function obj = SourceBlock()
      % Initialize the block to have 1 output port
      obj.addOutputPort();
    end
    
  end

  
  
end