function tests = test_ConstantSource
tests = functiontests(localfunctions);
end

function test_work(testCase)
% Test the work function
tb = runtime.TopBlock; % Top block
% Create the constant source block
constant = 5;
constantSource = blocks.ConstantSource(tb,constant);
% Create the data sink block
dataSink = blocks.DataSink(tb);
% Connect the blocks
tb.connect(constantSource,1,dataSink,1);
% Run a work function work of data
tb.run(constantSource.nOutputItemsMax);

expected = repmat(constant,constantSource.nOutputItemsMax,1);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);
end

function test_work_twoOutput(testCase)
% Test the work function
tb = runtime.TopBlock; % Top block
% Create the constant source block
constant = 5;
constantSource = blocks.ConstantSource(tb,constant,'nOutputPorts',2);
% Create the data sink block
dataSink = blocks.DataSink(tb,'nInputPorts',2);
% Connect the blocks
tb.connect(constantSource,1,dataSink,1);
tb.connect(constantSource,2,dataSink,2);
% Run a work function work of data
tb.run(constantSource.nOutputItemsMax);

expected = repmat(constant,constantSource.nOutputItemsMax,2);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);
end