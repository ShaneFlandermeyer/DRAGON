classdef TopBlock < handle
  
  % Idea: In the blocks constructor, give an option for the block's parent
  % flowgraph, which is an instance of this class. When a block object is
  % created, its parent is given as this and its name is added to the
  % "blocks" property
  properties
    blocks runtime.Block
    sources runtime.Block
    sinks runtime.Block
    
  end
  
  methods
    
    function addBlock(obj,block)
      
      % Add a block to the list
      if isempty(block)
        obj.blocks = block;
      else
        obj.blocks = [obj.blocks; block];
      end
      
      if isa(block,'runtime.SourceBlock')
        obj.sources = [obj.sources; block];
      elseif isa(block,'runtime.SinkBlock')
        obj.sinks = [obj.sinks;block];
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
    
    function run(obj,nItems)
      % A blocking start(N) (calls start then wait)
      
      % If the number of items is not specified, the flowgraph will run
      % continuously (equivalent to calling start)
      % TODO: This is not working well for real-time plotting
      if nargin == 1
        nItems = inf;
      end
      
      nItems = floor(nItems);
      
      isDone = false(length(obj.sources),1);
      while ~all(isDone)
        % Loop through all sources and process nItems data items. If the block
        % input item limit is less than this, that is the number of items that
        % will be processedd
        for iSource = 1 : length(obj.sources)
          nItemsWritten = obj.sources(iSource).nItemsWritten;
          if nItemsWritten >= nItems
            isDone(iSource) = true;
            continue
          end
          if isinf(nItems)
            obj.sources(iSource).processData(nItems);
          else
            obj.sources(iSource).processData(nItems-nItemsWritten);
          end
        end
      end
      
    end
    
    function lock(obj)
      % TODO: locks the flowgraph so we can reconfigure it
    end
    
    function unlock(obj)
      % TODO: unlocks and restarts the flowgraph
    end
    
    % Connect the source block output port at index sourcePort to the sink block
    % input port at index sinkPort
    function connect(~, source, sourcePort, sink, sinkPort)
      source.outputPorts(sourcePort).connect(sink.inputPorts(sinkPort));
    end
    
    function showGraph(obj)
      % Use graphviz to show the connections of the current flowgraph
      % NOTE: Only works for bash!
      
      % Like gnuradio, the graph flows from left to right and each node is
      % represented by a box
      command = 'echo ''strict digraph { rankdir=LR node [shape=box]';
      % Clear the unique name list
      runtime.TopBlock.addUniqueName([],true);
      for iBlock = 1:length(obj.blocks)
        % If the block is a sink block, show the path to all its children. This
        % does no error checking to verify if the connections are legitimate.
        % That should probably be handled in the block class itself
        if isa(obj.blocks(iBlock),'runtime.SinkBlock')
          command = [command obj.printGraph(obj.blocks(iBlock))];
          command = [command runtime.TopBlock.getNodeLabels];
        end
      end
      
      % Output the graph as a png
      command = [command '}'' | dot -Tpng > output.png'];
      system(command);
      % Plot the graph
      imshow('output.png')
      % Remove the intermediate graph file
      system('rm output.png');
    end
    
    function depthFirstPath = printGraph(obj,block)
      % Return a string representing the path taken for a depth first traversal
      % of the tree rooted at the given block. Each tree node is given
      % a unique identifier of the form "blockN" for some integer N, which can
      % be used to plot the tree using graphviz. Branching tree paths are
      % separated by semicolons
      %
      % INPUTS:
      % - obj: The TopBlock object containing the blocks in question
      %        TODO: This method will probably be made static soon, removing
      %              this argument
      % - block: The block to be used as the root of the tree
      %
      % OUTPUTS:
      % - depthFirstPath: An ASCII drawing of the traversal of the tree, with
      % arrows pointing from leaf nodes to the root
      
      persistent traversalPath
      depthFirstPath = [];
      
      blockNames = runtime.TopBlock.addUniqueName(block,false);
      nNames = length(blockNames);
      blockID = ['block',num2str(nNames)];
      
      % If the current block is a sink block, don't add an output arrow to it
      if isa(block,'runtime.SinkBlock')
        traversalPath = [blockID];
      else
        traversalPath = [blockID ' -> ' traversalPath];
      end
      
      % Add a semicolon to the end of every sink block name to mark the end of
      % the graph path
      if isa(block,'runtime.SinkBlock')
        traversalPath = [traversalPath ';'];
      end
      
      currentPath = traversalPath;
      
      % Append all children paths to the output
      for iPort = 1 : length(block.inputPorts)
        depthFirstPath = [obj.printGraph(block.inputPorts(iPort).connections.parent) depthFirstPath];
        % Discard the changes made to the persistent variable in the function
        % calls above
        traversalPath = currentPath;
      end
      
      % If we've reached a source block, the graph ends
      if isa(block,'runtime.SourceBlock')
        depthFirstPath = traversalPath;
      end
      
    end
    
    
  end
  
  methods (Static, Access = private)
    
    function out = addUniqueName(block,reset)
      % Append the name of the input block to a list of unique block names.
      % Used to visualize the flowgraph with graphviz.
      %
      % INPUTS:
      % - block: The runtime.Block object whose name is to be added to the list
      % - clear: If this value is true, the name array is emptied
      %
      % OUTPUTS:
      % - out: The name array after the new block name has been appended
      %
      % TODO:
      % - Give the block objects a "name" parameter, then use that instead
      %   of the name of the block class
      % - Use inputParser object to be smarter about the inputs
      
      % Array of names
      persistent names;
      
      
      % No arguments specified. Return the name array
      if nargin == 0
        out = names;
        return
      end
      
      % Block name given, assume the user wants to append it to the list
      if nargin == 1
        reset = false;
      end
      
      % Reset operation specified; clear the array and return
      if reset
        clear names
        out = [];
        return
      end
      
      % User wants to add block to the list. Add the block name, but not its
      % qualifier
      str = split(class(block),'.');
      names{end+1} = str{end};
      out = names;
      
    end
    
    function labelCommand = getNodeLabels()
      % Output the graphviz command that allows us to convert from the abstract
      % numbered block names to the actual names of the blocks being used
      
      names = runtime.TopBlock.addUniqueName;
      labelCommand = [];
      for iName = 1:length(names)
        labelCommand = [labelCommand 'block' num2str(iName) '[label="' names{iName} '"];'];
      end
      
    end
    
  end
  
end % class
