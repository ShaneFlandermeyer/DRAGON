#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
# GNU Radio version: 3.8.2.0

from gnuradio import analog
from gnuradio import blocks
import pmt
from gnuradio import gr
from gnuradio.filter import firdes
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import dragon


class test_mts(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Not titled yet")

        ##################################################
        # Variables
        ##################################################
        self.tgt_vel = tgt_vel = [100,-100]
        self.tgt_rng = tgt_rng = [1000,14000]
        self.tgt_rcs = tgt_rcs = [1,100]
        self.sig_prf = sig_prf = 5000
        self.sig_freq_tx = sig_freq_tx = 2.4e9
        self.samp_rate = samp_rate = 20e6

        ##################################################
        # Blocks
        ##################################################
        self.dragon_moving_target_sim_py_0 = dragon.moving_target_sim_py(tgt_rng, tgt_vel, tgt_rcs, sig_freq_tx, sig_prf, samp_rate,
        "SOB", "EOB", True, 100e-6)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_float_to_short_0 = blocks.float_to_short(1, 1)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_gr_complex*1, '/home/shane/chirpTx.dat', False, 0, 0)
        self.blocks_file_source_0.set_begin_tag(pmt.PMT_NIL)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*1, '/home/shane/chirpRx.dat', False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(1)
        self.blocks_burst_tagger_0 = blocks.burst_tagger(gr.sizeof_gr_complex)
        self.blocks_burst_tagger_0.set_true_tag('SOB',True)
        self.blocks_burst_tagger_0.set_false_tag('EOB',False)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.blocks_add_const_vxx_0 = blocks.add_const_ff(0)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.1585, 0)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_add_const_vxx_0, 0), (self.blocks_float_to_short_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.dragon_moving_target_sim_py_0, 0))
        self.connect((self.blocks_burst_tagger_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_complex_to_mag_0, 0), (self.blocks_add_const_vxx_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_burst_tagger_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_float_to_short_0, 0), (self.blocks_burst_tagger_0, 1))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_complex_to_mag_0, 0))
        self.connect((self.dragon_moving_target_sim_py_0, 0), (self.blocks_file_sink_0, 0))


    def get_tgt_vel(self):
        return self.tgt_vel

    def set_tgt_vel(self, tgt_vel):
        self.tgt_vel = tgt_vel

    def get_tgt_rng(self):
        return self.tgt_rng

    def set_tgt_rng(self, tgt_rng):
        self.tgt_rng = tgt_rng

    def get_tgt_rcs(self):
        return self.tgt_rcs

    def set_tgt_rcs(self, tgt_rcs):
        self.tgt_rcs = tgt_rcs

    def get_sig_prf(self):
        return self.sig_prf

    def set_sig_prf(self, sig_prf):
        self.sig_prf = sig_prf

    def get_sig_freq_tx(self):
        return self.sig_freq_tx

    def set_sig_freq_tx(self, sig_freq_tx):
        self.sig_freq_tx = sig_freq_tx

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)





def main(top_block_cls=test_mts, options=None):
    tb = top_block_cls()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        sys.exit(0)

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    tb.start()

    tb.wait()


if __name__ == '__main__':
    main()
