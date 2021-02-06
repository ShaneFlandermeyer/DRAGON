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
#include "pad_vector_impl.h"

namespace gr {
namespace dragon {

pad_vector::sptr
pad_vector::make(uint32_t old_len, uint32_t new_len) {
  return gnuradio::get_initial_sptr
      (new pad_vector_impl(old_len, new_len));
}

/*
 * The private constructor
 */
pad_vector_impl::pad_vector_impl(uint32_t old_len, uint32_t new_len)
    : gr::sync_block("pad_vector",
                     gr::io_signature::make(1, 1,
                                            sizeof(gr_complex) * old_len),
                     gr::io_signature::make(1, 1,
                                            sizeof(gr_complex) * new_len)),
      old_len(old_len),
      new_len(new_len) {}

/*
 * Our virtual destructor.
 */
pad_vector_impl::~pad_vector_impl() {
}

int
pad_vector_impl::work(int noutput_items,
                      gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items) {
  const auto *in = (const gr_complex *) input_items[0];
  auto *out = (gr_complex *) output_items[0];

  // Pre-allocate our output with zeros
  memset(out, 0, sizeof(gr_complex) * noutput_items * new_len);

  // Copy over the entire input and leave the rest of the output vector alone.
  for (int ii = 0; ii < noutput_items; ii++) {
    memcpy(out, in, sizeof(gr_complex) * old_len);
    out += new_len;
    in += old_len;
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

} /* namespace dragon */
} /* namespace gr */

