classdef Scheduler < handle
  
  % Convert flowgraph to flat,acyclic, directed graph
  % Find sources and call then
  % Ripple data through the tree-ish structure
  
  properties (SetAccess = private)
    
    eventQueue (1,1) Queue
    
  end
  
  methods
    
    function schedule(obj,block)
      % Find the source port(s) associated with the given block
      
      % If this block is a source block, add it to the queue and return
      if isa(block,'runtime.SourceBlock')
        obj.eventQueue.push(block);
        return
      end
      
      % Current block is not a source block. Keep going through its input
      % until all source blocks are found
      for iPort = 1:length(blocks.inputPort)
        
        
      end
      
    end
    
    function execute(obj)

    end
    
  end
  
end