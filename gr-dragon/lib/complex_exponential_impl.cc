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
#include "complex_exponential_impl.h"

namespace gr {
  namespace dragon {

    complex_exponential::sptr
    complex_exponential::make()
    {
      return gnuradio::get_initial_sptr
        (new complex_exponential_impl());
    }


    /*
     * The private constructor
     */
    complex_exponential_impl::complex_exponential_impl()
      : gr::sync_block("complex_exponential",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {}

    /*
     * Our virtual destructor.
     */
    complex_exponential_impl::~complex_exponential_impl()
    {
    }

    int
    complex_exponential_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const auto *in = (const float *) input_items[0];
      auto *out = (gr_complex *) output_items[0];
      std::complex<float> j(0,1);
      for (int i = 0;i < noutput_items; i++)
          out[i] = std::exp(j*in[i]);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dragon */
} /* namespace gr */

