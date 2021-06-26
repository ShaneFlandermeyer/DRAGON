

clc;close all;clear;
tb = runtime.TopBlock();

% LFM Parameters
sampleRate = 20e6;
bandwidth  = 10e6;
pulsewidth = 10e-6;
dutyCycle  = 0.1;

% Noise parameters
power = 1e-5;

% Instantiate blocks
lfmSource = blocks.LinearFMSource(tb,sampleRate,bandwidth,pulsewidth,dutyCycle,'chirpDirection',blocks.LinearFMSource.upchirp);
noise = blocks.Noise(tb,power);
abs = blocks.AbsoluteValue(tb);
dataSink = blocks.DataSink(tb);
timeSink = blocks.TimeSink(tb);

% Add connections
tb.connect(lfmSource,1,noise,1);
tb.connect(noise,1,abs,1);
tb.connect(abs,1,timeSink,1);
tb.connect(abs,1,dataSink,1);
tb.showGraph();

tb.run(10e3);
