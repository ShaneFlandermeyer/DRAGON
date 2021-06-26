function tests = test_EmptyBlock
tests = functiontests(localfunctions);
end

function test_work_singleInput(testCase)
% Test the work function. For the EmptyBlock object, the work function
% simply copies the input directly to the output

tb = runtime.TopBlock();

% Create a constant source block
constant = 1;
constantSource = blocks.ConstantSource(tb,constant);

% Create the empty block to be tested
emptyBlock = blocks.EmptyBlock(tb);

% Create the data sink
dataSink = blocks.DataSink(tb);

% Connect the blocks
tb.connect(constantSource,1,emptyBlock,1);
tb.connect(emptyBlock,1,dataSink,1);

tb.run(constantSource.nOutputItemsMax);

expected = ones(constantSource.nOutputItemsMax,1);
actual = dataSink.data;
testCase.verifyEqual(actual,expected);

end

function test_work_twoInputs(testCase)
% Test the work function with two inputs
% The flowgraph is as follows
%
%                    ------------
%  ConstantSource ->| EmptyBlock |-> DataSink
%  ConstantSource ->|            |-> DataSink
%                    ------------
%

tb = runtime.TopBlock();

constant1 = 1;
constant2 = 2;

constantSource1 = blocks.ConstantSource(tb,constant1);
constantSource2 = blocks.ConstantSource(tb,constant2);

emptyBlock = blocks.EmptyBlock(tb,'nInputPorts',2,'nOutputPorts',2);

dataSink = blocks.DataSink(tb,'nInputPorts',2);

% Connect things
tb.connect(constantSource1,1,emptyBlock,1);
tb.connect(constantSource2,1,emptyBlock,2);
tb.connect(emptyBlock,1,dataSink,1);
tb.connect(emptyBlock,2,dataSink,2);

nSamples = 100;
tb.run(nSamples);

actual = dataSink.data;
expected = [repmat(constant1,nSamples,1), repmat(constant2,nSamples,1)];

testCase.verifyEqual(actual,expected);


end
