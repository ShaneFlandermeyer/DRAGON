classdef (Abstract) SyncBlock < runtime.Block
  
  properties
    % TODO: Do an error check to ensure that these are equal. When one is
    % updated, the other should also be updated
    nInputItems = 4096;
    nOutputItems = 4096;
  end
  
  
  methods (Abstract)
    work(obj)
  end
  
end