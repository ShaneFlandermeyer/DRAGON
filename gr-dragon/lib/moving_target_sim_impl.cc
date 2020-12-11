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
#include <gnuradio/constants.h>
#include "moving_target_sim_impl.h"
#include "pmt/pmt.h"

namespace gr {
namespace dragon {

moving_target_sim::sptr
moving_target_sim::make(std::vector<float> tgt_rng, std::vector<float> tgt_vel,
                        std::vector<float> tgt_rcs, float tx_freq, float prf,
                        float
                        samp_rate, uint32_t mode) {
    return gnuradio::get_initial_sptr
            (new moving_target_sim_impl(tgt_rng, tgt_vel, tgt_rcs, tx_freq, prf,
                                        samp_rate, mode));
}


/*
 * The private constructor
 */
moving_target_sim_impl::moving_target_sim_impl(std::vector<float> tgt_rng,
                                               std::vector<float> tgt_vel,
                                               std::vector<float> tgt_rcs,
                                               float tx_freq,
                                               float prf,
                                               float samp_rate, uint32_t mode)
        : gr::block("moving_target_sim",
                    gr::io_signature::make(0, 0, 0),
                    gr::io_signature::make(0, 0, 0)),
          d_tgt_rng(tgt_rng),
          d_tgt_vel(tgt_vel),
          d_tgt_rcs(tgt_rcs),
          d_num_tgts(tgt_rng.size()),
          d_tx_freq(tx_freq),
          d_prf(prf),
          d_samp_rate(samp_rate),
          d_wavelength(C / tx_freq),
          d_burst_counter(0),
          d_mode(mode) {
    message_port_register_in(pmt::mp("in"));
    set_msg_handler(pmt::mp("in"),
                    [this](pmt::pmt_t pdu) { this->handle_pdu(pdu); });
    message_port_register_out(pmt::mp("out"));
}

/*
 * Our virtual destructor.
 */
moving_target_sim_impl::~moving_target_sim_impl() {
}

/*
 * Our input message handling function
 */
void moving_target_sim_impl::handle_pdu(pmt::pmt_t pdu) {

    /*
     * Check the PDU formatting
     */
    // Make sure PDU data is formed properly
    if (!(pmt::is_pair(pdu))) {
        GR_LOG_ERROR(this->d_logger, "Received unexpected PMT (non-pair)");
        return;
    }

    pmt::pmt_t meta = pmt::car(pdu);
    pmt::pmt_t v_data = pmt::cdr(pdu);

    // Make sure pmt car is a dictionary. If it isn't, the data vector shouldn't be
    // uniform (not sure why... this error checking code is taken from pdu_to_bursts in
    // PDU utils)
    if (!(is_dict(meta) && pmt::is_uniform_vector(v_data))) {
        GR_LOG_ERROR(this->d_logger, "PMT is not a PDU, dropping");
        return;
    }

    if (pmt::length(v_data) != 0) {
        size_t v_itemsize = pmt::uniform_vector_itemsize(v_data);
        // Make sure the data is complex
        if (v_itemsize != sizeof(gr_complex)) {
            GR_LOG_ERROR(this->d_logger,
                         boost::format(
                                 "PDU received has incorrect itemsize (%d != %d)") %
                                 v_itemsize % sizeof(gr_complex));
            return;
        }
        v_data = simulate_targets(pmt::c32vector_elements(v_data));
        // TODO: Decide what to output for the metadata dictionary below. I'm thinking
        //  we show the target truth information along with the burst timing
        this->message_port_pub(pmt::mp("out"), pmt::cons(meta, v_data));
        // Increment the burst counter and target ranges
        d_burst_counter++;
        for (uint32_t i = 0; i < d_tgt_rng.size(); i++) {
            d_tgt_rng[i] += d_tgt_vel[i] / d_prf;
        }


    }
}

/*
 * Calculate the number of samples to delay each target
 */
std::vector<int> moving_target_sim_impl::get_tgt_delay_samps() {
    std::vector<int> delays(d_num_tgts, 0);
    for (uint32_t i = 0; i < d_num_tgts; i++)
        delays[i] = std::roundf(2 * d_samp_rate * fabs(d_tgt_rng[i]) / C);
    return delays;
}

/*
 * Calculate a vector of complex scale factors corresponding to each target
 */
std::vector<gr_complex> moving_target_sim_impl::get_tgt_scale_factors() {
    // TODO: Implement me!
    std::vector<gr_complex> sf(d_num_tgts);
    gr_complex amplitude_scaling;
    gr_complex dopp_freq;
    gr_complex dopp_shift;
    for (uint32_t i = 0; i < d_num_tgts; i++) {
        amplitude_scaling = sqrtf(std::pow(d_wavelength, 2) * 2 * d_tgt_rcs[i] /
                                  (std::pow(4 * M_PI, 3) * std::pow
                                          (d_tgt_rng[i], 4)));
        dopp_freq = 2 * d_tgt_vel[i] / d_wavelength;
        dopp_shift = std::exp(j * (float) 2 * (float) M_PI * dopp_freq / d_prf *
                              (float) d_burst_counter);
        sf[i] = amplitude_scaling * dopp_shift;
    }
    return sf;
}

pmt::pmt_t
moving_target_sim_impl::simulate_targets(std::vector<gr_complex> data) {
    std::vector<gr_complex> sf = get_tgt_scale_factors();
    std::vector<int> delays = get_tgt_delay_samps();
    int max_delay = *std::max_element(delays.begin(), delays.end());
    pmt::pmt_t out;

    // If we're operating in simulation mode, zeros are already included and we
    // don't need to make the vector bigger. If we're in loopback, we need to
    // expand the vector by the amount of delay.
    if (d_mode == LFM_GENERATOR_MODE__SIMULATION)
        out = pmt::make_c32vector(data.size(), (gr_complex) 0);
    else if (d_mode == LFM_GENERATOR_MODE__LOOPBACK)
        out = pmt::make_c32vector(data.size() + max_delay,
                                  (gr_complex) 0);

    // Simulate over all targets
    for (uint32_t ii = 0; ii < d_num_tgts; ii++) {
        // Operating in simulation mode
        if (d_mode == LFM_GENERATOR_MODE__SIMULATION) {
            for (uint32_t jj = delays[ii];
                 jj < data.size(); ++jj) {
                pmt::c32vector_set(out, jj, pmt::c32vector_ref(out, jj) +
                                            data[jj - delays[ii]] * sf[ii]);

            }
        } else if (d_mode == LFM_GENERATOR_MODE__LOOPBACK) {
            for (uint32_t jj = delays[ii]; jj < data.size()+delays[ii]; ++jj) {
                pmt::c32vector_set(out, jj, pmt::c32vector_ref(out, jj) +
                                            data[jj - delays[ii]] * sf[ii]);

            }
        }
    }

    return out;
} /* function simulate_targets() */

// TODO: These don't work in GRC (...yet)
/*
 * Thread-safe setters!
 */
void moving_target_sim_impl::set_tgt_rng(std::vector<float> tgt_rng) {
    gr::thread::scoped_lock l(d_setlock);

    d_tgt_rng = tgt_rng;
}

void moving_target_sim_impl::set_tgt_vel(std::vector<float> tgt_vel) {
    gr::thread::scoped_lock l(d_setlock);

    d_tgt_vel = tgt_vel;
}

void moving_target_sim_impl::set_tgt_rcs(std::vector<float> tgt_rcs) {
    gr::thread::scoped_lock l(d_setlock);

    d_tgt_rcs = tgt_rcs;
}

void moving_target_sim_impl::set_tx_freq(float tx_freq) {
    gr::thread::scoped_lock l(d_setlock);

    d_tx_freq = tx_freq;
}

void moving_target_sim_impl::set_prf(float prf) {
    gr::thread::scoped_lock l(d_setlock);

    d_prf = prf;
}
} /* namespace dragon */
} /* namespace gr */

