
% Simulate a GNU Radio sync block
% Author: Shane Flandermeyer
%% Driver
close all;clc;clear;
addpath('/home/shane/DRAGON/matlab/Functions')
[radar,tags] = setup_data();
global nitems_read is_burst pulse_count pad_pulses;
pad_pulses = 1;
nitems_read = 0;
is_burst = 0;
pulse_count = 0;
out = [];
ii = 1;
while nitems_read < length(radar.sig_data_rx)
    noutput_items(ii) = randi([1,8192],1);
    in = radar.sig_data_rx(nitems_read+1:...
        min(nitems_read+noutput_items(ii),length(radar.sig_data_rx)));
    out = [out;work(radar,noutput_items(ii),in,tags)];
    ii = ii+1;
end
if pad_pulses
    rd_map = plot_rd_map(radar,out);
end
%% Block Functions

% Usage: out = work(radar,noutput_items,in,[tags])
% Emulates the work() function in a gnuradio block, which takes an input
% buffer, performs operations on it, and 
function out = work(radar,noutput_items,in,tags)
    global nitems_read is_burst pulse_count pad_pulses;
    % If the tag vector wasn't specified, make an empty placeholder
    if ~exist('tags','var')
        tags = [];
    end
    % Get the offsets for all tags
    offsets = zeros(length(tags),1);
    for ii = 1:length(tags)
        offsets(ii) = tags(ii).offset;
    end
    
    % Using the offsets above, get the tags that are in the current input
    % buffer
    tags = get_tags_in_range(nitems_read,nitems_read+noutput_items,tags,offsets);
%     fprintf("Num Tags: %d, is_burst: %d, nitems_read: %d \n",length(tags),is_burst,nitems_read);
%     figure(1);plot(abs(in));title('Input Buffer')
    switch length(tags)
        % No tags in buffer. If we're currently in a burst, then the pulse
        % is longer than the buffer size. If not, nothing is being
        % transmitted. For now, I'm assuming the pulse width (in samples)
        % is less than the buffer length, so this should never execute
        %                    _______________
        %     No pulse          pulse
        % _______________ or 
        
        case 0
            if ~is_burst
                % Nothing to do here. Send the input straight to the output
                out = [];
            else
                % Scale by doppler shift, but since this isn't the start of
                % the burst then don't delay
                sf = get_tgt_scale_factor(radar,pulse_count);
                out = scale_and_shift_pulse(in,sf,0);
            end
            nitems_read = nitems_read + length(in);
            
        % If only one tag is found, two options are possible
        % 1) We have the end of one pulse and then nothing
        % 2) We have nothing and then the beginning of one pulse
        % _______                    _______   
        %        |                  |
        %  pulse |_______ or _______|  pulse
        case 1
            if ~is_burst
                % Not in a burst at the start of the buffer. Therefore
                % option 2 is occurring
                pulse_count = pulse_count+1;
                radar.tgt_rng = radar.tgt_rng+(radar.tgt_vel*radar.sig_pri);
                sf = get_tgt_scale_factor(radar,pulse_count);
                delay_samps = get_tgt_delay_samps(radar);
                rel_start = tags(1).offset-nitems_read;
                rel_stop = length(in);
                pulse = in(rel_start:rel_stop);
                out = scale_and_shift_pulse(pulse,sf,delay_samps);
                is_burst = 1;
            else
                % In a burst at the start of the buffer. Therefore option 1
                % is occurring
                sf = get_tgt_scale_factor(radar,pulse_count);
                delay_samps = get_tgt_delay_samps(radar);
                rel_start = 1;
                rel_stop = tags(1).offset-nitems_read-1;
                pulse = in(rel_start:rel_stop);
                out = scale_and_shift_pulse(pulse,sf,0);
                is_burst = 0;
                if pad_pulses
                    num_zeros = get_padding_zeros(radar)-max(delay_samps);
                    out = [out;zeros(num_zeros,1)];
                end
            end
            nitems_read = nitems_read+length(in);
            
        % If two tags are found in the buffer, two options are possible:
        % 1) We have a full pulse
        % 2) We have the ending of one pulse and then the beginning of
        % another
        %      ______         ______        _____
        %     |      |         pulse|      |pulse
        % ____| pulse|_____ or      |______|
        
        case 2
            if ~is_burst
                % The first tag signals the start of the burst, so option 1
                % is occurring (we have the full pulse)
                pulse_count = pulse_count+1;
                radar.tgt_rng = radar.tgt_rng+(radar.tgt_vel*radar.sig_pri);
                sf = get_tgt_scale_factor(radar,pulse_count);
                delay_samps = get_tgt_delay_samps(radar);
                rel_start = tags(1).offset - nitems_read;
                rel_stop = tags(2).offset - nitems_read-1;
                pulse = in(rel_start:rel_stop);
                out = scale_and_shift_pulse(pulse,sf,delay_samps);
                nitems_read = nitems_read + length(in);
            else
                % The first tag signals the end of the burst, so option 2
                % is occurring (we have the end of one pulse and the start
                % of the next)
                sf = get_tgt_scale_factor(radar,pulse_count);
                delay_samps = get_tgt_delay_samps(radar); % Don't delay in the  middle of a pulse
                rel_start = 1;
                rel_stop = tags(1).offset - nitems_read-1;
                pulse = in(rel_start:rel_stop);
                out = scale_and_shift_pulse(pulse,sf,0);
                nitems_read = tags(2).offset-1;
                is_burst = 0;
            end
            if pad_pulses
                num_zeros = get_padding_zeros(radar)-max(delay_samps);
                out = [out;zeros(num_zeros,1)];
            end
        
        % If more than two tags are found in the buffer, things get a
        % little more complicated since the processing changes depending on
        % the number of tags. Consider the two scenarios below with three
        % tags. For (1), the block should operate on the full pulse and
        % then stop (the next call to work() would handle the rest). For
        % (2), the block should process the first half pulse and then
        % stop.
        %  _______      ____      ____       _______
        % |       |    |              |     |       |
        % |       |____|       or     |_____|       |___
            
        otherwise
            if ~is_burst
                % The first sample is not part of a pulse, so scenario (1)
                % occurs
                pulse_count = pulse_count+1;
                radar.tgt_rng = radar.tgt_rng+(radar.tgt_vel*radar.sig_pri);
                sf = get_tgt_scale_factor(radar,pulse_count);
                delay_samps = get_tgt_delay_samps(radar);
                rel_start = tags(1).offset-nitems_read;
                rel_stop  = tags(2).offset-nitems_read-1;
                pulse = in(rel_start:rel_stop);
                out = scale_and_shift_pulse(pulse,sf,delay_samps);
                nitems_read = tags(3).offset-1;      
            else
                % The first sample is part of a previous pulse. Process
                % only the remainder of the pulse and return
                sf = get_tgt_scale_factor(radar,pulse_count);
                delay_samps = get_tgt_delay_samps(radar);
                rel_start = 1;
                rel_stop  = tags(1).offset-nitems_read-1;
                pulse = in(rel_start:rel_stop);
                out = scale_and_shift_pulse(pulse,sf,0);
                is_burst = 0;
                nitems_read = tags(2).offset-1;
            end
            if pad_pulses
                num_zeros = get_padding_zeros(radar)-max(delay_samps);
                out = [out;zeros(num_zeros,1)];
            end
             
    end
    
