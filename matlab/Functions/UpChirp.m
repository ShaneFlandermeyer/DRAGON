function uchirp = UpChirp(bw, tau, samp_rate)
% Generates and upchirp of bandwidth bw that is swept at a rate of bw/tau
% with an oversampling rate of samp_rate.
% Inputs:
%       bw        - The bandwidth occupied by the chirp.
%       tau       - The time extent of the chirp (seconds).
%       samp_rate - the oversampling rate, relative to Nyquist. In other
%                   words, uchirp will be of a vector with bw*tau*samp_rate 
%                   complex samples.
%
% Outputs:
%       uchirp    - An upchirp of length tau and sweep rate bw/len.
%
% t_samps = linspace(-len/2, len/2, bw*tau*samp_rate).';
t_samps = linspace(-tau/2, tau/2, bw*tau*samp_rate).';
uchirp = exp(1i*pi*bw/tau*t_samps.^2);