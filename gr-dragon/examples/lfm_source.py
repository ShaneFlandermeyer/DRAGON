#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
# Author: shane
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
from gnuradio import blocks
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio.qtgui import Range, RangeWidget
import dragon

from gnuradio import qtgui

class lfm_source(gr.top_block, Qt.QWidget):

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

        self.settings = Qt.QSettings("GNU Radio", "lfm_source")

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
        self.prf = prf = 5000
        self.duty_factor = duty_factor = .1
        self.sweep_time = sweep_time = duty_factor/prf
        self.samp_rate = samp_rate = 20e6
        self.pri = pri = 1/prf
        self.bandwidth = bandwidth = 5e6

        ##################################################
        # Blocks
        ##################################################
        self._samp_rate_range = Range(0, 100e6, 1, 20e6, 200)
        self._samp_rate_win = RangeWidget(self._samp_rate_range, self.set_samp_rate, 'Sample Rate', "counter_slider", float)
        self.top_grid_layout.addWidget(self._samp_rate_win)
        self._prf_range = Range(0, 10000, 1, 5000, 200)
        self._prf_win = RangeWidget(self._prf_range, self.set_prf, 'PRF', "counter_slider", float)
        self.top_grid_layout.addWidget(self._prf_win)
        self._bandwidth_range = Range(0, samp_rate, 1, 5e6, 200)
        self._bandwidth_win = RangeWidget(self._bandwidth_range, self.set_bandwidth, 'Bandwidth', "counter_slider", float)
        self.top_grid_layout.addWidget(self._bandwidth_win)
        self.qtgui_sink_x_0 = qtgui.sink_c(
            int(pri*samp_rate), #fftsize
            firdes.WIN_RECTANGULAR, #wintype
            0, #fc
            samp_rate, #bw
            "", #name
            True, #plotfreq
            True, #plotwaterfall
            True, #plottime
            True #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)

        self.qtgui_sink_x_0.enable_rf_freq(False)

        self.top_grid_layout.addWidget(self._qtgui_sink_x_0_win)
        self.lfm_source_0 = dragon.lfm_source(bandwidth, sweep_time, samp_rate, prf, [])
        self._duty_factor_range = Range(0, 1, .01, .1, 200)
        self._duty_factor_win = RangeWidget(self._duty_factor_range, self.set_duty_factor, 'Duty Factor', "counter_slider", float)
        self.top_grid_layout.addWidget(self._duty_factor_win)
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, int(samp_rate*pri))
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_throttle_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.blocks_vector_to_stream_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.lfm_source_0, 0), (self.blocks_vector_to_stream_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "lfm_source")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_prf(self):
        return self.prf

    def set_prf(self, prf):
        self.prf = prf
        self.set_pri(1/self.prf)
        self.set_sweep_time(self.duty_factor/self.prf)
        self.lfm_source_0.setPRF(self.prf)

    def get_duty_factor(self):
        return self.duty_factor

    def set_duty_factor(self, duty_factor):
        self.duty_factor = duty_factor
        self.set_sweep_time(self.duty_factor/self.prf)

    def get_sweep_time(self):
        return self.sweep_time

    def set_sweep_time(self, sweep_time):
        self.sweep_time = sweep_time
        self.lfm_source_0.setSweepTime(self.sweep_time)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.lfm_source_0.setSampRate(self.samp_rate)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)

    def get_pri(self):
        return self.pri

    def set_pri(self, pri):
        self.pri = pri

    def get_bandwidth(self):
        return self.bandwidth

    def set_bandwidth(self, bandwidth):
        self.bandwidth = bandwidth
        self.lfm_source_0.setBandwidth(self.bandwidth)





def main(top_block_cls=lfm_source, options=None):

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
