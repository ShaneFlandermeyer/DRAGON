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

#ifndef INCLUDED_DRAGON_LFM_PDU_IMPL_H
#define INCLUDED_DRAGON_LFM_PDU_IMPL_H

#include <dragon/lfm_pdu.h>
#include <dragon/pdu_constants.h>

namespace gr {
namespace dragon {

class lfm_pdu_impl : public lfm_pdu {
private:
    /*
     * Constants
     */
    const gr_complex j = gr_complex(0, 1);
    /*
     * Waveform parameters
     */
    float d_bandwidth; // Bandwidth
    float d_samp_rate; // Sample rate
    float d_sweep_time; // Pulse width
    float d_prf; // PRF
    float d_pri; // PRI
    // d_finished tells the scheduler that we have killed the thread (set to false in
    // stop()
    bool d_finished;
    uint32_t d_mode;
    // PMT string for the output and control message ports
    pmt::pmt_t d_waveform;
    pmt::pmt_t d_out_port;
    pmt::pmt_t d_ctrl_port;
    // The block's thread
    boost::shared_ptr <gr::thread::thread> d_thread;

    void handle_ctrl_msg(pmt::pmt_t msg);

    void run();

    void generate_lfm();


public:
    lfm_pdu_impl(float bandwidth, float sweep_time, float samp_rate, float prf, uint32_t mode);

    ~lfm_pdu_impl() override;

    // Overload these methods from gr::block so we can start and stop the internal
    // thread that periodically transmits a waveform
    bool start() override;

    bool stop() override;

    template<typename T>
    std::vector <T> arange(T start, T stop, T step = 1);

    void set_bandwidth(float bandwidth);
    void set_samp_rate(float samp_rate);
    void set_sweep_time(float sweep_time);
    void set_prf(float prf);
    void set_pri(float pri);
};


} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_LFM_PDU_IMPL_H */

