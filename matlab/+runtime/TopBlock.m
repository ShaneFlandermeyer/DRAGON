classdef TopBlock < handle

  properties (SetAccess = private)
    blocks 
    sources 
    sinks 
  end
  
  methods
    
    function id = addBlock(obj,block)
      % Add the input block to the TopBlock/flowgraph. If the block is a
      % source or a sink, it is added to the TopBlock's source/sink list
      % accordingly
      % 
      % INPUTS:
      % - block: The block object to be added
      %
      % OUTPUTS: The unique ID number assigned to the block
      
      if ~isa(block,'runtime.Block')
        error('Only runtime.Block objects can be added to a flowgraph')
      end
      
      % Assign the block an ID
      id = length(obj.blocks);
      
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
      % starts the flow graph running with N as the maximum noutput_items any block can receive.
      if nargin == 2
        validateattributes(n,{'numeric'},{'scalar','positive','real','integer'});
        for iBlock = 1 : length(obj.blocks)
          obj.blocks(iBlock).nOutputItemsMax = n;
        end
      end
      obj.run();
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
      
      for iBlock = 1 : length(obj.blocks)
        % Flush the rest of the buffers
        
        if isa(obj.blocks(iBlock),'runtime.SourceBlock')
          % Source blocks have already produced all their inputs in the above loop
          continue
        end
        
        while any(obj.blocks(iBlock).hasInputItems)
          % Process any leftover inputs in the block
          obj.blocks(iBlock).processData(length(obj.blocks(iBlock).inputPorts.buffer));
        end
      end

    end

    function connect(~, source, sourcePort, sink, sinkPort)
      % Connect the source block output port at index sourcePort to the sink block
      % input port at index sinkPort
      source.outputPorts(sourcePort).connect(sink.inputPorts(sinkPort));
    end
    
    function showGraph(obj)
      % Use graphviz to show the connections of the current flowgraph
      
      if ispc
        error('This function currently only works in Linux environments')
      end
        
      
      % Like gnuradio, the graph flows from left to right and each node is
      % represented by a box
      command = 'echo ''strict digraph { rankdir=LR node [shape=box]';
      for iBlock = 1:length(obj.blocks)
        % If the block is a sink block, show the path to all its children. This
        % does no error checking to verify if the connections are legitimate.
        % That should probably be handled in the block class itself
        if isa(obj.blocks(iBlock),'runtime.SinkBlock')
          command = [command obj.printGraph(obj.blocks(iBlock))];
          command = [command obj.getNodeLabels];
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

      blockID = ['block',num2str(block.uid)];
      % If the current block is a sink block, don't add an output arrow to it
      if isa(block,'runtime.SinkBlock')
        traversalPath = blockID;
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
  
  methods (Access = private)
    function labelCommand = getNodeLabels(obj)
      % Output the graphviz command that allows us to convert from the abstract
      % numbered block names to the actual names of the blocks being used
      
      labelCommand = [];
      for iBlock = 1 : length(obj.blocks)
        name = split(class(obj.blocks(iBlock)),'.');
        name = name{end};
        labelCommand = [labelCommand 'block' num2str(obj.blocks(iBlock).uid) '[label="' name '"];'];
      end
    end
  end
  
end % class
