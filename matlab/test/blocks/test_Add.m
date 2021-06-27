function tests = test_Add
tests = functiontests(localfunctions);
end

function test_work(testCase)

tb = runtime.TopBlock();

% Blocks
constant = 1;
input = blocks.ConstantSource(tb,constant);
adder = blocks.Add(tb);
% TODO: Create an IOSignature to handle this
dataSink = blocks.DataSink(tb);

% Create connections
tb.connect(input,1,adder,1);
tb.connect(input,1,adder,2);
tb.connect(adder,1,dataSink,1);

% Run the flowgraph
nSamples = 4096;
tb.run(nSamples);

expected = repmat(constant+constant2,nSamples,1);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);
end
