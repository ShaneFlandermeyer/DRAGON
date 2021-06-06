classdef (Abstract) Block < handle & matlab.mixin.Heterogeneous
  
  methods (Abstract)
    work(obj)
  end
  
  
  methods (Static)
    
    function connect(src,srcPortIdx,dst,dstPortIdx)
      src.outputPorts(srcPortIdx).addConnection(dst.inputPorts(dstPortIdx));
    end
    
    function disconnect(src,srcPortIdx,dst,dstPortIdx)
      src.outputPorts(srcPortIdx).deleteConnection(dst.inputPorts(dstPortIdx));
    end
    
%     function connect(src,srcPortIdx,dst,dstPortIdx)
%       % Connects the given output port of the block src to the given input
%       % port of the block dst
%       
%       % Connect the source's output port to the destination block
%       if srcPortIdx > length(src.outputPorts)
%         % Port number exceeds the current number of ports; append it to the
%         % end of the list
%         src.outputPorts = [src.outputPorts; dst];
%       elseif srcPortIdx < 1
%         % Port number specified is less than the smallest possible index;
%         % add it as the first port
%         src.outputPorts = [dst; src.outputPorts];
%       else
%         % Replacing an existing connection, so we can index into it like a
%         % normal array
%         src.outputPorts(srcPortIdx) = dst;
%       end
%       
%       % Connect the destination's input port to the source block
%       if dstPortIdx > length(dst.inputPorts)
%         % Port number exceeds the current number of ports; append it to the
%         % end of the list
%         dst.inputPorts = [dst.inputPorts; src];
%       elseif srcPortIdx < 1
%         % Port number specified is less than the smallest possible index;
%         % add it as the first port
%         dst.inputPorts = [src; dst.inputPorts];
%       else
%         % Replacing an existing connection, so we can index into it like a
%         % normal array
%         dst.inputPorts(dstPortIdx) = src;
%       end
%       
% %       % Reshape the output to a column vector
% %       src.outputPorts = src.outputPorts(:);
% %       dst.inputPorts = dst.inputPorts(:);
%       
%     end
%     
%     function disconnect(src,srcPortIdx,dst,dstPortIdx)
%       % If either of the indices are out of the array bounds, there's
%       % nothing to delete
%       if (srcPortIdx < 1 || srcPortIdx > length(src.outputPorts))
%         return
%       end
%       if (dstPortIdx < 1 || dstPortIdx > length(dst.inputPorts))
%         return
%       end
%       src.outputPorts(srcPortIdx) = [];
%       dst.inputPorts(dstPortIdx) = [];
%     end
%     
%   end
  
end