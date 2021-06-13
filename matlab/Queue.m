% A basic Queue
% 
% PROPERTIES:
% - data: A homogeneous array of data values 
%
% METHODS: 
% - enqueue: Add the newData to the queue as a column vector
% 
% - dequeue: emove nItems data items from the head of the queue
classdef Queue < handle
  
  properties (SetAccess = protected)
    data;
  end
  
  methods 
    function enqueue(obj,newData)
      % Add the newData to the queue as a column vector
      if isempty(obj.data)
        obj.data = newData(:);
      else
        obj.data = [obj.data; newData(:)];
      end
    end
    
    function outData = dequeue(obj,nItems)
      % Remove nItems data items from the head of the queue
      outData = obj.data(1:nItems);
      obj.data = obj.data(nItems+1:end);
    end
      
    
  end
end