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

#ifndef INCLUDED_DRAGON_PDU_PHASE_CODE_GENERATOR_IMPL_H
#define INCLUDED_DRAGON_PDU_PHASE_CODE_GENERATOR_IMPL_H

#include <dragon/waveform/pdu_phase_code_generator.h>

namespace gr {
namespace dragon {

class pdu_phase_code_generator_impl : public pdu_phase_code_generator {
private:
    dragon::phase_code::code_type d_type;
    pmt::pmt_t d_phase_code;
    pmt::pmt_t d_out_port;
    pmt::pmt_t d_ctrl_port;
    uint32_t d_code_len;
    bool d_repeat;

    bool d_finished;
    boost::shared_ptr<gr::thread::thread> d_thread;

    void generate_code();
    void handle_ctrl_msg(pmt::pmt_t msg);
    void run();

public:
    pdu_phase_code_generator_impl(dragon::phase_code::code_type type, uint32_t code_len,
                                  bool repeat);

    ~pdu_phase_code_generator_impl() override;


    bool start() override;
    bool stop() override;
    void set_code_len(uint32_t code_len);
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PDU_PHASE_CODE_GENERATOR_IMPL_H */

