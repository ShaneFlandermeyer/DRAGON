function tests = test_TopBlock
tests = functiontests(localfunctions);
end



function test_run(testCase)
% TODO: Implement this with a vector sink instead of a time sink.

% The top block to be tested
tb = runtime.TopBlock();
constantSource = blocks.ConstantSource(tb,5);
emptyBlock = blocks.EmptyBlock(tb);
timeSink = blocks.TimeSink(tb);

% Connect the blocks as follows:
% ConsantSource -> EmptyBlock -> NullSink
constantSource.outputPorts(1).connect(emptyBlock.inputPorts(1));
emptyBlock.outputPorts(1).connect(timeSink.inputPorts(1));

% Generate 100 samples
tb.run(4096);


end
