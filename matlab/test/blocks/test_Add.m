function tests = test_Add
tests = functiontests(localfunctions);
end

function test_work(testCase)

tb = runtime.TopBlock();

% Blocks
constant1 = 1;
constant2 = 2;
input1 = blocks.ConstantSource(tb,constant1);
input2 = blocks.ConstantSource(tb,constant2);
adder = blocks.Add(tb,nInputPorts=2);
% TODO: Create an IOSignature to handle this
dataSink = blocks.DataSink(tb);

% Create connections
tb.connect(input1,1,adder,1);
tb.connect(input2,1,adder,2);
tb.connect(adder,1,dataSink,1);

% Run the flowgraph
nSamples = 4096;
tb.run(nSamples);

expected = repmat(constant1+constant2,nSamples,1);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);
end
