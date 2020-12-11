% Calculates the response of a data matrix to the filter vector filt.
% Assumes the data matrix is m x p, where m is the length of the pulse and
% n is the number of pulses. filt can be a row or column vector
%
% Usage: filt_resp = calc_filter_resp(data,filt,[Tadc])
function filt_resp = calc_filter_resp(data,filt,Tadc)
narginchk(2,3);
% No sampling interval specified, so don't scale the convolution output
if nargin < 2
Tadc = 1;
end
% Make the filter a column vector
if ~iscolumn(filt)
filt = filt';
end
num_pulses = size(data,2);
% Calculate the filter response in the time domain
% For each pulse, the convolution has length(filter)+length(pulse)-1
filt_resp = zeros(length(filt)+size(data,1)-1,num_pulses);
for ii = 1:num_pulses
filt_resp(:,ii) = conv(data(:,ii),filt);
end
% Scale the output to account for approximations in conv()
filt_resp = filt_resp*Tadc;
end