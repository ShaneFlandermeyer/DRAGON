% TODO: Convert this to a live script
clc;close all;clear;

%% Simulation Parameters
% LFM Parameters
sampleRate = 20e6;
bandwidth  = 10e6;
pulsewidth = 10e-6;
dutyCycle  = 0.1;

% Noise parameters
power = 1e-6;

% Target parameters
c = physconst('lightspeed');
range = 1e3;
time = 2 * range / c;
nDelaySamps = floor(time*sampleRate);

%% Flowgraph Setup and Execution
tb = runtime.TopBlock();
% Instantiate blocks
lfmSource = blocks.LinearFMSource(tb,sampleRate,bandwidth,pulsewidth,dutyCycle,'chirpDirection',blocks.LinearFMSource.upchirp);
delay = blocks.Delay(tb,nDelaySamps);
noise = blocks.Noise(tb,power);
dataSink = blocks.DataSink(tb,'nInputPorts',2);

% Connect the main blocks
tb.connect(lfmSource,1,delay,1);
tb.connect(delay,1,noise,1);
tb.connect(noise,1,dataSink,1);

% Connect the original signal to the dataSink
tb.connect(lfmSource,1,dataSink,2);


try 
  tb.showGraph();
catch
  warning('Could not generate the flow graph visualization')
end

% Simulate nPulses pulses worth of data
nPulses = 64;
nSamplesPerPulse = floor(pulsewidth*sampleRate);
nSamplesPerPRI = nSamplesPerPulse/dutyCycle;
nSamples = nSamplesPerPRI*nPulses;
tb.run(nSamples);

%% Post-processing
% Reshape the flowgraph output into a matrix where each column is a separate PRI
pulseTrain = dataSink.data(1:nSamples,1);
fastTimeSlowTime = reshape(pulseTrain,[],nPulses);

% Compute the matched filter of the waveform and perform pulse compression. The
% fastTimeSlowTime matrix is zero padded so that the fftfilt output is the
% correct length
matchFilt = flipud(conj(dataSink.data(1:nSamplesPerPulse,2)));
rangeSlowTime = fftfilt(matchFilt,[fastTimeSlowTime; zeros(nSamplesPerPulse-1,nPulses)]);

% Plot the range response
idx = (1:size(rangeSlowTime,1))';
tAxis = (idx - length(matchFilt))/sampleRate;
rangeAxis = tAxis * (c/2);
figure()
plot(rangeAxis,db(abs(rangeSlowTime(:,1))))
xlabel('Range (m)')
ylabel('Amplitude (dB)')
title('Pulse Compression Response')

% Plot the range-doppler map
nDopplerBins = 128;
rangeDoppler = fftshift(fft(rangeSlowTime,nDopplerBins,2),2);
PRI = pulsewidth/dutyCycle;
PRF = 1 / PRI;
prfStep = PRF/nDopplerBins;
velAxis = (-PRF/2:prfStep:PRF/2-prfStep);

figure()
imagesc(velAxis,rangeAxis,db(abs(rangeDoppler)))
xlabel('Doppler Frequency (Hz)')
ylabel('Range (m)')
title('Range-Doppler Map')
colorbar
