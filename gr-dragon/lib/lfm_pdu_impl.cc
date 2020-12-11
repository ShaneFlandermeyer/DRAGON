/* -*- c++ -*- */
/*
 * Copyright 2020 gr-dragon author.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "lfm_pdu_impl.h"

namespace gr {
namespace dragon {
lfm_pdu::sptr
lfm_pdu::make(float bandwidth, float sweep_time, float samp_rate, float prf,
              uint32_t mode) {
    return gnuradio::get_initial_sptr
            (new lfm_pdu_impl(bandwidth, sweep_time, samp_rate, prf, mode));
}


/*
 * The private constructor
 */
lfm_pdu_impl::lfm_pdu_impl(float bandwidth, float sweep_time, float samp_rate,
                           float prf, uint32_t mode)
        : block("lfm_pdu",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
          d_bandwidth(bandwidth),
          d_sweep_time(sweep_time),
          d_samp_rate(samp_rate),
          d_prf(prf),
          d_pri(1 / d_prf),
          d_out_port(pmt::mp("out")),
          d_ctrl_port(pmt::mp("ctrl")),
          d_mode(mode) {
    if (d_mode == LFM_GENERATOR_MODE__SIMULATION) {
        d_waveform = pmt::make_c32vector(d_samp_rate * d_pri, 0);
    } else if (d_mode == LFM_GENERATOR_MODE__LOOPBACK){
        d_waveform = pmt::make_c32vector(d_samp_rate * d_sweep_time, 0);
    }
    generate_lfm();
    message_port_register_out(d_out_port);
    message_port_register_in(d_ctrl_port);
    set_msg_handler(d_ctrl_port,
                    [this](pmt::pmt_t msg) { this->handle_ctrl_msg(msg); });


}

void lfm_pdu_impl::handle_ctrl_msg(pmt::pmt_t msg) {
    // If the message isn't a dictionary, do nothing
    if (!pmt::is_dict(msg))
        return;
    // Get the dictionary and convert the message to a dictionary if it's a pair
    try {
        pmt::pmt_t keys = pmt::dict_keys(msg);
        msg = pmt::car(msg);
    } catch (const pmt::wrong_type &e) {
        // So we fix it:
        GR_LOG_WARN(this->d_logger, "got a pair, not a dict, fixing");
        msg = pmt::dict_add(pmt::make_dict(), pmt::car(msg), pmt::cdr(msg));
    }
    // Handle command to change bandwidth
    if (pmt::dict_has_key(msg, pmt::mp("bandwidth"))) {
        float new_bandwidth = pmt::to_float(
                pmt::dict_ref(msg, pmt::mp("bandwidth"),
                              pmt::from_float(d_bandwidth)));
        if (new_bandwidth > 0)
            set_bandwidth(new_bandwidth);
    } else if (pmt::dict_has_key(msg, pmt::mp("samp_rate"))) {
        float new_samp_rate = pmt::to_float(
                pmt::dict_ref(msg, pmt::mp("samp_rate"),
                              pmt::from_float(d_samp_rate)));
        if (new_samp_rate > 0)
            set_samp_rate(new_samp_rate);
    } else if (pmt::dict_has_key(msg, pmt::mp("sweep_time"))) {
        float new_sweep_time = pmt::to_float(
                pmt::dict_ref(msg, pmt::mp("sweep_time"),
                              pmt::from_float(d_sweep_time)));
        if (new_sweep_time > 0)
            set_sweep_time(new_sweep_time);
    } else if (pmt::dict_has_key(msg, pmt::mp("prf"))) {
        float new_prf = pmt::to_float(
                pmt::dict_ref(msg, pmt::mp("prf"),
                              pmt::from_float(d_prf)));
        if (new_prf > 0)
            set_prf(new_prf);
    } else if (pmt::dict_has_key(msg, pmt::mp("pri"))) {
        float new_pri = pmt::to_float(
                pmt::dict_ref(msg, pmt::mp("pri"),
                              pmt::from_float(d_pri)));
        if (new_pri > 0)
            set_pri(new_pri);
    }
    // Generate a new waveform vector
    generate_lfm();

}

void lfm_pdu_impl::run() {
    long sleep_time = static_cast<long>((d_pri) * 1e6);
    while (!d_finished) {
            boost::this_thread::sleep(
                    boost::posix_time::microseconds(sleep_time));
        if (d_finished) {
            return;
        }
        this->message_port_pub(d_out_port,
                               pmt::cons(pmt::make_dict(), d_waveform));
    }
}


bool lfm_pdu_impl::start() {
    // Create a new thread for the block
    d_finished = false;
    d_thread = boost::shared_ptr<gr::thread::thread>(
            new gr::thread::thread(boost::bind
                                           (&lfm_pdu_impl::run,
                                            this)));
    return block::start();
}

bool lfm_pdu_impl::stop() {
    // Shut down the thread
    d_finished = true;
    d_thread->interrupt();
    d_thread->join();

    return block::stop();
}


template<typename T>
std::vector<T> lfm_pdu_impl::arange(T start, T stop, T step) {
    std::vector<T> values;
    for (T value = start; value < stop; value += step)
        values.push_back(value);
    return values;
}

void lfm_pdu_impl::generate_lfm() {
    std::vector<float> t = arange(static_cast<float>(0),
                                  static_cast<float>(d_sweep_time -
                                                     1 / d_samp_rate),
                                  static_cast<float>(1 / d_samp_rate));
    float phase;
    for (int ii = 0; ii < t.size(); ii++) {
        phase = -2 * M_PI * (d_bandwidth / 2) * t[ii] + M_PI * d_bandwidth /
                                                        d_sweep_time *
                                                        std::pow(t[ii], 2);
        pmt::c32vector_set(d_waveform, ii, std::exp(j * phase));
    }
}

void lfm_pdu_impl::set_bandwidth(float bandwidth) {
    gr::thread::scoped_lock l(this->d_setlock);

    d_bandwidth = bandwidth;

}

void lfm_pdu_impl::set_samp_rate(float samp_rate) {
    gr::thread::scoped_lock l(this->d_setlock);

    d_samp_rate = samp_rate;
    d_waveform = pmt::make_c32vector(d_sweep_time * samp_rate, 0);

}

void lfm_pdu_impl::set_sweep_time(float sweep_time) {
    gr::thread::scoped_lock l(this->d_setlock);

    d_sweep_time = sweep_time;
    d_waveform = pmt::make_c32vector(d_samp_rate * sweep_time, 0);

}

void lfm_pdu_impl::set_prf(float prf) {
    gr::thread::scoped_lock l(this->d_setlock);

    d_prf = prf;
    d_pri = 1 / d_prf;

}

void lfm_pdu_impl::set_pri(float pri) {
    gr::thread::scoped_lock l(this->d_setlock);

    d_pri = pri;
    d_prf = 1 / d_pri;

}


/*
 * Our virtual destructor.
 */
lfm_pdu_impl::~lfm_pdu_impl() = default;

} /* namespace dragon */
} /* namespace gr */

