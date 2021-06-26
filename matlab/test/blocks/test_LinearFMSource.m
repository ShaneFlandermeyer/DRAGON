function tests = test_LinearFMSource
tests = functiontests(localfunctions);
end

function test_constructor(testCase)
tb = runtime.TopBlock();

% Waveform parameters
sampleRate = 10e6;
bandwidth = 5e6;
pulsewidth = 10e-6;
dutyCycle = 0.1;
lfm = blocks.LinearFMSource(tb,sampleRate,bandwidth,pulsewidth,dutyCycle);
end

function test_work(testCase)
tb = runtime.TopBlock();

% Waveform parameters
sampleRate = 10e6;
bandwidth = 5e6;
pulsewidth = 10e-6;
dutyCycle = 0.1;
lfm = blocks.LinearFMSource(tb,sampleRate,bandwidth,pulsewidth,dutyCycle);

% Connect it to a data sink block
dataSink = blocks.DataSink(tb);
tb.connect(lfm,1,dataSink,1);

% Get 1 pulse (without zero padding) worth of data
pulseSamples = pulsewidth*sampleRate;
tb.run(pulseSamples);

% Get the waveform using matlab's chirp function (requires signal processing
% toolbox)
Ts = 1 / sampleRate;
t = (0:Ts:pulsewidth-Ts);
t1 = pulsewidth;
f0 = -bandwidth/2;
f1 = bandwidth/2;
expected = chirp(t,f0,t1,f1,'complex').';
actual = dataSink.data;

testCase.verifyEqual(actual,expected,'relTol',1e-10);
end

