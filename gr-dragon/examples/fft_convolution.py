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
import sip
from gnuradio import blocks
from gnuradio import gr
from gnuradio.filter import firdes
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio.qtgui import Range, RangeWidget
import dragon
from gnuradio.fft import window
import pdu_utils
import threading

from gnuradio import qtgui

class fft_convolution(gr.top_block, Qt.QWidget):

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

        self.settings = Qt.QSettings("GNU Radio", "fft_convolution")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        self._lock = threading.RLock()

        ##################################################
        # Variables
        ##################################################
        self.pri = pri = 200e-6
        self.duty_factor = duty_factor = 0.1
        self.sweep_time = sweep_time = duty_factor*pri
        self.samp_rate = samp_rate = 20e6
        self.sig_len = sig_len = int(samp_rate*sweep_time)
        self.filt_len = filt_len = int(samp_rate*sweep_time)
        self.prf = prf = 1/pri
        self.fft_len = fft_len = sig_len+filt_len-1+225
        self.bandwidth = bandwidth = 5e6

        ##################################################
        # Blocks
        ##################################################
        self._bandwidth_range = Range(0, 100e6, 1, 5e6, 200)
        self._bandwidth_win = RangeWidget(self._bandwidth_range, self.set_bandwidth, 'bandwidth', "counter_slider", float)
        self.top_grid_layout.addWidget(self._bandwidth_win)
        self.qtgui_vector_sink_f_0 = qtgui.vector_sink_f(
            fft_len,
            0,
            1.0,
            'delay',
            'magnitude',
            "",
            1 # Number of inputs
        )
        self.qtgui_vector_sink_f_0.set_update_time(0.10)
        self.qtgui_vector_sink_f_0.set_y_axis(-140, 10)
        self.qtgui_vector_sink_f_0.enable_autoscale(True)
        self.qtgui_vector_sink_f_0.enable_grid(True)
        self.qtgui_vector_sink_f_0.set_x_axis_units("")
        self.qtgui_vector_sink_f_0.set_y_axis_units("")
        self.qtgui_vector_sink_f_0.set_ref_level(0)

        labels = ['', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
            "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_vector_sink_f_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_vector_sink_f_0.set_line_label(i, labels[i])
            self.qtgui_vector_sink_f_0.set_line_width(i, widths[i])
            self.qtgui_vector_sink_f_0.set_line_color(i, colors[i])
            self.qtgui_vector_sink_f_0.set_line_alpha(i, alphas[i])

        self._qtgui_vector_sink_f_0_win = sip.wrapinstance(self.qtgui_vector_sink_f_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_vector_sink_f_0_win)
        self.dragon_pdu_to_stream_X_0_0 = dragon.pdu_to_stream_c(dragon.EARLY_BURST_BEHAVIOR__APPEND, 64, True, True)
        self.dragon_pdu_to_stream_X_0 = dragon.pdu_to_stream_c(dragon.EARLY_BURST_BEHAVIOR__APPEND, 64, True, True)
        self.dragon_pdu_lfm_0 = dragon.pdu_lfm(bandwidth, sweep_time, samp_rate, prf,dragon.TX_MODE__LOOPBACK,True)
        self.dragon_pad_vector_0_0 = dragon.pad_vector(sig_len, fft_len)
        self.dragon_pad_vector_0 = dragon.pad_vector(sig_len, fft_len)
        self.dragon_convolve_fft_hier_0 = dragon.convolve_fft_hier(fft_len, window.rectangular(fft_len), 1)
        self.dragon_cl_db_0 = dragon.cl_db(1, 1, 1, 1, dragon.VOLTAGE, 0)
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_float*1, fft_len)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex*fft_len, samp_rate,True)
        self.blocks_stream_to_vector_1 = blocks.stream_to_vector(gr.sizeof_float*1, fft_len)
        self.blocks_stream_to_vector_0_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, sig_len)
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, sig_len)
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(fft_len)



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.dragon_pdu_lfm_0, 'out'), (self.dragon_pdu_to_stream_X_0, 'in'))
        self.msg_connect((self.dragon_pdu_lfm_0, 'filter'), (self.dragon_pdu_to_stream_X_0_0, 'in'))
        self.connect((self.blocks_complex_to_mag_0, 0), (self.blocks_vector_to_stream_0, 0))
        self.connect((self.blocks_stream_to_vector_0, 0), (self.dragon_pad_vector_0, 0))
        self.connect((self.blocks_stream_to_vector_0_0, 0), (self.dragon_pad_vector_0_0, 0))
        self.connect((self.blocks_stream_to_vector_1, 0), (self.qtgui_vector_sink_f_0, 0))
        self.connect((self.blocks_throttle_0_0, 0), (self.blocks_complex_to_mag_0, 0))
        self.connect((self.blocks_vector_to_stream_0, 0), (self.dragon_cl_db_0, 0))
        self.connect((self.dragon_cl_db_0, 0), (self.blocks_stream_to_vector_1, 0))
        self.connect((self.dragon_convolve_fft_hier_0, 0), (self.blocks_throttle_0_0, 0))
        self.connect((self.dragon_pad_vector_0, 0), (self.dragon_convolve_fft_hier_0, 1))
        self.connect((self.dragon_pad_vector_0_0, 0), (self.dragon_convolve_fft_hier_0, 0))
        self.connect((self.dragon_pdu_to_stream_X_0, 0), (self.blocks_stream_to_vector_0, 0))
        self.connect((self.dragon_pdu_to_stream_X_0_0, 0), (self.blocks_stream_to_vector_0_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "fft_convolution")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_pri(self):
        return self.pri

    def set_pri(self, pri):
        with self._lock:
            self.pri = pri
            self.set_prf(1/self.pri)
            self.set_sweep_time(self.duty_factor*self.pri)

    def get_duty_factor(self):
        return self.duty_factor

    def set_duty_factor(self, duty_factor):
        with self._lock:
            self.duty_factor = duty_factor
            self.set_sweep_time(self.duty_factor*self.pri)

    def get_sweep_time(self):
        return self.sweep_time

    def set_sweep_time(self, sweep_time):
        with self._lock:
            self.sweep_time = sweep_time
            self.set_filt_len(int(self.samp_rate*self.sweep_time))
            self.set_sig_len(int(self.samp_rate*self.sweep_time))
            self.dragon_pdu_lfm_0.set_sweep_time(self.sweep_time)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        with self._lock:
            self.samp_rate = samp_rate
            self.set_filt_len(int(self.samp_rate*self.sweep_time))
            self.set_sig_len(int(self.samp_rate*self.sweep_time))
            self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)
            self.dragon_pdu_lfm_0.set_samp_rate(self.samp_rate)

    def get_sig_len(self):
        return self.sig_len

    def set_sig_len(self, sig_len):
        with self._lock:
            self.sig_len = sig_len
            self.set_fft_len(self.sig_len+self.filt_len-1+225)

    def get_filt_len(self):
        return self.filt_len

    def set_filt_len(self, filt_len):
        with self._lock:
            self.filt_len = filt_len
            self.set_fft_len(self.sig_len+self.filt_len-1+225)

    def get_prf(self):
        return self.prf

    def set_prf(self, prf):
        with self._lock:
            self.prf = prf
            self.dragon_pdu_lfm_0.set_prf(self.prf)

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        with self._lock:
            self.fft_len = fft_len

    def get_bandwidth(self):
        return self.bandwidth

    def set_bandwidth(self, bandwidth):
        with self._lock:
            self.bandwidth = bandwidth
            self.dragon_pdu_lfm_0.set_bandwidth(self.bandwidth)





def main(top_block_cls=fft_convolution, options=None):

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
