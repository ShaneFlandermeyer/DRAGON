% Usage: plot_rd_map(radar, [data_vec])
% Form a pulse train from the data then do pulse compression and do
% doppler processing
function rd_map = plot_rd_map(radar,data_vec)
% If only the radar argument is given, assume the data vector is in the
% sig_data_rx field
if nargin == 1
    data_vec = radar.sig_data_rx;
end
% If no data vector is available, throw an error and return
if isempty(data_vec)
    error('No input data given.')
end
% Reshape the output vector into a train of pulses.
num_zeros = round((radar.sig_pri-...
    radar.sig_comp_pulsewidth)*radar.samp_rate);
pulse_length = length(radar.sig_data_tx)-num_zeros;
padded_pulse_length = length(radar.sig_data_tx);
pulse_train = reshape(data_vec(1:padded_pulse_length*radar.num_pulses),padded_pulse_length,radar.num_pulses);
% Set up the matched filter and its response for each pulse
match_filt = flipud(conj(radar.sig_data_tx(1:end-num_zeros)));
mf_resp = zeros(length(radar.sig_data_tx)+length(match_filt),...
    radar.num_pulses);
% Zero pad the filter and data vector, then convolve
N = size(mf_resp,1);
padded_mf = [match_filt; zeros(N-length(match_filt),1)];
padded_pulse_train = [pulse_train;zeros(N-size(pulse_train,1),size(pulse_train,2))];
for ii = 1:radar.num_pulses
    mf_resp(:,ii) = ifft(fft(padded_mf).*fft(padded_pulse_train(:,ii)));
end
%% Plots
radar = struct(radar);
radar.max_range = round(radar.const.c*radar.sig_pri/2);
radar.max_vel = (radar.sig_prf/2)*radar.sig_wavelength/2;
fprintf("Maximum Range (m): %.1f\n",radar.max_range);
fprintf("Maximum Velocity (m/s): %.1f\n",radar.max_vel);

dopp_os = 2;
% Generate a range-doppler map
rd_map = fftshift(fft(mf_resp,dopp_os*radar.num_pulses,2),2);
rd_map_db = db(abs(rd_map));
max_val = max(rd_map_db(:));
dynamic_rng = 80;
figure()
% Shift the range axis so that 0 m is the first location at which the full
% pulse has been emitted
range_offset = (pulse_length)*radar.const.c/(2*radar.samp_rate);
r_scale = linspace(-range_offset,length(mf_resp)*radar.const.c/(2*radar.samp_rate)-range_offset, size(rd_map,1));
d_scale = linspace(-radar.max_vel, radar.max_vel-(1/size(rd_map,2)), size(rd_map,2));
imagesc(d_scale, r_scale, rd_map_db);

caxis([max_val-dynamic_rng max_val])
set(gca, 'fontweight', 'bold', 'fontsize', 18,'ydir','normal');
title("Range-Doppler Map");
xlabel("Velocity (m/s)");
ylabel("Range (m)");
h = colorbar;
h.Label.String = 'Signal Power (db)';
ylim([0 max(r_scale)])
end