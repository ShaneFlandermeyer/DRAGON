function tests = test_OutputPort
tests = functiontests(localfunctions);
end

function test_connect(testCase)
inPort = runtime.InputPort;
outPort = runtime.OutputPort;

outPort.connect(inPort);
testCase.verifyTrue(length(outPort.connections) == 1)
testCase.verifyTrue(length(inPort.connections) == 1)
end

function test_disconnect(testCase)
inPort = runtime.InputPort;
outPort = runtime.OutputPort;

outPort.connect(inPort);
outPort.disconnect(inPort);
testCase.verifyTrue(isempty(outPort.connections))
end