classdef (Abstract) Block < handle & matlab.mixin.Heterogeneous
  
  properties (SetAccess = private)
    parent (1,1) runtime.TopBlock
    inputPorts runtime.InputPort   
    outputPorts runtime.OutputPort
    inputSignature (1,1) runtime.IOSignature
    outputSignature (1,1) runtime.IOSignature
    nItemsWritten
    nItemsRead
  end
  
  properties (Abstract, Dependent)
    nInputItemsMax
    nOutputItemsMax
  end

  methods (Abstract)
%     outputItems = work(obj,nOutputItemsMax,inputItems)
    outputItems = general_work(obj,nInputItems,nOutputItems,inputItems,outputItems);
  end
  
  methods
    
    function obj = Block(parent,varargin)
      % Parse inputs
      validNonnegativeNumber = @(x) isnumeric(x) && isscalar(x) && (x >= 0);
      validPositiveNumber = @(x) isnumeric(x) && isscalar(x) && (x > 0);
      
      p = inputParser();
      p.addParameter('nInputPorts',0,validNonnegativeNumber);
      p.addParameter('nOutputPorts',0,validNonnegativeNumber);
      p.addParameter('nInputPortsMin',0,validNonnegativeNumber);
      p.addParameter('nInputPortsMax',inf,validNonnegativeNumber);
      p.addParameter('nOutputPortsMin',0,validNonnegativeNumber);
      p.addParameter('nOutputPortsMax',inf,validNonnegativeNumber);
      p.addParameter('vectorLength',1,validPositiveNumber);
      p.parse(varargin{:})
      
      % Link the block to its parent flowgraph
      validateattributes(parent,{'runtime.TopBlock'},{})
      obj.parent = parent;
      parent.addBlock(obj);
      
      % Create the IO signatures for each port
      obj.inputSignature = runtime.IOSignature(minPorts=p.Results.nInputPortsMin,...
        maxPorts=p.Results.nInputPortsMax,vectorLength=p.Results.vectorLength);
      
      obj.outputSignature = runtime.IOSignature(minPorts=p.Results.nOutputPortsMin,...
        maxPorts=p.Results.nOutputPortsMax,vectorLength=p.Results.vectorLength);
      
      % Add input and output ports
      for iPort = 1 : p.Results.nInputPorts
        obj.addInputPort();
      end
      
      for iPort = 1 : p.Results.nOutputPorts
        obj.addOutputPort();
      end
      
      % Initialize the number of IO items read
      obj.nItemsWritten = zeros(p.Results.nOutputPorts,1);
      obj.nItemsRead = zeros(p.Results.nInputPorts,1);
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
      
      
      % Get the length of the inputItems array for each input port. If the input
      % array buffers have different sizes, the smallest size is used as the
      % input size
      minBufferSize = inf;
      for iPort = 1 : length(obj.inputPorts)
        % If data exists at the output of the previous block, transfer it to the
        % input port of this block
        if ~isempty(obj.inputPorts(iPort).connections.buffer)
          obj.inputPorts(iPort).buffer.enqueue(...
          obj.inputPorts(iPort).connections.buffer.dequeue(...
          length(obj.inputPorts(iPort).connections.buffer)));
        end
        % Compute the minimum buffer size
        minBufferSize = min(minBufferSize,length(obj.inputPorts(iPort).buffer));
      end
      nInputItems = min(min(nItems,obj.nInputItemsMax),minBufferSize);
      inputItems = zeros(nInputItems,length(obj.inputPorts));
      
      % Get data from each input port
      for iPort = 1 : length(obj.inputPorts)
        if isempty(inputItems)
          return;
        end
          inputItems(:,iPort) = obj.inputPorts(iPort).buffer.peek(nInputItems);
        
      end
      
      % Process input data
      nOutputItems = min(nItems,obj.nOutputItemsMax);
      outputItems = obj.general_work(nInputItems,nOutputItems,inputItems);
      
      for iPort = 1 : length(obj.outputPorts)
        % Send the result of the work function to the output buffer(s)
        obj.outputPorts(iPort).buffer.enqueue(outputItems(:,iPort));
        obj.nItemsWritten(iPort) = obj.nItemsWritten(iPort) + length(outputItems);
        % TODO: This function currently recursively calls itself for the next
        % block in the flowgraph, but it should probably be handled somewhere
        % else 
        for iConnection = 1 : length(obj.outputPorts(iPort).connections)
          obj.outputPorts(iPort).connections(iConnection).parent.processData(nItems);
        end
      end
      
      
    end
    
    function consume(obj,nItems,iPort)
      obj.inputPorts(iPort).buffer.dequeue(nItems);
      % TODO: Should nItemsRead track the total number of items read or the
      % number of items read per port?
      obj.nItemsRead(iPort) = obj.nItemsRead(iPort) + nItems;
    end
    
    function deletePort(obj,port)
      % Delete an existing port from the block
      % 
      % INPUT: A reference to the runtime.Port object to be deleted
      
      % TODO: Do nothing here if it causes the number of ports to violate the IO
      % signature
      
      idx = find(obj.inputPorts == port);
      obj.inputPorts(idx) = [];

      idx = find(obj.outputPorts == port);
      obj.outputPorts(idx) = [];
    end
    
    
  end
  
  methods (Access = protected)
    function addInputPort(obj)
      % Add a new input port to the block
      
      if isa(obj,'runtime.SourceBlock')
        % TODO: Make this an exception
        error('Source Blocks cannot instantiate input ports (messages input ports not supported)')
      end
      
      % Don't add more ports than the IO signature allows
      if length(obj.inputPorts) >= obj.inputSignature.maxPorts
        return
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
      
      % Don't add more ports than the IO signature allows
      if length(obj.outputPorts) >= obj.outputSignature.maxPorts
        return
      end
      
      if isempty(obj.outputPorts)
        obj.outputPorts = runtime.OutputPort(obj);
      else
        obj.outputPorts = [obj.outputPorts; runtime.OutputPort(obj)];
      end
        
    end
  end
  
end
