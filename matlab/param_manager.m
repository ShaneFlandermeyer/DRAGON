classdef param_manager < matlab.mixin.Copyable

%% Constants
properties (Constant)
    constant_header = "Constant Parameters";
    const = struct('c',299792458,'k',1.38064852e-23,'T0_k',290)
end
%% Internal Parameters
properties (Access = private)
    initial_param;
    updatedList = {}
end
%% P_fa Parameters
properties
    pd;
    pfa;
end
%% System Parameters
properties
    system_header = "System Parameters";
    duty_cycle;
    pwr_noise;
    noise_fig;
    samp_rate;
end
%% Target Parameters
properties
    tgt_header = "Target Parameters";
    num_tgts;
    tgt_rng;
    tgt_vel;
    tgt_rcs;
end
%% Waveform Parameters
properties
    waveform_header = "Waveform Parameters";
    num_pulses;
    sig_bandwidth;
    sig_comp_pulsewidth;
    sig_data_tx;
    sig_data_rx;  
    sig_freq_tx;
    sig_prf;
    sig_pri;
    sig_wavelength;
    sig_pulse_comp_gain;
end
%% Constructor
methods
    function obj = param_manager(paramStruct)
        warning('off','MATLAB:structOnObject')
%         obj = param_manager();
        if nargin > 0
            structFields = fieldnames(paramStruct);
            objFields = fieldnames(obj);
            constIdx = ismember(structFields,'const');
            listIdx = ismember(structFields,'updatedList');
            oriIdx = ismember(structFields,'initial_param');
            structFields{constIdx} = [];
            structFields{listIdx} = [];
            structFields{oriIdx} = [];
            for ii = 1:length(structFields)
                if any(strcmp(structFields{ii},objFields)) && ...
                        ~isstring(paramStruct.(structFields{ii}))
                    obj.(structFields{ii}) = paramStruct.(structFields{ii});
                end
            end
        end
            
    end
end
%% Setter functions
methods
    %% Target Parameter Setters
    function set.tgt_rng(obj,val)
        obj.tgt_rng = val;
        obj.checkIfUpdated('tgt_rng',val);
    end
    function set.tgt_vel(obj,val)
        obj.tgt_vel = val;
        obj.checkIfUpdated('tgt_vel',val);
    end
    function set.tgt_rcs(obj,val)
        obj.tgt_rcs = val;
        obj.checkIfUpdated('tgt_rcs',val);
    end
    %% Waveform Parameter Setters
    function set.sig_bandwidth(obj,val)
        obj.sig_bandwidth = val;
        obj.checkIfUpdated('sig_bandwidth',val);
    end
    function set.sig_freq_tx(obj,val)
        obj.sig_freq_tx = val;
        obj.checkIfUpdated('sig_freq_tx',val);
    end
    function set.sig_wavelength(obj,val)
        obj.sig_wavelength = val;
        obj.checkIfUpdated('sig_wavelength',val);
    end
    function set.sig_prf(obj,val)
        obj.sig_prf = val;
        obj.checkIfUpdated('sig_prf',val);
    end
    function set.sig_pri(obj,val)
        obj.sig_pri = val;
        obj.checkIfUpdated('sig_pri',val);
    end
end
%% Private methods
methods (Access = private)
    %% TODO: updateDependentParams()
    function updateDependentParams(obj,paramName,paramVal)
        % Input parameter is empty. Going through the switch case with
        % empty values would break things, so break from the function
        if isempty(paramVal)
            return
        end
        % Get the param that the user initially set. This will be used as a
        % base case to break out of the recursive loop this function causes
        if isequal(paramName,obj.updatedList{1})
            obj.initial_param = paramName;
        end
        switch paramName
            %% Waveform Parameter Updates
            % Waveform bandwidth changed
            case 'sig_bandwidth'
                if ~isempty(obj.sig_comp_pulsewidth)
                    obj.sig_pulse_comp_gain = obj.sig_bandwidth*obj.sig_comp_pulsewidth;
                end
                obj.pwr_noise = obj.const.k*obj.const.T0_k*obj.sig_bandwidth;
                
            % Center frequency changed
            case 'sig_freq_tx'
                obj.sig_wavelength = obj.const.c/obj.sig_freq_tx;
                
            % PRF changed
            case 'sig_prf'
                obj.sig_pri = 1/obj.sig_prf;
                
            % PRI changed
            case 'sig_pri'
                obj.sig_prf = 1/obj.sig_pri;
                if ~isempty(obj.sig_comp_pulsewidth)
                    obj.duty_cycle = obj.sig_comp_pulsewidth/obj.sig_pri;
                end
                
            % Wavelength changed
            case 'sig_wavelength'
                obj.sig_freq_tx = obj.const.c/obj.sig_wavelength;
                
        end
        % Original parameter has updated all its dependent parameters. 
        % Empty the list of parameters that have already been updated for
        % the next assignment
        if strcmp(obj.initial_param,paramName)
            obj.updatedList = {};
        end
    end
    %% TODO: checkIfUpdated()
    function checkIfUpdated(obj,paramName,paramVal)
        if ~any(strcmp(paramName, obj.updatedList))
            obj.updatedList{end+1} = paramName;
            obj.updateDependentParams(paramName,paramVal)
        end
    end
end
end
