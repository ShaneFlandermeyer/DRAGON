classdef BlockTest < matlab.unittest.TestCase
  
  methods (Test)
    
    function testAddOutputPort(testCase)
      % Test the addOutputPort function
      
      block1 = blocks.EmptyBlock();
      % Add the first output port
      block1.addOutputPort();
      testCase.verifyEqual(length(block1.outputPorts),1);
      % Add the first input port
      block1.addOutputPort();
      testCase.verifyEqual(length(block1.outputPorts),2);
      
    end
    
    function testAddInputPort(testCase)
      % Test the addInputPort function
      
      block1 = blocks.EmptyBlock();
      % Add the first output port
      block1.addInputPort();
      testCase.verifyEqual(length(block1.inputPorts),1);
      % Add the first input port
      block1.addInputPort();
      testCase.verifyEqual(length(block1.inputPorts),2);
      
    end
    
    function testDeletePort(testCase)
      % Test the deletePort function
      
      block1 = blocks.EmptyBlock();
      % Add an output port
      block1.addOutputPort();
      block1.deletePort(block1.outputPorts(1));
      testCase.verifyTrue(isempty(block1.outputPorts));
      
    end
    
  end
end