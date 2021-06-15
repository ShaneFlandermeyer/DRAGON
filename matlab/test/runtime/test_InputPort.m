function tests = test_InputPort
tests = functiontests(localfunctions);
end

function test_connect(testCase)
tb = runtime.TopBlock;
cs = blocks.ConstantSource(tb,1);

inPort = runtime.InputPort(cs);
outPort = runtime.OutputPort(cs);

inPort.connect(outPort);
testCase.verifyTrue(length(outPort.connections) == 1)
testCase.verifyTrue(length(inPort.connections) == 1)
end

function test_disconnect(testCase)
tb = runtime.TopBlock;
cs = blocks.ConstantSource(tb,1);

inPort = runtime.InputPort(cs);
outPort = runtime.OutputPort(cs);

inPort.connect(outPort);
testCase.verifyTrue(length(outPort.connections) == 1)
testCase.verifyTrue(length(inPort.connections) == 1)
inPort.disconnect(outPort);
testCase.verifyTrue(isempty(inPort.connections))
end
