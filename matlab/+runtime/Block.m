classdef (Abstract) Block < handle & matlab.mixin.Heterogeneous
  
  properties (SetAccess = private)
    parent runtime.TopBlock
    inputPorts runtime.InputPort   
    outputPorts runtime.OutputPort
    nItemsWritten
    nItemsRead
  end
  
 
  properties (Abstract, Dependent)
    nInputItemsMax
    nOutputItemsMax
  end
  
%   properties (Abstract, Access = protected)
%     d_nInputItemsMax
%     d_nOutputItemsMax
%   end
  
  methods (Abstract)
    outputItems = work(obj,nOutputItemsMax,inputItems)
  end
  
  methods
    
    function obj = Block(parent,varargin)
      validateattributes(parent,{'runtime.TopBlock'},{})
      % Link the block to its parent flowgraph
      obj.parent = parent;
      parent.addBlock(obj);
      
      % Initialize the number of IO items read
      obj.nItemsWritten = 0;
      obj.nItemsRead = 0;
    end
    
    function processData(obj, nItems)
      % Process the data from the block's input ports and output the result to
      % its output port buffers
      % 
      % INPUTS:
      % - nItems: The desired number of input items to process for the given
      %           call to work(). If there is not enough data in the input
      %           buffers or if the block requires less data, the minimum of
      %           those values will be used instead
      % 
      
      
      % Get data from each input port
      inputItems = [];
      for iPort = 1 : length(obj.inputPorts)
        % If data exists at the output of the previous block, transfer it to the
        % input port of this block
        if ~isempty(obj.inputPorts(iPort).connections.buffer)
          obj.inputPorts(iPort).buffer.enqueue(...
          obj.inputPorts(iPort).connections.buffer.dequeue(...
          length(obj.inputPorts(iPort).connections.buffer)));
        end
        nInputItems = min(min(nItems,obj.nInputItemsMax),length(obj.inputPorts(iPort).buffer));
        inputItems(:,iPort) = obj.inputPorts(iPort).buffer.dequeue(nInputItems);
        % TODO: This might cause problems if the number of inputs is not the
        % same from each port (for example, if one port does not have enough
        % data). Will probably need to find the minimum length of all port
        % buffers for nInputItems assignment statement
        obj.nItemsRead = obj.nItemsRead + nInputItems;
        
      end
      
      % Process input data
      outputItems = obj.work(obj.nOutputItemsMax,inputItems);
      
      for iPort = 1 : length(obj.outputPorts)
        % Send the result of the work function to the output buffer(s)
        obj.outputPorts(iPort).buffer.enqueue(outputItems(:,iPort));
        obj.nItemsWritten = obj.nItemsWritten + length(outputItems);
        % TODO: This function currently recursively calls itself for the next
        % block in the flowgraph, but it should probably be handled somewhere
        % else so 
        for iConnection = 1 : length(obj.outputPorts(iPort).connections)
          obj.outputPorts(iPort).connections(iConnection).parent.processData(nItems);
        end
      end
      
      
    end
    
    function addInputPort(obj)
      % Add a new input port to the block
      if isa(obj,'runtime.SourceBlock')
        % TODO: Make this an exception
        error('Source Blocks cannot instantiate input ports (messages input ports not supported)')
      end
      
      if isempty(obj.inputPorts)
        obj.inputPorts = runtime.InputPort(obj);
      else
        obj.inputPorts = [obj.inputPorts; runtime.InputPort(obj)];
      end
      
    end
    
    function addOutputPort(obj)
      % Add a new output port to the block
      if isa(obj,'runtime.SinkBlock')
        % TODO: Make this an exception
        error('Sink Blocks cannot instantiate output ports (messages output ports not supported)')
      end
      
      if isempty(obj.outputPorts)
        obj.outputPorts = runtime.OutputPort(obj);
      else
        obj.outputPorts = [obj.outputPorts; runtime.OutputPort(obj)];
      end
        
    end
    
    function deletePort(obj,port)
      % Delete an existing port from the block
      % 
      % INPUT: A reference to the runtime.Port object to be deleted
      
      idx = find(obj.inputPorts == port);
      obj.inputPorts(idx) = [];

      idx = find(obj.outputPorts == port);
      obj.outputPorts(idx) = [];
    end
    
    
  end
  
end
