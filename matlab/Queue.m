% A basic Queue
% 
% PROPERTIES:
% - data: A homogeneous array of data values 
%
% METHODS: 
% - enqueue: Add the newData to the queue as a column vector
% 
% - dequeue: emove nItems data items from the head of the queue
classdef Queue < handle & matlab.mixin.Heterogeneous
  
  properties (SetAccess = protected)
    data;
  end
  
  methods 
    
    function out = dequeue(obj,nItems)
      % Remove and return nItems samples from the front of the queue
      
      if nargin == 1
        nItems = 1;
      end
      
      if isempty(obj.data)
        out = [];
      else
        out = obj.data(1:nItems);
        obj.data = obj.data(nItems+1:end);
      end
    end
    
    function out = peek(obj,nItems)
      % Return the value of the first nItems items from the front of the
      % queue (without dequeueing)
      if isempty(obj.data)
        out = [];
      else
        out = obj.data(1:nItems);
      end
    end
    
    function enqueue(obj,newData)
      % Add the newData to the queue as a column vector
      if isempty(obj.data)
        obj.data = newData(:);
      else
        obj.data = [obj.data; newData(:)];
      end
    end
    
    function len = length(obj)
      % Return the length of the queue
      len = length(obj.data);
    end
    
%     function delete(obj)
%       obj.data = [];
%     end
    
  end
end
