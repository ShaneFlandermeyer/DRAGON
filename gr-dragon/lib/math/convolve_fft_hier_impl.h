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

#ifndef INCLUDED_DRAGON_CONVOLVE_FFT_HIER_IMPL_H
#define INCLUDED_DRAGON_CONVOLVE_FFT_HIER_IMPL_H

#include <dragon/math/convolve_fft_hier.h>
#include <gnuradio/fft/fft_vcc.h>
#include <gnuradio/blocks/multiply.h>
#include <dragon/vector-utils/pad_vector.h>

namespace gr {
namespace dragon {

class convolve_fft_hier_impl : public convolve_fft_hier {
 private:

 protected:
  gr::fft::fft_vcc::sptr fft_fwd_sig;
  gr::fft::fft_vcc::sptr fft_fwd_filt;
  gr::fft::fft_vcc::sptr fft_rev;
  gr::blocks::multiply<gr_complex>::sptr multiply;
  dragon::pad_vector::sptr pad_filt_vec;
  dragon::pad_vector::sptr pad_sig_vec;

 public:
  convolve_fft_hier_impl(uint32_t fft_size,
                         const std::vector<float> &window,
                         int nthreads);
  ~convolve_fft_hier_impl();

  // Where all the action really happens
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_CONVOLVE_FFT_HIER_IMPL_H */

