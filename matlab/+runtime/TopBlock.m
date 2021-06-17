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
      % Use graphviz to show the current flowgraph for this block
      
      % Create graphviz instructions
      command = 'echo ''strict digraph { rankdir=LR node [shape=box]';
      for iBlock = 1:length(obj.blocks)
        % Only print the graph path if 
        if isa(obj.blocks(iBlock),'runtime.SinkBlock')
          command = [command obj.printGraph(obj.blocks(iBlock))];
        end
      end
      
      command = [command '}'' | dot -Tpng > output.png'];
      
      % Use imshow on the opened png
      
      system(command)
      imshow('output.png')
    end
    
    % TODO:
    % - Decide if this should be static and whether or not the user should be
    %   able to call it
    % - Decide if the tempStr parameter should become some form of a persistent
    %   variable
    % - Update this function to use a "name" parameter for each block rather
    %   than just the block name
    function outStr = printGraph(obj,block,tempStr)
      % Print the graph associated with the input block, with the output given
      % in the dot language. This block will eventually be used to generate a
      % graphviz output of the flowgraph
      
      outStr = '';
      name = split(class(block),'.');
      % If inStr is not specified, this is the first call from an external
      % function call. The block is then assumed to be a sink block
      if nargin == 2
        tempStr = [name{2}];
      else
        tempStr = [name{2} ' -> ' tempStr];
      end
      
      % Add a semicolon to the end of every sink block name to mark the end of
      % the graph path
      if isa(block,'runtime.SinkBlock')
        tempStr = [tempStr ';'];
      end
      
      
      % Append all children graphs to the output
      for iPort = 1 : length(block.inputPorts)
        outStr = [obj.printGraph(block.inputPorts(iPort).connections.parent,tempStr) outStr];
      end
      
      % If we've reached a source block, the graph ends 
      if isa(block,'runtime.SourceBlock')
        outStr = tempStr;
      end
      
    end
    
    
  end
end
