function tests = test_Block
tests = functiontests(localfunctions);
end

function test_addInputPort(testCase)
% Test the addInputPort function

block1 = blocks.EmptyBlock();
% An input port is added to Sync Blocks by default
testCase.verifyEqual(length(block1.inputPorts),1);
% Add another outside the block constructor
block1.addInputPort();
testCase.verifyEqual(length(block1.inputPorts),2);

end

function test_addOutputPort(testCase)
% Test the addOutputPort function

block1 = blocks.EmptyBlock();

% An output port is added to Sync Blocks by default
testCase.verifyEqual(length(block1.outputPorts),1);
% Add another outside the block constructor
block1.addOutputPort();
testCase.verifyEqual(length(block1.outputPorts),2);


end

function test_deletePort(testCase)
% Test the deletePort function

block1 = blocks.EmptyBlock();

block1.deletePort(block1.outputPorts(1));
testCase.verifyTrue(isempty(block1.outputPorts));

end