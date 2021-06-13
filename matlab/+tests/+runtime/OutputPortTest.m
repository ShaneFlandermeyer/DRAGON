classdef OutputPortTest < matlab.unittest.TestCase

  methods (Test)
    
    function testConnect(testCase)
      inPort = runtime.InputPort;
      outPort = runtime.OutputPort;
      
      outPort.connect(inPort);
      testCase.verifyTrue(length(outPort.connections) == 1)
      testCase.verifyTrue(length(inPort.connections) == 1)
    end
    
    function testDisconnect(testCase)
      inPort = runtime.InputPort;
      outPort = runtime.OutputPort;
      
      outPort.connect(inPort);
      outPort.disconnect(inPort);
      testCase.verifyTrue(isempty(outPort.connections))
    end
  end

end