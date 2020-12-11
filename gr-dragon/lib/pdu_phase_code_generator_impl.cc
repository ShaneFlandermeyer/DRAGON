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
#include "pdu_phase_code_generator_impl.h"

namespace gr {
namespace dragon {

pdu_phase_code_generator::sptr
pdu_phase_code_generator::make(dragon::phase_code::code_type type, uint32_t code_len,
                               bool repeat) {
    return gnuradio::get_initial_sptr
            (new pdu_phase_code_generator_impl(type, code_len, repeat));
}


/*
 * The private constructor
 */
pdu_phase_code_generator_impl::pdu_phase_code_generator_impl(
        dragon::phase_code::code_type type, uint32_t code_len, bool repeat)
        : gr::block("pdu_phase_code_generator",
                    gr::io_signature::make(0, 0, 0),
                    gr::io_signature::make(0, 0, 0)),
          d_code_len(code_len),
          d_type(type),
          d_repeat(repeat),
          d_out_port(pmt::mp("out")),
          d_ctrl_port(pmt::mp("ctrl")),
          d_phase_code(pmt::make_f32vector(code_len, 0)) {
    generate_code();
    message_port_register_in(d_ctrl_port);
    message_port_register_out(d_out_port);
    set_msg_handler(d_ctrl_port,
                    [this](pmt::pmt_t msg) { this->handle_ctrl_msg(msg); });

}

/*
 * Our virtual destructor.
 */
pdu_phase_code_generator_impl::~pdu_phase_code_generator_impl() {
}

void pdu_phase_code_generator_impl::run() {
    while (!d_finished) {
        // TODO: Allow the user to set the sleep duration
        boost::this_thread::sleep(
                boost::posix_time::milliseconds(static_cast<long>(5)));
        if (d_finished) {
            return;
        }
        this->message_port_pub(d_out_port, pmt::cons(pmt::make_dict(), d_phase_code));
//        d_finished = true;
    }


}

bool pdu_phase_code_generator_impl::start() {
    d_finished = false;
    d_thread = boost::shared_ptr<gr::thread::thread>(
            new gr::thread::thread(boost::bind
                                           (&pdu_phase_code_generator_impl::run,
                                            this)));
    return block::start();
}

bool pdu_phase_code_generator_impl::stop() {
    d_finished = true;
    d_thread->interrupt();
    d_thread->join();

    return block::stop();
}

void pdu_phase_code_generator_impl::handle_ctrl_msg(pmt::pmt_t msg) {
    // If the message isn't a dictionary, do nothing
    if (!pmt::is_dict(msg))
        return;
    // Get the dictionary keys and values from the message
    try {
        pmt::pmt_t keys = pmt::dict_keys(msg);
        msg = pmt::car(msg);
    } catch (const pmt::wrong_type &e) {
        // If the message is a pair, make it a dict
        GR_LOG_WARN(this->d_logger, "got a pair, not a dict; fixing")
        msg = pmt::dict_add(pmt::make_dict(), pmt::car(msg), pmt::cdr(msg));
    }

    if (pmt::dict_has_key(msg, pmt::mp("code_len"))) {
        uint32_t new_code_len = pmt::to_uint64(pmt::dict_ref(msg, pmt::mp("code_len"),
                                                             pmt::from_uint64(
                                                                     d_code_len)));
        if (new_code_len > 0) {
            set_code_len(new_code_len);
            d_phase_code = pmt::make_f32vector(d_code_len, 0);
        }
    }
    generate_code();
}

void pdu_phase_code_generator_impl::generate_code() {
    std::vector<float> code = dragon::phase_code::get_code(d_type, d_code_len);
    for (int ii = 0; ii < code.size(); ii++) {
        pmt::f32vector_set(d_phase_code, ii, code[ii]);
    }
}

void pdu_phase_code_generator_impl::set_code_len(uint32_t code_len) {
    gr::thread::scoped_lock l(this->d_setlock);
    d_code_len = code_len;
}

} /* namespace dragon */
} /* namespace gr */

