classdef BlockTest < matlab.unittest.TestCase
  
  methods (Test)
    
    function testConnect(testCase)
      % Test the connect function
      
      block1 = blocks.EmptyBlock();
      block2 = blocks.EmptyBlock();
      block3 = blocks.EmptyBlock();
      
      % Connect block 1 (src) to block 2 (dst)
      blocks.Block.connect(block1,1,block2,1);
      
      % Test that output port has registered the input port
      testCase.verifyEqual(block1.outputPorts.connections(1),block2.inputPorts(1));
      % Test that input port has registered the output port
      testCase.verifyEqual(block2.inputPorts.connections(1),block1.outputPorts(1));
      
      % Test connect with a source port index < 1
      blocks.Block.connect(block1,0,block2,1);
      testCase.verifyEqual(block1.outputPorts(1).connections(1),block2.inputPorts(1));
      
      % Test connect with a source port index < 1
      blocks.Block.connect(block1,1,block2,-1);
      testCase.verifyEqual(block1.outputPorts(1).connections(1),block2.inputPorts(1));
      
      % Test case where block output goes to multiple blocks
      blocks.Block.connect(block1,2,block2,1);
      blocks.Block.connect(block1,2,block3,1);
      testCase.verifyEqual(block1.outputPorts(2).connections(2:3),...
        [block2.inputPorts(1); block2.inputPorts(1)]);
      
    end
    
    function testDisconnect(testCase)
      % Test the disconnect function
      
      block1 = blocks.EmptyBlock();
      block2 = blocks.EmptyBlock();
      
      % Test disconnect from already disconnected block
      blocks.Block.disconnect(block1,1,block2,1);
      
      % Test disconnect from port index < 1
      blocks.Block.connect(block1,1,block2,1);
      blocks.Block.disconnect(block1,1,block2,0);
      
      % Test valid disconnect
      blocks.Block.connect(block1,1,block2,1);
      blocks.Block.disconnect(block1,1,block2,1);
      testCase.verifyTrue(isempty(block1.outputPorts(1).connections))
      testCase.verifyTrue(isempty(block2.inputPorts(1).connections))
    end
    
  end
end