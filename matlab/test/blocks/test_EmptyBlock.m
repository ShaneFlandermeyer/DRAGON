function tests = test_EmptyBlock
tests = functiontests(localfunctions);
end

function test_work(testCase)
% Test the work function. For the EmptyBlock object, the work function
% simply copies the input directly to the output

tb = runtime.TopBlock();

% Create a constant source block
constant = 1;
constantSource = blocks.ConstantSource(tb,constant);

% Create the empty block to be tested
emptyBlock = blocks.EmptyBlock(tb);

% Create the data sink
dataSink = blocks.DataSink(tb);

% Connect the blocks
constantSource.outputPorts(1).connect(emptyBlock.inputPorts(1));
emptyBlock.outputPorts(1).connect(dataSink.inputPorts(1));

tb.run(constantSource.nOutputItemsMax);

expected = ones(constantSource.nOutputItemsMax,1);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);

end
