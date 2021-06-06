classdef (Abstract) Block < handle
    
    properties
        
        inputNodes Block
        outputNodes Block
        
    end
    
    methods (Abstract)
        work(obj)
    end
    
    methods
        function connect(obj,srcPort,dst,dstPort)
            % Wire two blocks together
            % 
            % INPUTS:
            % - srcPort: The index of the output port for this block
            % - dst: The block whose input becomes connected to this
            %        block's output
            % - dstPort: The index of the input port in dst to connect
            obj.outputNodes(srcPort) = dst.inputNodes(dstPort);
        end
        
        function disconnect(obj,srcPort,dst,dstPort)
            % Disconnect two blocks
            % 
            % INPUTS:
            % - srcPort: The index of the output port for this block
            % - dst: The block whose input becomes connected to this
            %        block's output
            % - dstPort: The index of the input port in dst to connect
            
            obj.outputNodes(srcPort) = [];
            dst.inputNodes(dstPort) = [];
        end
    end
end