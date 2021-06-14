function tests = test_ConstantSource
tests = functiontests(localfunctions);
end

function test_work(testCase)
% Test the work function
tb = runtime.TopBlock; % Top block
constant = 5;
cs = blocks.ConstantSource(tb,constant);
cs.work();
expected = repmat(constant,cs.nOutputItems,1);
actual = cs.outputPorts(1).buffer.pop(cs.nOutputItems);
testCase.verifyEqual(actual,expected);
end