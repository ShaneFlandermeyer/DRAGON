classdef (Abstract) Port < handle & matlab.mixin.Heterogeneous
  
  properties (SetAccess = protected)
    connections runtime.Port
    buffer
  end
  
  methods (Abstract)
    connect(obj,port);
    disconnect(obj,port);
  end
  
%   methods (Static)
%     
%     function connect(src,dst)
%       % Validate inputs
%       validateattributes(src,'runtime.OutputPort');
%       validateattributes(dst,'runtime.InputPort');
%       src.addConnection(dst);
%     end
%     
%     function disconnect(src,dst)
%       % Validate inputs
%       validateattributes(src,'runtime.OutputPort');
%       validateattributes(dst,'runtime.InputPort');
%       src.deleteConnection(dst);
%     end
%     
%   end
  
end