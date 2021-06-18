function tests = test_TopBlock
tests = functiontests(localfunctions);
end



function test_run(testCase)
% The top block to be tested
tb = runtime.TopBlock();
cs1 = blocks.ConstantSource(tb,1);
eb = blocks.EmptyBlock(tb);
ns = blocks.NullSink(tb);

% Connect the blocks as follows:
% ConsantSource -> EmptyBlock -> NullSink
cs1.outputPorts(1).connect(eb.inputPorts(1));
eb.outputPorts(1).connect(ns.inputPorts(1));

% Generate 100 samples
tb.run(100);

% Check the null sink output buffer for the output of the flowgraph
expected = ones(100,1);
actual = ns.outputPorts(1).buffer.peek(100);

testCase.verifyEqual(actual,expected);

end
