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
#include "pcfm_modulator_impl.h"

namespace gr {
  namespace dragon {

    pcfm_modulator::sptr
    pcfm_modulator::make(int code_len, int oversampling, int samp_rate)
    {
      return gnuradio::get_initial_sptr
        (new pcfm_modulator_impl(code_len, oversampling, samp_rate));
    }


    /*
     * The private constructor
     */
    pcfm_modulator_impl::pcfm_modulator_impl(int code_len, int oversampling, int samp_rate)
      : gr::hier_block2("pcfm_modulator",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
      connect(self(), 0, d_firstblock, 0);
      // connect other blocks
      connect(d_lastblock, 0, self(), 0);
    }

    /*
     * Our virtual destructor.
     */
    pcfm_modulator_impl::~pcfm_modulator_impl()
    {
    }


  } /* namespace dragon */
} /* namespace gr */

