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

#ifndef INCLUDED_DRAGON_PULSE_SHAPER_IMPL_H
#define INCLUDED_DRAGON_PULSE_SHAPER_IMPL_H

#include <dragon/waveform/pulse_shaper.h>
#include <gnuradio/filter/fir_filter.h>

namespace gr {
namespace dragon {

class pulse_shaper_impl : public pulse_shaper {
private:
  int d_vlen;
  int d_filt_len;
  int d_oversampling;
  double d_beta;
  gr::filter::kernel::fir_filter_fff *d_fir;

protected:
  std::vector<float> d_taps;

public:
  pulse_shaper_impl(dragon::cpm::cpm_type type, int vlen, int filt_len,
                    int oversampling, double beta);
  ~pulse_shaper_impl();

  int work(int noutput_items, gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PULSE_SHAPER_IMPL_H */
