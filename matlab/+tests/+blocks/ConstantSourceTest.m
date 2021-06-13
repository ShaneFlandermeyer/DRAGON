classdef ConstantSourceTest < matlab.unittest.TestCase
  
  methods (Test)
    
    function testWork(testCase)
      cs = blocks.ConstantSource(1);
      cs.work();
      actual = cs.outputPorts(1).buffer.data;
      expected = ones(4096,1);
      testCase.verifyEqual(actual,expected);
    end
    
  end
  
end