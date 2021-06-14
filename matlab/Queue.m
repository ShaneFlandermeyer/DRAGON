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
    
    function out = pop(obj)
      if isempty(obj.data)
        out = [];
      else
        out = obj.data(1);
        obj.data = obj.data(2:end);
      end
    end
    
    function out = peek(obj)
      if isempty(obj.data)
        out = [];
      else
        out = obj.data(1);
      end
    end
    
    function push(obj,newItem)
      obj.data = [obj.data; newItem(:)];
    end
    
    function len = length(obj)
      len = length(obj.data);
    end
    
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