end

% Usage: tags_in_range = get_tags_in_range(start,stop,tags,offsets)
% Return the array of tags for which start<tags.offsets<stop
function tags_in_range = get_tags_in_range(start,stop,tags,offsets)
    tag_idx = offsets>=start & offsets<=stop;
    tags_in_range = tags(tag_idx);
end

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
% Generate the waveform to be transmitted and zero pad according to the
% specified PRI
radar.num_pulses = 32;
radar.sig_data_tx = UpChirp(radar.sig_bandwidth,radar.sig_comp_pulsewidth,...
    radar.samp_rate/radar.sig_bandwidth);
padding_zeros = zeros(round((radar.sig_pri-radar.sig_comp_pulsewidth)*radar.samp_rate),1);
radar.sig_data_tx = [radar.sig_data_tx;padding_zeros];
write_complex_binary(repmat(radar.sig_data_tx,radar.num_pulses,1),input_path); % Write to a file
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
%% Initialize Target Parameters
% TODO: Add error checking to make sure the size of these arrays are the
% same for a given number of targets
radar.num_tgts = 2;
radar.tgt_rng = [14000;10000];
radar.tgt_vel = [100;-100];
radar.tgt_rcs = [1e3;1];
%% Setup tags
% Generate tag objects demarcating the start and end of bursts
num_pulse_samps = length(radar.sig_data_tx)-length(padding_zeros);
offsets = 1:num_pulse_samps:length(radar.sig_data_rx);
% offsets(1) = 1;
% offsets(end) = [];
tags = stream_tag.empty(length(offsets),0);
for ii = 1:length(offsets)
    if mod(ii,2)
        tags(ii) = stream_tag("SOB","SOB",offsets(ii));
    else
        tags(ii) = stream_tag("EOB","EOB",offsets(ii));
    end

end
end

%% Helper functions
% Usage sf = get_tgt_scale_factor(radar,pulse_count)
% Updates the position for each target in the radar parameter object, then
% returns a scale factor that consists of an amplitude term (from the radar
% range equation) and a doppler shift.
function sf = get_tgt_scale_factor(radar,pulse_count)
    % Update target positions
    amplitude_scaling = sqrt(radar.sig_wavelength^2*radar.tgt_rcs./((4*pi)^3*radar.tgt_rng.^4));
    % Assume no backlobe scattering
    amplitude_scaling(radar.tgt_rng < 0) = 0;
    dopp_freq = 2*radar.tgt_vel/radar.sig_wavelength;
    dopp_shift = exp(1i*2*pi*dopp_freq*radar.sig_pri*pulse_count);
    sf = amplitude_scaling.*dopp_shift;
    
end


% Usage: function delay_samps = get_tgt_delay_samps(radar)
% Get the number of samples of delay for each target in the radar parameter
% obj
function delay_samps = get_tgt_delay_samps(radar)
    delay_samps = round(2*radar.samp_rate*(abs(radar.tgt_rng) / ...
        radar.const.c));
end

% Usage: padding_zeros = get_padding_zeros(radar)
% Get the number of padding zeros to append to the end of a captured pulse.
% Will only be called if pad_pulses = 1
function padding_zeros = get_padding_zeros(radar)
    padding_zeros = round((radar.sig_pri-radar.sig_comp_pulsewidth)*radar.samp_rate);
end

% Usage: shifted_pulse = scale_and_shift_pulse(pulse,scale_factors,delays)
% Given an input pulse and vectors containing complex scaling factors and
% delays for a given number of targets, return the superposition of all
% scaled and shifted returns.
function shifted_pulse = scale_and_shift_pulse(pulse,scale_factors,delays)
    shifted_pulse = zeros(length(pulse)+max(delays),1);
    for idx = 1:length(delays)
        shifted_pulse = shifted_pulse + ...
            [zeros(delays(idx),1);
            pulse*scale_factors(idx);
            zeros(max(delays)-delays(idx),1)];
    end
end