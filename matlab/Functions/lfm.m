function chirp = lfm(bandwidth, pulse_width, oversampling)
% Generates an LFM with a given bandwidth that is swept at a rate of
% bandwidth/pulse_width, oversampled by oversampling
% 
% INPUTS:
% - bandwidth: The waveform bandwidth
% - pulse_width: The sweep time of the LFM (seconds)
% - oversampling: The oversampling rate relative to nyquist
%
% OUTPUTS:
% - chirp: An LFM waveform of length pulse_width and sweep rate
%          bandwidth/pulse_width
t = (0:1/bandwidth/oversampling:pulse_width-1/bandwidth/oversampling).';
chirp = exp(-1i*pi*B+1i*pi*bandwidth/pulse_width*t.^2);
end