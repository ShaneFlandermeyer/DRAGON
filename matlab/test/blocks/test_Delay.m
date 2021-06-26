function tests = test_Delay
tests = functiontests(localfunctions);
end

function test_general_work(testCase)
tb = runtime.TopBlock();

% Create the following flowgraph
% ConstantSource -> Delay -> DataSink

% Simulation parameters
nSourceSamples = 100; % Number of constant source samples
nDelaySamples = 50;   % Number of samples in the delay block

constantSource = blocks.ConstantSource(tb,1);
delay = blocks.Delay(tb,nDelaySamples);
dataSink = blocks.DataSink(tb);

tb.connect(constantSource,1,delay,1);
tb.connect(delay,1,dataSink,1);

tb.run(nSourceSamples);

actual = dataSink.data;
expected = [zeros(nDelaySamples,1);ones(nSourceSamples,1)];
testCase.verifyEqual(actual,expected);
end
