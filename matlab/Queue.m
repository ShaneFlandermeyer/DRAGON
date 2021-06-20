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
  
  properties (SetAccess = private)
    data;
  end
  
  methods 
    
    %% Constructor
    function obj = Queue()
      % TODO: This only works if we assume the data in the queue is numerical,
      % but that's fine for now
      obj.data = zeros(1,0);
    end
    
    function out = dequeue(obj,nItems)
      % Remove and return nItems samples from the front of the queue
      out = obj.peek(nItems);
      obj.data = obj.data(nItems+1:end);
    end
    
    function out = peek(obj,nItems)
      % Return the value of the first nItems items from the front of the
      % queue (without dequeueing)
      
      if nargin == 1
        nItems = 1;
      end
      
      nItems = min(nItems,length(obj.data));
      out = obj.data(1:nItems);
    end
    
    function enqueue(obj,newData)
      % Add the newData to the queue as a column vector
      obj.data = [obj.data; newData(:)];
    end
    
    function len = length(obj)
      % Return the length of the queue
      len = length(obj.data);
    end
    
    function isEmpty = isempty(obj)
      % Return true if there is any data in the queue, false otherwise
      isEmpty = isempty(obj.data);
    end
    
    function clear(obj)
      % Reset all queue data
      obj.data = zeros(1,0);
    end
    
  end
end
