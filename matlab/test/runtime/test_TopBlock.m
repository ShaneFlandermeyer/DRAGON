function tests = test_TopBlock
tests = functiontests(localfunctions);
end



function test_printGraph_connected_singleInput(testCase)
tb = runtime.TopBlock();
cs1 = blocks.ConstantSource(tb,1);
eb = blocks.EmptyBlock(tb);
ns = blocks.NullSink(tb);
cs1.outputPorts(1).connect(eb.inputPorts(1));
eb.outputPorts(1).connect(ns.inputPorts(1));

% Test that the function works properly regardless of which block is used for the call 
expected = 'blocks.ConstantSource -> blocks.EmptyBlock -> blocks.NullSink;';

actual = tb.printGraph(ns);
testCase.verifyEqual(actual,expected)

actual = tb.printGraph(eb);
testCase.verifyEqual(actual,expected)

actual = tb.printGraph(cs1);
testCase.verifyEqual(actual,expected)

end

function test_printGraph_connected_MultiInput(testCase)
tb = runtime.TopBlock();
cs1 = blocks.ConstantSource(tb,1);
cs2 = blocks.ConstantSource(tb,2);
eb = blocks.EmptyBlock(tb);
eb.addInputPort();
ns = blocks.NullSink(tb);
cs1.outputPorts(1).connect(eb.inputPorts(1));
cs2.outputPorts(1).connect(eb.inputPorts(2));
eb.outputPorts(1).connect(ns.inputPorts(1));

% Test that the function works properly regardless of which block is used for the call 
expected = '{blocks.ConstantSource blocks.ConstantSource} -> {blocks.EmptyBlock} -> blocks.NullSink;';

actual = tb.printGraph();
testCase.verifyEqual(actual,expected)
% 
% actual = tb.printGraph();
% testCase.verifyEqual(actual,expected)
% 
% actual = tb.printGraph();
% testCase.verifyEqual(actual,expected)

end
