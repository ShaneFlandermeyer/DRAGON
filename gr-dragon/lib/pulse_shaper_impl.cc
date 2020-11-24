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
#include "pulse_shaper_impl.h"
#include <gnuradio/math.h>
namespace gr {
  namespace dragon {

    pulse_shaper::sptr
    pulse_shaper::make(dragon::cpm::cpm_type type, int vlen, int filt_len, int oversampling, float h, double beta)
    {
      return gnuradio::get_initial_sptr
        (new pulse_shaper_impl(type, vlen, filt_len, oversampling, h, beta));
    }


    /*
     * The private constructor
     */
    pulse_shaper_impl::pulse_shaper_impl(dragon::cpm::cpm_type type, int vlen, int filt_len, int oversampling, float h, double beta)
      : gr::sync_block("pulse_shaper",
              gr::io_signature::make(1, 1, sizeof(float)*vlen),
              gr::io_signature::make(1, 1, sizeof(float)*vlen)),
          d_vlen(vlen),
          d_filt_len(filt_len),
          d_oversampling(oversampling),
          d_h(h),
          d_beta(beta),
          d_taps(dragon::cpm::phase_response(type, oversampling, filt_len, beta))

    {
      d_fir = new gr::filter::kernel::fir_filter_fff(1,d_taps);
      set_history(oversampling);
    }

    /*
     * Our virtual destructor.
     */
    pulse_shaper_impl::~pulse_shaper_impl()
    {
      delete d_fir;
    }

    int
    pulse_shaper_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
      for (int i = 0; i < noutput_items; i++) {
        d_fir->filterN(out+i*d_vlen,in+i*d_vlen,d_vlen);
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dragon */
} /* namespace gr */

