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

#ifndef INCLUDED_DRAGON_MOVING_TARGET_SIM_IMPL_H
#define INCLUDED_DRAGON_MOVING_TARGET_SIM_IMPL_H

#include <dragon/moving_target_sim.h>
#include <vector>
#include <gnuradio/math.h>
#include <dragon/pdu_constants.h>

namespace gr {
namespace dragon {

class moving_target_sim_impl : public moving_target_sim {
private:
    // Constants
    const double C = 2.99792458e8; // Speed of light in a vacuum (m/s)
    const gr_complex j = gr_complex(0, 1); // Imaginary number i
    // Class members
    std::vector<float> d_tgt_rng;
    std::vector<float> d_tgt_vel;
    std::vector<float> d_tgt_rcs;
    float d_tx_freq;
    float d_prf;
    float d_samp_rate;
    float d_wavelength;
    uint64_t d_burst_counter;
    uint32_t d_num_tgts;
    uint32_t d_mode;

    // Private functions
    pmt::pmt_t simulate_targets(std::vector <gr_complex> data);

    std::vector<int> get_tgt_delay_samps();

    std::vector <gr_complex> get_tgt_scale_factors();

    std::vector <gr_complex> delay_vector(std::vector <gr_complex> vec, uint64_t delay);

public:
    void handle_pdu(pmt::pmt_t pdu);

    moving_target_sim_impl(std::vector<float> tgt_rng, std::vector<float> tgt_vel,
                           std::vector<float> tgt_rcs, float tx_freq, float prf, float
                           samp_rate, uint32_t mode);

    ~moving_target_sim_impl();

    void set_tgt_rng(std::vector<float> tgt_rng);

    void set_tgt_vel(std::vector<float> tgt_vel);

    void set_tgt_rcs(std::vector<float> tgt_rcs);

    void set_tx_freq(float tx_freq);

    void set_prf(float prf);


//    // Where all the action really happens
//    int work(
//            int noutput_items,
//            gr_vector_const_void_star &input_items,
//            gr_vector_void_star &output_items
//    );
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_MOVING_TARGET_SIM_IMPL_H */

