function tests = test_EmptyBlock
tests = functiontests(localfunctions);
end

function test_work(testCase)
% Test the work function. For the EmptyBlock object, the work function
% simply copies the input directly to the output

tb = runtime.TopBlock();

% Create a constant source block
constant = 1;
cs = blocks.ConstantSource(tb,constant);

% Create the empty block to be tested
eb = blocks.EmptyBlock(tb);

% Connect them
cs.outputPorts(1).connect(eb.inputPorts(1));
cs.work();
eb.work();

expected = ones(cs.nOutputItems,1);
actual = eb.outputPorts(1).buffer.data;
testCase.verifyEqual(actual,expected);

end
