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
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import dragon
import pdu_utils

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
        self.pri = pri = 200e-6
        self.duty_factor = duty_factor = 0.1
        self.sweep_time = sweep_time = duty_factor*pri
        self.samp_rate = samp_rate = 20e6
        self.filt_len = filt_len = int(samp_rate*sweep_time)
        self.sig_len = sig_len = filt_len
        self.prf = prf = 1/pri
        self.out_len = out_len = sig_len+filt_len-1
        self.bandwidth = bandwidth = 10e6

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
            True, #plottime
            True #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)

        self.qtgui_sink_x_0.enable_rf_freq(False)

        self.top_grid_layout.addWidget(self._qtgui_sink_x_0_win)
        self.qtgui_edit_box_msg_0 = qtgui.edit_box_msg(qtgui.FLOAT, '', '', True, False, 'bandwidth')
        self._qtgui_edit_box_msg_0_win = sip.wrapinstance(self.qtgui_edit_box_msg_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_edit_box_msg_0_win)
        self.pdu_utils_pdu_add_noise_0 = pdu_utils.pdu_add_noise(0.01, 0.0, 1.0, 123456789, pdu_utils.UNIFORM)
        self.dragon_pdu_to_stream_X_0 = dragon.pdu_to_stream_c(dragon.EARLY_BURST_BEHAVIOR__APPEND, 64, True, True)
        self.dragon_pdu_lfm_0 = dragon.pdu_lfm(bandwidth, sweep_time, samp_rate, prf,dragon.TX_MODE__SIMULATION,True)



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.dragon_pdu_lfm_0, 'out'), (self.pdu_utils_pdu_add_noise_0, 'pdu_in'))
        self.msg_connect((self.pdu_utils_pdu_add_noise_0, 'pdu_out'), (self.dragon_pdu_to_stream_X_0, 'in'))
        self.msg_connect((self.qtgui_edit_box_msg_0, 'msg'), (self.dragon_pdu_lfm_0, 'ctrl'))
        self.connect((self.dragon_pdu_to_stream_X_0, 0), (self.qtgui_sink_x_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "lfm_source")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_pri(self):
        return self.pri

    def set_pri(self, pri):
        self.pri = pri
        self.set_prf(1/self.pri)
        self.set_sweep_time(self.duty_factor*self.pri)

    def get_duty_factor(self):
        return self.duty_factor

    def set_duty_factor(self, duty_factor):
        self.duty_factor = duty_factor
        self.set_sweep_time(self.duty_factor*self.pri)

    def get_sweep_time(self):
        return self.sweep_time

    def set_sweep_time(self, sweep_time):
        self.sweep_time = sweep_time
        self.set_filt_len(int(self.samp_rate*self.sweep_time))
        self.dragon_pdu_lfm_0.set_sweep_time(self.sweep_time)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_filt_len(int(self.samp_rate*self.sweep_time))
        self.dragon_pdu_lfm_0.set_samp_rate(self.samp_rate)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)

    def get_filt_len(self):
        return self.filt_len

    def set_filt_len(self, filt_len):
        self.filt_len = filt_len
        self.set_out_len(self.sig_len+self.filt_len-1)
        self.set_sig_len(self.filt_len)

    def get_sig_len(self):
        return self.sig_len

    def set_sig_len(self, sig_len):
        self.sig_len = sig_len
        self.set_out_len(self.sig_len+self.filt_len-1)

    def get_prf(self):
        return self.prf

    def set_prf(self, prf):
        self.prf = prf
        self.dragon_pdu_lfm_0.set_prf(self.prf)

    def get_out_len(self):
        return self.out_len

    def set_out_len(self, out_len):
        self.out_len = out_len

    def get_bandwidth(self):
        return self.bandwidth

    def set_bandwidth(self, bandwidth):
        self.bandwidth = bandwidth
        self.dragon_pdu_lfm_0.set_bandwidth(self.bandwidth)





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
