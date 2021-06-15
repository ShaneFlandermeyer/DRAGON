classdef TopBlock < handle
  
  % Idea: In the blocks constructor, give an option for the block's parent
  % flowgraph, which is an instance of this class. When a block object is
  % created, its parent is given as this and its name is added to the
  % "blocks" property
  properties 
    blocks runtime.Block
    
  end
  
  methods
    
    function addBlock(obj,block)
      
      % Add a block to the list 
      if isempty(block)
        obj.blocks = block;
      else
        obj.blocks = [obj.blocks; block];
      end
      
    end
    
    function start(obj,n)
      % TODO: starts the flow graph running with N as the maximum noutput_items any block can receive.
    end
    
    function stop(obj)
      % TODO: stops the top block
    end
    
    function wait(obj)
      % TODO: blocks until top block is finished
    end
    
    function run(obj,n)
      % TODO: a blocking start(N) (calls start then wait)
    end
    
    function lock(obj)
      % TODO: locks the flowgraph so we can reconfigure it
    end
    
    function unlock(obj)
      % TODO: unlocks and restarts the flowgraph
    end
    
    function showGraph(obj)
      % TODO: Use graphviz to show the current flowgraph for this block
      
      % Create graphviz instructions
      str = ['echo ''digraph {a -> b'];
      str = [str '}'' | dot -Tpng > output.png'];
      % Get dot strings, get dot labels, etc
      
      % Write to a .gv file
      
      % Load .gv file and output to a png
      
      % Use imshow on the opened png
      
      system(str)
      imshow('output.png')
    end
    
    function outStr = printGraph(obj)
      block = obj.blocks(2);
      while ~isempty(block.inputPorts)
        block = block.inputPorts(1).parent;
      end
        
    end
    
  end
end
