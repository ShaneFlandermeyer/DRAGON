classdef (Abstract) SourceBlock < runtime.Block
  
  methods
    
    function obj = SourceBlock()
      obj.addOutputPort();
    end
    
  end

  methods (Abstract)
    work(obj)
  end
  
end