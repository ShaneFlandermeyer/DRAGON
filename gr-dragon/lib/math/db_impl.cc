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
#include "dragon/constants.h"
#include "db_impl.h"

namespace gr {
namespace dragon {

db::sptr
db::make(uint32_t mode, uint32_t vlen) {
  return gnuradio::get_initial_sptr
      (new db_impl(mode, vlen));
}

/*
 * The private constructor
 */
db_impl::db_impl(uint32_t mode, uint32_t vlen)
    : gr::sync_block("db",
                     gr::io_signature::make(1, 1, sizeof(float)*vlen),
                     gr::io_signature::make(1, 1, sizeof(float)*vlen)),
      mode(mode),
      vlen(vlen) {
}

/*
 * Our virtual destructor.
 */
db_impl::~db_impl() {
}

int
db_impl::work(int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items) {
  auto *in = (const float *) input_items[0];
  auto *out = (float *) output_items[0];
  uint32_t noi = noutput_items * vlen;

  for (uint32_t ii = 0; ii < noi; ii++) {
    if (mode == VOLTAGE)
      out[ii] = 20 * std::log10(in[ii]);
    else if (mode == POWER)
      out[ii] = 10 * std::log10(in[ii]);
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

} /* namespace dragon */
} /* namespace gr */

