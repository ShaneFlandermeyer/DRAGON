function tests = test_TopBlock
tests = functiontests(localfunctions);
end

function test_uid(testCase)
tb = runtime.TopBlock();

constant = 0;
constantSource = blocks.ConstantSource(tb,constant);
emptyBlock = blocks.EmptyBlock(tb);
dataSink = blocks.DataSink(tb);

testCase.verifyEqual(constantSource.uid,0);
testCase.verifyEqual(emptyBlock.uid,1);
testCase.verifyEqual(dataSink.uid,2);
end

function test_run(testCase)
% TODO: Implement this with a vector sink instead of a time sink.

% The top block to be tested
tb = runtime.TopBlock();

constant = 5;
constantSource = blocks.ConstantSource(tb,constant);
emptyBlock = blocks.EmptyBlock(tb);
dataSink = blocks.DataSink(tb);

% Connect the blocks as follows:
% ConsantSource -> EmptyBlock -> NullSink
tb.connect(constantSource,1,emptyBlock,1);
tb.connect(emptyBlock,1,dataSink,1);

% Generate samples
nSamples = 4096;
tb.run(nSamples);

expected = repmat(constant,nSamples,1);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);


end
