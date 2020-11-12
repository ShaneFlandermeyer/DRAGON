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


class lfm_source(gr.sync_block):
    """
    docstring for block lfm_source
    """

    def __init__(self, chirp_type, bandwidth, sweep_time, samp_rate, prf, append_zeros, repeat,
                 add_tags, tags):
        gr.sync_block.__init__(self,
                               name="lfm_source",
                               in_sig=None,
                               out_sig=[np.complex64]),
        self.chirp_type = chirp_type
        self.bandwidth = bandwidth
        self.sweep_time = sweep_time
        self.samp_rate = samp_rate
        self.prf = prf
        self.append_zeros = append_zeros
        self.repeat = repeat
        self.add_tags = add_tags
        self.tags = tags
        self.pri = 1 / prf
        self.items_remaining = 0
        self.pulse_count = 0
        self.waveform = []

    def work(self, input_items, output_items):
        out = output_items[0]
        noutput_items = len(out)
        free_buffer_space = noutput_items
        # No more waveform data to output. Generate a new waveform
        if self.items_remaining == 0:
            # Only generate a new waveform if repeat setting is active or if
            # repeat is not active but we have not generated one
            if self.repeat or (not self.repeat and self.pulse_count == 0):
                self.chirp()
                if self.add_tags:
                    num_zeros = int((self.pri - self.sweep_time) * self.samp_rate)
                    self.add_item_tag(0, self.nitems_written(0), pmt.intern(self.tags[0]),
                                      pmt.to_pmt(self.waveform.size - num_zeros))
                    self.add_item_tag(0, self.nitems_written(0) + self.waveform.size,
                                      pmt.intern(self.tags[1]), pmt.to_pmt(num_zeros))
        # Keep count of where the free space of the output buffer begins
        out_idx = 0
        # There's enough space to add items to the output buffer. Continue
        # processing.
        while free_buffer_space > 0:
            nitems_to_read = min(free_buffer_space, self.items_remaining)
            free_buffer_space -= nitems_to_read
            self.items_remaining -= nitems_to_read
            out[out_idx:out_idx + nitems_to_read] = self.waveform[:nitems_to_read]
            out_idx += nitems_to_read
            self.waveform = self.waveform[nitems_to_read:]
            if self.items_remaining == 0:
                if self.repeat:
                    self.chirp()
                    if self.add_tags:
                        if self.append_zeros:
                            num_zeros = int((self.pri - self.sweep_time) * self.samp_rate)
                        else:
                            num_zeros = 0
                        self.add_item_tag(0, self.nitems_written(0) + out_idx,
                                          pmt.intern(self.tags[0]),
                                          pmt.to_pmt(self.waveform.size - num_zeros))
                        self.add_item_tag(0, self.nitems_written(
                            0) + out_idx + self.waveform.size-num_zeros,
                                          pmt.intern(self.tags[1]),
                                          pmt.to_pmt(num_zeros))
                else:
                    break
        return noutput_items - free_buffer_space

    def chirp(self):
        # Define the time support for the waveform from [0,sweep_time-Tadc]
        t = np.arange(0, self.sweep_time - 1 / self.samp_rate, 1 / self.samp_rate)
        # Chirp up from -B/2 to B/2
        if self.chirp_type == "Upchirp":
            phase = -2 * sc.pi * (self.bandwidth / 2) * t + sc.pi * (
                    self.bandwidth / self.sweep_time) * np.power(t, 2)
        # Chirp down from B/2 to B/2
        else:
            phase = 2 * sc.pi * (self.bandwidth / 2) * t - sc.pi * (
                    self.bandwidth / self.sweep_time) * np.power(t, 2)
        # Define the waveform array
        self.waveform = np.exp(1j * phase)
        # Pad the output with zeros based on the PRF and pulse width
        if self.append_zeros:
            num_zeros = int((self.pri - self.sweep_time) * self.samp_rate)
            self.waveform = np.concatenate((self.waveform, np.zeros(num_zeros)))
        # Increment the pulse count and update the number of items left for
        # the output to consume
        self.pulse_count += 1
        self.items_remaining = self.waveform.size
