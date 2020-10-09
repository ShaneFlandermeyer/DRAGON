% Simulate a GNU Radio sync block
% Author: Shane Flandermeyer
%% TODO: Simulate the flowgraph run
close all;clear;clc;
% addpath('Functions')

% Set up input data
% Example: [radar,tags] = setup_data();
% Set up global variables (anything that needs to be saved between each
% work function and isn't in the radar parameter object)
global nitems_read;
nitems_read = 0;
out = [];
while nitems_read < length(radar.sig_data_rx)
    % Number of input/output items in the given call to work
    noutput_items = randi([4095,4096],1);
    % Simulate the work function
    in = radar.sig_data_rx(nitems_read+1:...
        min(nitems_read+noutput_items,length(radar.sig_data_rx)));
    out = [out;work(radar,noutput_items,in,tags)];
end
%% TODO: Execute post-processing functions here
% Example: rd_map = plot_rd_map(radar,out);
clear nitems_read is_burst pulse_count in noutput_items;

%% Simulate the work function
% Usage: out = work(radar,noutput_items,in,[tags])
% Emulates the work() function in a gnuradio block, which takes an input
% buffer, performs operations on it, and 
function out = work(radar,noutput_items,in,tags)
    % Specify global variable here
    global nitems_read;
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
    % <+ Do signal processing +>
end

% Usage: tags_in_range = get_tags_in_range(start,stop,tags,offsets)
% Return the array of tags for which start<tags.offsets<stop
function tags_in_range = get_tags_in_range(start,stop,tags,offsets)
    tag_idx = offsets>=start & offsets<=stop;
    tags_in_range = tags(tag_idx);
end

%%  TODO: Set up the parameter object and format the data/tags here
% Usage: [radar,tags] = setup_data()
% Creates a radar parameter object and a set of stream tag objects with the
% given properties. These properties are hard-coded for now.
function [radar,tags] = setup_data()

end

%% TODO: Put helper functions here