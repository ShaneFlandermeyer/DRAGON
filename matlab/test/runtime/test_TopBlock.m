function tests = test_TopBlock
tests = functiontests(localfunctions);
end



function test_addBlock(testCase)
tb = runtime.TopBlock();
cs = blocks.ConstantSource(tb,1);
eb = blocks.EmptyBlock(tb);
cs.outputPorts(1).connect(eb.inputPorts(1));
end
% 
% function test_run(testCase)
% tb = runtime.TopBlock();
% cs = blocks.ConstantSource(tb,1);
% eb = blocks.EmptyBlock(tb);
% cs.outputPorts(1).connect(eb.inputPorts(1));
% end