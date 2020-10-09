#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 Shane Flandermeyer.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#


import numpy as np
import scipy.constants as sc
from gnuradio import gr
import pmt


# Usage: shifted_pulse = self.scale_and_shift_pulse(pulse,scale_factors,
# delays)
# Given an input pulse and arrays containing complex scaling actors and
# delays for the given targets, return the superposition of all scaled
# and shifted returns
def scale_and_shift_pulse(pulse, scale_factors, delays):
    if isinstance(delays, int):
        delays = np.array([delays])
    shifted_pulse = np.zeros((len(pulse) + np.max(delays)),
                             dtype=np.complex64)
    for ii in range(len(scale_factors)):
        shifted_pulse += np.hstack((np.zeros((delays[ii]),
                                             dtype=np.complex64),
                                    pulse * scale_factors[ii],
                                    np.zeros((np.max(delays) - delays[ii]),
                                             dtype=np.complex64)))
    return shifted_pulse


class moving_target_sim_py(gr.basic_block):
    """
    docstring for block moving_target_sim_py
    """

    def __init__(self, tgt_rng, tgt_vel, tgt_rcs, sig_freq_tx, sig_prf,
                 samp_rate, start_tag_key, end_tag_key,pad_pulses,
                 sig_comp_pulsewidth):
        gr.basic_block.__init__(self,
                                name="moving_target_sim_py",
                                in_sig=[np.complex64],
                                out_sig=[np.complex64]),
        self.tgt_rng = np.asarray(tgt_rng, dtype=np.float32)
        self.tgt_vel = np.asarray(tgt_vel, dtype=np.float32)
        self.tgt_rcs = np.asarray(tgt_rcs, dtype=np.float32)
        self.sig_freq_tx = sig_freq_tx
        self.sig_wavelength = sc.c / sig_freq_tx
        self.sig_prf = sig_prf
        self.sig_pri = 1 / sig_prf
        self.samp_rate = samp_rate
        self.start_key = start_tag_key
        self.end_key = end_tag_key
        self.is_burst = 0
        self.pulse_count = 0
        self.tag_count = 0
        self.sig_comp_pulsewidth = sig_comp_pulsewidth
        self.pad_pulses = pad_pulses
        self.pulse = None

    def forecast(self, noutput_items, ninput_items_required):
        # setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = noutput_items

    def general_work(self, input_items, output_items):
        in0 = input_items[0][:len(output_items[0])]
        out = output_items[0]
        if self.pulse is not None:
            # Output buffer has enough room for the entire pulse, so hand it
            # off and return
            if len(out) > len(self.pulse):
                out[:len(self.pulse)] = self.pulse
                pulse_len = len(self.pulse)
                self.pulse = None
                return pulse_len
            # Output buffer is too small for the entire pulse, so only hand
            # off as much as
            else:
                out[:] = self.pulse[:len(out)]
                self.pulse = self.pulse[len(out):]
                return len(out)
        rel_start = self.nitems_read(0)
        rel_end = rel_start + len(in0)
        tags = self.get_tags_in_range(0, rel_start, rel_end)
        if len(tags) == 0:
            if not self.is_burst:
                out = []
            else:
                sf = self.get_tgt_scale_factors()
                self.pulse = scale_and_shift_pulse(in0, sf, 0)
            self.consume_each(len(in0))
        elif len(tags) == 1:
            if not self.is_burst:
                self.pulse_count += 1
                self.tgt_rng += self.tgt_vel * self.sig_pri
                sf = self.get_tgt_scale_factors()
                delay_samps = self.get_tgt_delay_samps()
                rel_start = tags[0].offset - self.nitems_read(0)
                rel_stop = len(in0)
                pulse = in0[rel_start:rel_stop]
                self.pulse = scale_and_shift_pulse(pulse, sf, delay_samps)
                self.is_burst = 1
            else:
                sf = self.get_tgt_scale_factors()
                delay_samps = self.get_tgt_delay_samps()
                rel_start = 0
                rel_stop = tags[0].offset - self.nitems_read(0) - 1
                pulse = in0[rel_start:rel_stop]
                self.pulse = scale_and_shift_pulse(pulse, sf, 0)
                self.is_burst = 0
                if self.pad_pulses:
                    num_zeros = self.get_padding_zeros() - np.max(delay_samps)
                    self.pulse = np.hstack((self.pulse, np.zeros(num_zeros,
                                                                 dtype=np.complex64)))

            self.consume_each(len(in0))
        elif len(tags) == 2:
            if not self.is_burst:
                self.pulse_count += 1
                self.tgt_rng += self.tgt_vel * self.sig_pri
                sf = self.get_tgt_scale_factors()
                delay_samps = self.get_tgt_delay_samps()
                rel_start = tags[0].offset - self.nitems_read(0)
                rel_stop = tags[1].offset - self.nitems_read(0)
                pulse = in0[rel_start:rel_stop]
                self.pulse = scale_and_shift_pulse(pulse, sf, delay_samps)
                if self.pad_pulses:
                    num_zeros = self.get_padding_zeros() - np.max(delay_samps)
                    self.pulse = np.hstack((self.pulse, np.zeros(num_zeros,
                                                                 dtype=np.complex64)))
                self.consume_each(len(in0))
            else:
                sf = self.get_tgt_scale_factors()
                delay_samps = self.get_tgt_delay_samps()
                rel_start = 0
                rel_stop = tags[0].offset - self.nitems_read(0)
                pulse = in0[rel_start:rel_stop]
                self.pulse = scale_and_shift_pulse(pulse, sf, 0)
                if self.pad_pulses:
                    num_zeros = self.get_padding_zeros() - np.max(delay_samps)
                    self.pulse = np.hstack((self.pulse, np.zeros(num_zeros,
                                                                 dtype=np.complex64)))
                    # FIXME: Might be off by one
                self.consume_each(tags[1].offset - 1 - self.nitems_read(0))
        else:
            if not self.is_burst:
                self.pulse_count += 1
                self.tgt_rng += self.tgt_vel * self.sig_pri
                sf = self.get_tgt_scale_factors()
                delay_samps = self.get_tgt_delay_samps()
                rel_start = tags[0].offset - self.nitems_read(0)
                rel_stop = tags[1].offset - self.nitems_read(0)
                pulse = in0[rel_start:rel_stop]
                self.pulse = scale_and_shift_pulse(pulse, sf, delay_samps)
                if self.pad_pulses:
                    num_zeros = self.get_padding_zeros() - np.max(delay_samps)
                    self.pulse = np.hstack((self.pulse, np.zeros(num_zeros,
                                                                 dtype=np.complex64)))
                self.consume_each(tags[2].offset - 1 - self.nitems_read(0))
            else:
                sf = self.get_tgt_scale_factors()
                delay_samps = self.get_tgt_delay_samps()
                rel_start = 0
                rel_stop = tags[0].offset - self.nitems_read(0)
                pulse = in0[rel_start:rel_stop]
                self.pulse = scale_and_shift_pulse(pulse, sf, 0)
                self.is_burst = 0
                if self.pad_pulses:
                    num_zeros = self.get_padding_zeros() - np.max(delay_samps)
                    self.pulse = np.hstack((self.pulse, np.zeros(num_zeros,
                                                                 dtype=np.complex64)))
                self.consume_each(tags[1].offset - 1 - self.nitems_read(0))
        return 0

    def get_tgt_scale_factors(self):
        amplitude_scaling = np.sqrt(
            self.sig_wavelength ** 2 * 2 * self.tgt_rcs /
            ((4 * sc.pi) ** 3 * self.tgt_rng ** 4))
        dopp_freq = 2 * self.tgt_vel / self.sig_wavelength
        dopp_shift = np.exp(
            1j * 2 * sc.pi * dopp_freq * self.sig_pri * self.pulse_count)
        sf = amplitude_scaling * dopp_shift
        return sf

    # Usage: delay_samps = self.get_tgt_delay_samps
    # Get the number of samples to delay for each target
    def get_tgt_delay_samps(self):
        return np.asarray(np.round(2 * self.samp_rate * (abs(self.tgt_rng) /
                                                         sc.c)),
                          dtype=int)

    def get_padding_zeros(self):
        return int(np.round((self.sig_pri - self.sig_comp_pulsewidth) *
                            self.samp_rate))
