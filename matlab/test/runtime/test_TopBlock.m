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
expected = 'ConstantSource -> EmptyBlock -> NullSink;';

actual = tb.printGraph(ns);
testCase.verifyEqual(actual,expected)

% actual = tb.printGraph(eb);
% testCase.verifyEqual(actual,expected)
% 
% actual = tb.printGraph(cs1);
% testCase.verifyEqual(actual,expected)

end

function test_printGraph_connected_MultiInput(testCase)

tb = runtime.TopBlock();
cs1 = blocks.ConstantSource(tb,1);
cs2 = blocks.ConstantSource(tb,2);

eb1 = blocks.EmptyBlock(tb);
eb1.addInputPort();
ns = blocks.NullSink(tb);

cs1.outputPorts(1).connect(eb1.inputPorts(1));
cs2.outputPorts(1).connect(eb1.inputPorts(2));
eb1.outputPorts(1).connect(ns.inputPorts(1));

cs3 = blocks.ConstantSource(tb,3);
eb2 = blocks.EmptyBlock(tb);
ns2 = blocks.NullSink(tb);
cs3.outputPorts(1).connect(eb2.inputPorts(1));
eb2.outputPorts(1).connect(ns2.inputPorts(1));

% Test that the function works properly regardless of which block is used for the call 
expected = 'ConstantSource -> EmptyBlock -> NullSink;ConstantSource -> EmptyBlock -> NullSink;';

actual = tb.printGraph(ns);
testCase.verifyEqual(actual,expected)
end
