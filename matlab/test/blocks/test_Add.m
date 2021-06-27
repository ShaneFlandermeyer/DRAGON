function tests = test_Add
tests = functiontests(localfunctions);
end

function test_work_twoInput(testCase)
% Test the block for the default case of two input ports

tb = runtime.TopBlock();

% Blocks
constant = 1;
input = blocks.ConstantSource(tb,constant);
adder = blocks.Add(tb);
dataSink = blocks.DataSink(tb);

% Create connections
tb.connect(input,1,adder,1);
tb.connect(input,1,adder,2);
tb.connect(adder,1,dataSink,1);

% Run the flowgraph
nSamples = 4096;
tb.run(nSamples);

expected = repmat(constant*2,nSamples,1);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);
end

function test_work_threeInput(testCase)
% Test the block for the default case of two input ports

tb = runtime.TopBlock();

% Blocks
constant = 1;
input = blocks.ConstantSource(tb,constant);
adder = blocks.Add(tb,'nInputPorts',3);
dataSink = blocks.DataSink(tb);

% Create connections
tb.connect(input,1,adder,1);
tb.connect(input,1,adder,2);
tb.connect(input,1,adder,3);
tb.connect(adder,1,dataSink,1);

% Run the flowgraph
nSamples = 4096;
tb.run(nSamples);

expected = repmat(constant*3,nSamples,1);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);
end
