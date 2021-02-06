% Uses the output of a gnuradio flowgraph to form a range-doppler map
% Author: Shane Flandermeyer
close all;clear;clc;
addpath '/home/shane/DRAGON/matlab/Functions/'
radar = setup_data();
if ~isempty(radar.sig_data_rx)
    rd_map = plot_rd_map(radar,radar.sig_data_rx);
end
ofdm = read_complex_binary('/home/shane/ofdm.dat');
db(max(abs(ofdm)));
db(max(abs(radar.sig_data_rx)));
db(max(abs(radar.sig_data_rx))) - db(max(abs(ofdm)));
fprintf("SINR: %f\n",db(max(abs(radar.sig_data_rx))) - db(max(abs(ofdm))))
%% Set parameters and input data
% Usage: [radar,tags] = setup_data()
% Creates a radar parameter object and a set of stream tag objects with the
% given properties. These properties are hard-coded for now.
function [radar,tags] = setup_data()
input_path = '/home/shane/chirpTx.dat';
output_path = '/home/shane/chirpRx.dat';
%% Initialize Waveform Parameters
radar = param_manager;
radar.sig_freq_tx = 2.4e9;
radar.sig_bandwidth = 5e6; % Waveform bandwidth
radar.sig_comp_pulsewidth = 40e-6;
radar.sig_pri = 200e-6; % Waveform PRI
radar.duty_cycle = 0.2;
% radar.sig_comp_pulsewidth = radar.sig_pri * 0.5; % Waveform compressed pulsewidth
% radar.sig_prf = 5e3;
radar.samp_rate = 20e6; % Radar Sampling rate (samps/s)
radar.num_pulses = 64;
% Generate the waveform to be transmitted and zero pad according to the
% specified PRI
%% Initialize System Parameters
usrpGainTx = 44.9; % SDR Tx front-end gain used in GNU Radio flowgraph(dB)
usrpGainRx = 38; % SDR Rx front-end gain used in GNU Radio flowgraph(dB)
radar.noise_fig = 8; % SDR maximum noise figure (from manufacturer, dB)
% Receiver noise power, including hardware imperfections256
radar.pwr_noise = radar.pwr_noise * 10^((radar.noise_fig+usrpGainTx+...
    usrpGainRx)/10);
radar.pfa = 1e-6; % Probability of false alarm
%% Try to read/write waveform information to a file
try
    radar.sig_data_tx = UpChirp(radar.sig_bandwidth,radar.sig_comp_pulsewidth,...
        radar.samp_rate/radar.sig_bandwidth);
    padding_zeros = zeros(round((radar.sig_pri-radar.sig_comp_pulsewidth)*radar.samp_rate),1);
    radar.sig_data_tx = [radar.sig_data_tx;padding_zeros];
    write_complex_binary(repmat(radar.sig_data_tx,radar.num_pulses,1),input_path); % Write to a file
    % Read GNU Radio receive data
    radar.sig_data_rx = read_complex_binary(output_path);
    % If we need more pulses than we have, read as many as possible
    radar.num_pulses = min(radar.num_pulses,floor(length(radar.sig_data_rx)/...
        (length(radar.sig_data_tx)*(1+radar.duty_cycle))));
    radar.sig_data_rx = radar.sig_data_rx(1:radar.num_pulses*...
        (length(radar.sig_data_tx)));
catch
    warning('Could not read/write waveform data to object.')
    radar.sig_data_rx = [];
end % try
end % function