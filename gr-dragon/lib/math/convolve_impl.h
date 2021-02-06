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

#ifndef INCLUDED_DRAGON_CONVOLVE_IMPL_H
#define INCLUDED_DRAGON_CONVOLVE_IMPL_H

#include <dragon/math/convolve.h>
#include <gnuradio/fft/fft.h>
#include <gnuradio/filter/fir_filter.h>

#define VERBOSE 0
namespace gr {
namespace dragon {

class convolve_impl : public convolve {

 private:

  pmt::pmt_t filt_port;
  pmt::pmt_t debug_port;
  uint32_t in_len;
  uint32_t filt_len;
  uint32_t out_len;

  bool msg_sent; // TODO: Remove this

  std::vector<gr_complex> match_filt;
  bool has_filt;

  gr::filter::kernel::fir_filter_ccc *fir;

  void handle_msg(pmt::pmt_t msg);

 public:
  explicit convolve_impl(uint32_t in_len, uint32_t filt_len);
  ~convolve_impl() override;

  // Where all the action really happens
  int work(
      int noutput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items
  );
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_CONVOLVE_IMPL_H */

