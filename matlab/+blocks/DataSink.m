% A Sink block that stores its input items in a member variable.
% Very helpful for debugging flowgraph outputs

classdef DataSink < runtime.SinkBlock

  properties (SetAccess = private)
    data;
  end
  
  methods
    
    function obj = DataSink(parent,varargin)
      obj@runtime.SinkBlock(parent,varargin{:});
    end
    
    function outputItems = work(obj,nOutputItemsMax,inputItems)
      % Do nothing
      outputItems = [];
      if isempty(obj.data)
        obj.data = inputItems(:);
      else
        obj.data = [obj.data;inputItems(:)];
      end
    end
    
  end
end
