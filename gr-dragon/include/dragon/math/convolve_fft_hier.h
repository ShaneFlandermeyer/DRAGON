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

#ifndef INCLUDED_DRAGON_CONVOLVE_FFT_HIER_H
#define INCLUDED_DRAGON_CONVOLVE_FFT_HIER_H

#include <dragon/api.h>
#include <gnuradio/hier_block2.h>

// Include all the blocks we need
#include <gnuradio/fft/fft_vcc.h>
#include <dragon/vector-utils/pad_vector.h>
#include <gnuradio/blocks/multiply.h>
namespace gr {
namespace dragon {

/*!
 * \brief <+description of block+>
 * \ingroup dragon
 *
 */
class DRAGON_API convolve_fft_hier : virtual public gr::hier_block2 {
 public:
  typedef boost::shared_ptr<convolve_fft_hier> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of dragon::convolve_fft_hier.
   *
   * To avoid accidental use of raw pointers, dragon::convolve_fft_hier's
   * constructor is in a private implementation
   * class. dragon::convolve_fft_hier::make is the public interface for
   * creating new instances.
   */
  static sptr make(uint32_t fft_size,
                   const std::vector<float> &window,
                   int nthreads);
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_CONVOLVE_FFT_HIER_H */

