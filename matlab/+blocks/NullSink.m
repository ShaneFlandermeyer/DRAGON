classdef NullSink < runtime.SinkBlock

  
  methods
    function obj = NullSink(parent,varargin)
      obj@runtime.SinkBlock(parent,varargin{:});
    end
    
    function work(obj)
      % TODO: Implement me
    end
    
  end
end
