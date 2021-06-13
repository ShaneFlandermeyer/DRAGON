classdef InputPortTest < matlab.unittest.TestCase

  methods (Test)
    
    function testConnect(testCase)
      inPort = runtime.InputPort;
      outPort = runtime.OutputPort;
      
      inPort.connect(outPort);
      testCase.verifyTrue(length(outPort.connections) == 1)
      testCase.verifyTrue(length(inPort.connections) == 1)
    end
    
    function testDisconnect(testCase)
      inPort = runtime.InputPort;
      outPort = runtime.OutputPort;
      
      inPort.connect(outPort);
      testCase.verifyTrue(length(outPort.connections) == 1)
      testCase.verifyTrue(length(inPort.connections) == 1)
      inPort.disconnect(outPort);
      testCase.verifyTrue(isempty(inPort.connections))
    end
    
  end

end