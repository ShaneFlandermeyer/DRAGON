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
#include "convolve_fft_hier_impl.h"

namespace gr {
namespace dragon {

convolve_fft_hier::sptr
convolve_fft_hier::make(uint32_t fft_size,
                        const std::vector<float> &window,
                        int nthreads) {
  return gnuradio::get_initial_sptr
      (new convolve_fft_hier_impl(fft_size, window, nthreads));
}

/*
 * The private constructor
 */
convolve_fft_hier_impl::convolve_fft_hier_impl(uint32_t fft_size,
                                               const std::vector<float> &window,
                                               int nthreads)
    : gr::hier_block2("convolve_fft_hier",
                      gr::io_signature::make(2, 2,
                                             sizeof(gr_complex) * fft_size),
                      gr::io_signature::make(1, 1,
                                             sizeof(gr_complex) * fft_size)),
      fft_fwd_sig(fft::fft_vcc::make(fft_size, true, window, false, 1)),
      fft_fwd_filt(fft::fft_vcc::make(fft_size, true, window, false, 1)),
      multiply(blocks::multiply_cc::make(fft_size)),
      fft_rev(fft::fft_vcc::make(fft_size, false, window, false, 1))
//  gr::fft::fft_vcc::sptr fwd_fft;
//  gr::fft::fft_vcc::sptr fft_rev;
//  gr::blocks::multiply<gr_complex>::sptr multiply;
//  dragon::pad_vector::sptr pad_filt_vec;
//  dragon::pad_vector::sptr pad_sig_vec;
{
  connect(self(), 0, fft_fwd_sig, 0);
  connect(self(), 1, fft_fwd_filt, 0);
  connect(fft_fwd_sig, 0, multiply, 0);
  connect(fft_fwd_filt, 0, multiply, 1);
  connect(multiply, 0, fft_rev, 0);
  connect(fft_rev, 0, self(), 0);
}
/*
 * Our virtual destructor.
 */
convolve_fft_hier_impl::~convolve_fft_hier_impl() {
}

} /* namespace dragon */
} /* namespace gr */

