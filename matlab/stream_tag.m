classdef stream_tag < matlab.mixin.Copyable
properties
    key;
    value;
    offset;
end % Properties
methods
    function obj = stream_tag(key,value,offset)
        if nargin == 0
            obj.key = [];
            obj.value = [];
            obj.offset = [];
        else
            obj.key = key;
            obj.value = value;
            obj.offset = offset;
        end
        
    end
end % Methods
end