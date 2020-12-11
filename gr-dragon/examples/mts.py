#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
# GNU Radio version: 3.8.2.0

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import analog
from gnuradio import blocks
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import dragon
import pdu_utils
import pmt

from gnuradio import qtgui

class mts(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Not titled yet")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Not titled yet")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "mts")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.sweep_time = sweep_time = 40e-6
        self.prf = prf = 5000
        self.tx_freq = tx_freq = 2.4e9
        self.tgt_vel = tgt_vel = [0]
        self.tgt_rng = tgt_rng = [5000]
        self.tgt_rcs = tgt_rcs = [100]
        self.samp_rate = samp_rate = 20e6
        self.lfm_bandwidth = lfm_bandwidth = 5e6
        self.duty_cycle = duty_cycle = sweep_time*prf

        ##################################################
        # Blocks
        ##################################################
        self.qtgui_sink_x_0 = qtgui.sink_c(
            1024, #fftsize
            firdes.WIN_BLACKMAN_hARRIS, #wintype
            0, #fc
            samp_rate, #bw
            "", #name
            True, #plotfreq
            True, #plotwaterfall
            False, #plottime
            True #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)

        self.qtgui_sink_x_0.enable_rf_freq(False)

        self.top_grid_layout.addWidget(self._qtgui_sink_x_0_win)
        self.pdu_utils_tags_to_pdu_X_0 = pdu_utils.tags_to_pdu_c(pmt.intern('SOB'), pmt.intern('EOB'), 1024, samp_rate, [], False, 0, 0.0)
        self.pdu_utils_tags_to_pdu_X_0.set_eob_parameters(1, 0)
        self.pdu_utils_tags_to_pdu_X_0.enable_time_debug(False)
        self.pdu_utils_pdu_to_bursts_X_0 = pdu_utils.pdu_to_bursts_c(pdu_utils.EARLY_BURST_BEHAVIOR__APPEND, 2048)
        self.dragon_moving_target_sim_0 = dragon.moving_target_sim(tgt_rng, tgt_vel, tgt_rcs, tx_freq, prf, samp_rate)
        self.dragon_lfm_source_0 = dragon.lfm_source('Upchirp', lfm_bandwidth, sweep_time, samp_rate, prf, True, True, True, ["SOB","EOB"])
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*1, '/home/shane/chirpRx.dat', False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.01, 0)



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.dragon_moving_target_sim_0, 'out'), (self.pdu_utils_pdu_to_bursts_X_0, 'bursts'))
        self.msg_connect((self.pdu_utils_tags_to_pdu_X_0, 'pdu_out'), (self.dragon_moving_target_sim_0, 'in'))
        self.connect((self.analog_noise_source_x_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.pdu_utils_tags_to_pdu_X_0, 0))
        self.connect((self.dragon_lfm_source_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.pdu_utils_pdu_to_bursts_X_0, 0), (self.blocks_add_xx_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "mts")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_sweep_time(self):
        return self.sweep_time

    def set_sweep_time(self, sweep_time):
        self.sweep_time = sweep_time
        self.set_duty_cycle(self.sweep_time*self.prf)

    def get_prf(self):
        return self.prf

    def set_prf(self, prf):
        self.prf = prf
        self.set_duty_cycle(self.sweep_time*self.prf)
        self.dragon_moving_target_sim_0.set_prf(self.prf)

    def get_tx_freq(self):
        return self.tx_freq

    def set_tx_freq(self, tx_freq):
        self.tx_freq = tx_freq
        self.dragon_moving_target_sim_0.set_tx_freq(self.tx_freq)

    def get_tgt_vel(self):
        return self.tgt_vel

    def set_tgt_vel(self, tgt_vel):
        self.tgt_vel = tgt_vel
        self.dragon_moving_target_sim_0.set_tgt_vel(self.tgt_vel)

    def get_tgt_rng(self):
        return self.tgt_rng

    def set_tgt_rng(self, tgt_rng):
        self.tgt_rng = tgt_rng
        self.dragon_moving_target_sim_0.set_tgt_rng(self.tgt_rng)

    def get_tgt_rcs(self):
        return self.tgt_rcs

    def set_tgt_rcs(self, tgt_rcs):
        self.tgt_rcs = tgt_rcs
        self.dragon_moving_target_sim_0.set_tgt_rcs(self.tgt_rcs)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.pdu_utils_tags_to_pdu_X_0.set_rate(self.samp_rate)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)

    def get_lfm_bandwidth(self):
        return self.lfm_bandwidth

    def set_lfm_bandwidth(self, lfm_bandwidth):
        self.lfm_bandwidth = lfm_bandwidth

    def get_duty_cycle(self):
        return self.duty_cycle

    def set_duty_cycle(self, duty_cycle):
        self.duty_cycle = duty_cycle





def main(top_block_cls=mts, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    def quitting():
        tb.stop()
        tb.wait()

    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()

if __name__ == '__main__':
    main()
