function tests = test_Noise
tests = functiontests(localfunctions);
end

function test_work_singleRealInput(testCase)
% Test the work function for a single real input signal. This test creates the
% following flowgraph
% 
% ConstantSource -> Noise -> dataSink
tb = runtime.TopBlock();

% Simulation parameters
power = 1e-5;


% Instantiate blocks
constantSource = blocks.ConstantSource(tb,0);
noise = blocks.Noise(tb,power);
dataSink = blocks.DataSink(tb);

% Create flowgraph
tb.connect(constantSource,1,noise,1);
tb.connect(noise,1,dataSink,1);

% Run the flowgraph with the given noise seed
rng(100);
nSamples = 100;
tb.run(nSamples);
rng(100); % Reset the seed for the expected value calculation

actual = dataSink.data;
expected = sqrt(power)*randn(nSamples,1) + zeros(nSamples,1);

testCase.verifyEqual(actual,expected);
end

function test_work_singleComplexInput(testCase)
% Test the work function for a single real input signal. This test creates the
% following flowgraph
% 
% ConstantSource -> Noise -> dataSink
tb = runtime.TopBlock();

% Simulation parameters
power = 1e-5;


% Instantiate blocks
constantSource = blocks.ConstantSource(tb,0+1i*1);
noise = blocks.Noise(tb,power);
dataSink = blocks.DataSink(tb);

% Create flowgraph
tb.connect(constantSource,1,noise,1);
tb.connect(noise,1,dataSink,1);

% Run the flowgraph with the given noise seed
rng(100);
nSamples = 100;
tb.run(nSamples);
rng(100); % Reset the seed for the expected value calculation

actual = dataSink.data;
expected = sqrt(power/2)*(randn(nSamples,1) + 1i*randn(nSamples,1)) + repmat(0+1i*1,nSamples,1);

testCase.verifyEqual(actual,expected);
end
