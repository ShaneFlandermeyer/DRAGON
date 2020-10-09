% Uses the output of a gnuradio flowgraph to form a range-doppler map
% Author: Shane Flandermeyer
close all;clear;clc;
addpath './Functions/'
radar = setup_data();
rd_map = plot_rd_map(radar);
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
radar.sig_comp_pulsewidth = 100e-6; % Waveform compressed pulsewidth
% radar.sig_pri = 200e-6; % Waveform PRI
radar.sig_prf = 5e3;
radar.samp_rate = 20e6; % Radar Sampling rate (samps/s)
radar.num_pulses = 32;
% Generate the waveform to be transmitted and zero pad according to the
% specified PRI
radar.sig_data_tx = UpChirp(radar.sig_bandwidth,radar.sig_comp_pulsewidth,...
    radar.samp_rate/radar.sig_bandwidth);
padding_zeros = zeros(round((radar.sig_pri-radar.sig_comp_pulsewidth)*radar.samp_rate),1);
radar.sig_data_tx = [radar.sig_data_tx;padding_zeros];
write_complex_binary(repmat(radar.sig_data_tx,64,1),input_path); % Write to a file
%% Initialize System Parameters
usrpGainTx = 44.9; % SDR Tx front-end gain used in GNU Radio flowgraph(dB)
usrpGainRx = 38; % SDR Rx front-end gain used in GNU Radio flowgraph(dB)
radar.noise_fig = 8; % SDR maximum noise figure (from manufacturer, dB)
% Receiver noise power, including hardware imperfections256
radar.pwr_noise = radar.pwr_noise * 10^((radar.noise_fig+usrpGainTx+...
    usrpGainRx)/10);
radar.pfa = 1e-6; % Probability of false alarm
% Read GNU Radio receive data
radar.sig_data_rx = read_complex_binary(output_path);
radar.sig_data_rx = radar.sig_data_rx(1:radar.num_pulses*...
    (length(radar.sig_data_tx)));
end