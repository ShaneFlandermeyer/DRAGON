/* -*- c++ -*- */
/*
 * Copyright 2021 gr-dragon author.
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
#include "convolve_impl.h"
//#include <volk/volk.h>
#include <dragon/constants.h>
#include <dragon/vector-utils/vector_functions.h>

namespace gr {
namespace dragon {

convolve::sptr
convolve::make(uint32_t in_len, uint32_t filt_len) {
  return gnuradio::get_initial_sptr
      (new convolve_impl(in_len, filt_len));
}

/*
 * The private constructor
 */
convolve_impl::convolve_impl(uint32_t in_len, uint32_t filt_len)
    : gr::sync_block("convolve",
                     gr::io_signature::make(1, 1, sizeof(gr_complex) * in_len),
                     gr::io_signature::make(1, 1, sizeof(gr_complex) *
                         (in_len + filt_len - 1))),
      in_len(in_len),
      filt_len(filt_len),
      out_len(in_len + filt_len - 1),
      filt_port(PMTCONSTSTR__FILTER),
      debug_port(PMTCONSTSTR__DEBUG),

      msg_sent(false) {
  // Set up our message ports
  message_port_register_in(filt_port);
  message_port_register_out(debug_port);
  set_msg_handler(filt_port,
                  [this](pmt::pmt_t msg) { this->handle_msg(msg); });
}

/*
 * Our virtual destructor.
 */
convolve_impl::~convolve_impl() {
  delete fir;
}

void convolve_impl::handle_msg(pmt::pmt_t msg) {

  if (!pmt::is_dict(msg))
    return;
  try {
    pmt::pmt_t keys = pmt::dict_keys(msg);
  } catch (const pmt::wrong_type &e) {
    GR_LOG_WARN(this->d_logger, "Got a pair, not a dict; fixing...");
    msg = pmt::dict_add(pmt::make_dict(), pmt::car(msg), pmt::cdr(msg));
  }

  /*
   * Matched filter message
   */
  if (pmt::dict_has_key(msg, PMTCONSTSTR__TAPS)) {
    pmt::pmt_t value = pmt::dict_ref(
        msg, PMTCONSTSTR__TAPS, init_data(match_filt));
    if (!pmt::is_uniform_vector(value)) {
      GR_LOG_WARN(this->d_logger, "Filter vector not found.");
      has_filt = false;
      return;
    }
    has_filt = true;
    match_filt = init_std_vector(value);
    fir = new gr::filter::kernel::fir_filter_ccc(1, match_filt);
    if (out_len == 1)
      set_history(match_filt.size());

  }

}
int
convolve_impl::work(int noutput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items) {
  auto *in = (const gr_complex *) input_items[0];
  auto *out = (gr_complex *) output_items[0];

  // If we don't have a filter reference yet, do nothing
  if (!has_filt)
    return 0;
  if (out_len == 1) {
    fir->filterN(out, in, noutput_items);
    return noutput_items;
  }
  // If we're dealing with vectors, apply the filter to each input vector
  // separately
  for (uint32_t ii = 0; ii < noutput_items; ii++) {
    fir->filterN(out + ii * out_len, in + ii * out_len, out_len);
  }
  return noutput_items;
}

} /* namespace dragon */
} /* namespace gr */

