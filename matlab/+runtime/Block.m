classdef (Abstract) Block < handle & matlab.mixin.Heterogeneous
  
  properties (SetAccess = protected)
    parent runtime.TopBlock
    inputPorts runtime.InputPort
    outputPorts runtime.OutputPort
    nItemsWritten
    nItemsRead
  end
  
 
  properties (Abstract, Dependent)
    nInputItems
    nOutputItems
  end
  
  properties (Abstract, Access = protected)
    d_nInputItems
    d_nOutputItems
  end
  
  methods (Abstract)
    work(obj)
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
