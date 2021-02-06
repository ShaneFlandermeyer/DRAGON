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
#include "dti_impl.h"

namespace gr {
namespace dragon {

dti::sptr
dti::make(int decim, unsigned int vlen) {
  return gnuradio::get_initial_sptr
      (new dti_impl(decim, vlen));
}

/*
 * The private constructor
 */
dti_impl::dti_impl(int decim, unsigned int vlen)
    : gr::sync_decimator("dti",
                         gr::io_signature::make(1, 1, sizeof(float) * vlen),
                         gr::io_signature::make(1, 1, sizeof(float) * vlen),
                         decim),
      d_decim(decim),
      d_vlen(vlen) {}

/*
 * Our virtual destructor.
 */
dti_impl::~dti_impl() {
}

int
dti_impl::work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items) {
  // TODO: Make this a template class and make this function prettier
  auto *in = (const float *) input_items[0];
  auto *out = (float *) output_items[0];

  for (int i = 0; i < noutput_items; ++i) {
    for (unsigned int j = 0; j < d_vlen; ++j) {
      out[i * d_vlen + j] = (float) 0;
    }
    for (int j = 0; j < d_decim; ++j) {
      out[i * d_vlen] = in[i * d_decim * d_vlen + j * d_vlen];
      for (unsigned int k = 1; k < d_vlen; ++k) {
        out[i * d_vlen + k] = out[i * d_vlen + (k - 1)] +
            in[i * d_decim * d_vlen + j * d_vlen + k];
      }
    }
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

} /* namespace dragon */
} /* namespace gr */

