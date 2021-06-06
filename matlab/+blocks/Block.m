classdef (Abstract) Block < handle & matlab.mixin.Heterogeneous
  
  methods (Abstract)
    work(obj)
  end
  
  methods (Static)
    
    function connect(src,srcPortIdx,dst,dstPortIdx)
      % Connect src block port srcPortIdx to dst block port dstPortIdx
      
      if srcPortIdx > length(src.outputPorts)
        % Port number exceeds the current number of ports; add another port
        % to the end of the list
        src.outputPorts = [src.outputPorts; blocks.OutputPort];
        srcPortIdx = length(src.outputPorts);
      elseif srcPortIdx < 1
        % Port number specified is less than the smallest possible index;
        % add it as the first port
        src.outputPorts = [blocks.OutputPort; src.outputPorts];
        srcPortIdx = 1;
      end
      
      if dstPortIdx > length(dst.inputPorts)
        % Port number exceeds the current number of ports; add another port
        % to the end of the list
        dst.inputPorts = [dst.inputPorts; blocks.InputPort];
        dstPortIdx = length(dst.inputPorts);
      elseif dstPortIdx < 1
        % Port number specified is less than the smallest possible index;
        % add it as the first port
        dst.inputPorts = [blocks.InputPort; dst.inputPorts];
        dstPortIdx = 1;
      end
      
      % Connect the ports
      src.outputPorts(srcPortIdx).addConnection(dst.inputPorts(dstPortIdx));
    end
    
    function disconnect(src,srcPortIdx,dst,dstPortIdx)
      % Connect src block port srcPortIdx to dst block port dstPortIdx
      
      % If either of the indices are out of the array bounds, there's
      % nothing to delete
      if (srcPortIdx < 1 || srcPortIdx > length(src.outputPorts))
        return
      end
      if (dstPortIdx < 1 || dstPortIdx > length(dst.inputPorts))
        return
      end
      
      % Disconnect the ports
      src.outputPorts(srcPortIdx).deleteConnection(dst.inputPorts(dstPortIdx));
    end

  end
  
end