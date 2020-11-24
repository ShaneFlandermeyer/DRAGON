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

#ifndef INCLUDED_DRAGON_PCFM_MOD_IMPL_H
#define INCLUDED_DRAGON_PCFM_MOD_IMPL_H

#include <dragon/pcfm_mod.h>
//#include <gnuradio/blocks/stream_to_vector.h>
#include <dragon/difference.h>
#include <dragon/oversample_vector.h>
#include <dragon/pulse_shaper.h>
#include <dragon/dti.h>
#include <gnuradio/blocks/vector_to_stream.h>
//
//#include <gnuradio/analog/frequency_modulator_fc.h>
//#include <gnuradio/blocks/char_to_float.h>
//#include <gnuradio/digital/cpmmod_bc.h>
//#include <gnuradio/filter/interp_fir_filter.h>
namespace gr {
namespace dragon {

class pcfm_mod_impl : public pcfm_mod {
private:
  int d_type;
  int d_code_len;
  int d_oversampling;
  int d_samp_rate;

protected:
  dragon::difference<float>::sptr d_difference;
  dragon::oversample_vector::sptr d_oversample_vector;
  dragon::pulse_shaper::sptr d_pulse_shaper;
  dragon::dti::sptr d_dti;
  gr::blocks::vector_to_stream::sptr d_vec_to_stream;

public:
  pcfm_mod_impl(dragon::cpm::cpm_type type, int code_len, int oversampling,
                int samp_rate);
  ~pcfm_mod_impl();
//  int type() const;
//  int code_len() const;
//  int oversampling() const;
//  int samp_rate() const;

  // Where all the action really happens
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PCFM_MOD_IMPL_H */
