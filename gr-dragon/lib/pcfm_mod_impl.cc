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

#include "pcfm_mod_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace dragon {

pcfm_mod::sptr pcfm_mod::make(dragon::cpm::cpm_type type, int code_len,
                              int oversampling, int samp_rate) {
  return gnuradio::get_initial_sptr(
      new pcfm_mod_impl(type, code_len, oversampling, samp_rate));
}

/*
 * The private constructor
 */
pcfm_mod_impl::pcfm_mod_impl(dragon::cpm::cpm_type type, int code_len,
                             int oversampling, int samp_rate)
    : gr::hier_block2("pcfm_mod",
                      gr::io_signature::make(1, 1, sizeof(float) * code_len),
                      gr::io_signature::make(1, 1, sizeof(float))),
      d_type(type), d_code_len(code_len), d_oversampling(oversampling),
      d_samp_rate(samp_rate),
      d_difference(dragon::difference<float>::make(code_len)),
      d_oversample_vector(
          dragon::oversample_vector::make(code_len, oversampling)),
      d_pulse_shaper(dragon::pulse_shaper::make(type, code_len * oversampling,
                                                oversampling, 1, 0.3)),
      d_dti(dragon::dti::make(1, code_len * oversampling)),
      d_vec_to_stream(blocks::vector_to_stream::make(sizeof(float) * 1,
                                                     code_len * oversampling))

{
  connect(self(), 0, d_difference, 0);
  connect(d_difference, 0, d_oversample_vector, 0);
  connect(d_oversample_vector, 0, d_pulse_shaper, 0);
  connect(d_pulse_shaper, 0, d_dti, 0);
  connect(d_dti, 0, d_vec_to_stream, 0);
  connect(d_vec_to_stream, 0, self(), 0);
}

/*
 * Our virtual destructor.
 */
pcfm_mod_impl::~pcfm_mod_impl() {}

} /* namespace dragon */
} /* namespace gr */
