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
constantSource.outputPorts(1).connect(dataSink.inputPorts(1));
% Run a work function work of data
tb.run(constantSource.nOutputItemsMax);

expected = repmat(constant,constantSource.nOutputItemsMax,1);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);
end
