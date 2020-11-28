#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: PCFM
# Author: Shane Flandermeyer
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
import dragon

from gnuradio import qtgui

class pcfm(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "PCFM")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("PCFM")
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

        self.settings = Qt.QSettings("GNU Radio", "pcfm")

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
        self.samp_rate = samp_rate = 100e6
        self.samp_interval = samp_interval = 1/samp_rate
        self.oversampling = oversampling = 5
        self.code_len = code_len = 200
        self.chip_width = chip_width = samp_interval*oversampling
        self.pulse_width = pulse_width = chip_width*code_len
        self.bandwidth = bandwidth = code_len/pulse_width

        ##################################################
        # Blocks
        ##################################################
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
            code_len*oversampling, #size
            samp_rate, #samp_rate
            'Phase Function Comparison', #name
            2 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Phase', 'radians')

        self.qtgui_time_sink_x_0.enable_tags(True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0.enable_grid(True)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)


        labels = ['PCFM', 'P4', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.qtgui_sink_x_0 = qtgui.sink_c(
            1024, #fftsize
            firdes.WIN_BLACKMAN_hARRIS, #wintype
            0, #fc
            samp_rate, #bw
            'Complex-Baseband Waveform', #name
            True, #plotfreq
            True, #plotwaterfall
            True, #plottime
            True #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)

        self.qtgui_sink_x_0.enable_rf_freq(False)

        self.top_grid_layout.addWidget(self._qtgui_sink_x_0_win)
        self.dragon_phase_code_generator_0 = dragon.phase_code_generator("P4", code_len, True, 1, [])
        self.dragon_pcfm_mod_0 = dragon.pcfm_mod(dragon.cpm.GAUSSIAN, code_len, int(oversampling), int(samp_rate))
        self.dragon_oversample_vector_0 = dragon.oversample_vector(code_len, oversampling)
        self.dragon_complex_exponential_0 = dragon.complex_exponential()
        self.blocks_vector_to_stream_0_0 = blocks.vector_to_stream(gr.sizeof_float*1, code_len*oversampling)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_vector_to_stream_0_0, 0), (self.qtgui_time_sink_x_0, 1))
        self.connect((self.dragon_complex_exponential_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.dragon_oversample_vector_0, 0), (self.blocks_vector_to_stream_0_0, 0))
        self.connect((self.dragon_pcfm_mod_0, 0), (self.dragon_complex_exponential_0, 0))
        self.connect((self.dragon_pcfm_mod_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.dragon_phase_code_generator_0, 0), (self.dragon_oversample_vector_0, 0))
        self.connect((self.dragon_phase_code_generator_0, 0), (self.dragon_pcfm_mod_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "pcfm")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samp_interval(1/self.samp_rate)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)

    def get_samp_interval(self):
        return self.samp_interval

    def set_samp_interval(self, samp_interval):
        self.samp_interval = samp_interval
        self.set_chip_width(self.samp_interval*self.oversampling)

    def get_oversampling(self):
        return self.oversampling

    def set_oversampling(self, oversampling):
        self.oversampling = oversampling
        self.set_chip_width(self.samp_interval*self.oversampling)

    def get_code_len(self):
        return self.code_len

    def set_code_len(self, code_len):
        self.code_len = code_len
        self.set_bandwidth(self.code_len/self.pulse_width)
        self.set_pulse_width(self.chip_width*self.code_len)

    def get_chip_width(self):
        return self.chip_width

    def set_chip_width(self, chip_width):
        self.chip_width = chip_width
        self.set_pulse_width(self.chip_width*self.code_len)

    def get_pulse_width(self):
        return self.pulse_width

    def set_pulse_width(self, pulse_width):
        self.pulse_width = pulse_width
        self.set_bandwidth(self.code_len/self.pulse_width)

    def get_bandwidth(self):
        return self.bandwidth

    def set_bandwidth(self, bandwidth):
        self.bandwidth = bandwidth





def main(top_block_cls=pcfm, options=None):

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
