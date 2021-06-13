classdef EmptyBlockTest < matlab.unittest.TestCase
  
  methods (Test)
    
    function testWork(testCase)
      
      % TODO: Add a default of 1 I/O port for the general block class
      constSrc = blocks.ConstantSource(1);
      emptyBlock = blocks.EmptyBlock();
      constSrc.outputPorts(1).connect(emptyBlock.inputPorts(1));

    end
    
  end
  
end