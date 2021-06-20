function tests = test_Block
tests = functiontests(localfunctions);
end

function test_addInputPort(testCase)
% Test the addInputPort function

tb = runtime.TopBlock; % Top block

% Test the addInputPort function
block = blocks.EmptyBlock(tb,nInputPorts=4,nOutputPorts=4);
% An input port is added to Sync Blocks by default
testCase.verifyEqual(length(block.inputPorts),4);

end

function test_addOutputPort(testCase)
% Test the addOutputPort function

tb = runtime.TopBlock; % Top block

block = blocks.EmptyBlock(tb,nInputPorts=4,nOutputPorts=4);

% Output ports are added to the block in the constructor
testCase.verifyEqual(length(block.outputPorts),4);

end

function test_deletePort(testCase)
% Test the deletePort function

tb = runtime.TopBlock; % Top block

block1 = blocks.EmptyBlock(tb);

block1.deletePort(block1.outputPorts(1));
testCase.verifyTrue(isempty(block1.outputPorts));

end
