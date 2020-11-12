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

#include "shaping_filt_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
    namespace dragon {

        shaping_filt::sptr
        shaping_filt::make(std::string filt_type, int filt_len, int vlen) {
            return gnuradio::get_initial_sptr(
                    new shaping_filt_impl(filt_type, filt_len, vlen));
        }


/*
 * The private constructor
 */
        shaping_filt_impl::shaping_filt_impl(std::string filt_type,
                                             int filt_len, int vlen)
                : gr::sync_block("shaping_filt",
                                 gr::io_signature::make(1, 1,
                                                        sizeof(float) * vlen),
                                 gr::io_signature::make(1, 1,
                                                        sizeof(float) * vlen)),
                  d_filt_type(filt_type),
                  d_filt_len(filt_len),
                  d_vlen(vlen) {
        }

/*
 * Our virtual destructor.
 */
        shaping_filt_impl::~shaping_filt_impl() {}

        int shaping_filt_impl::work(int noutput_items,
                                    gr_vector_const_void_star &input_items,
                                    gr_vector_void_star &output_items) {
            const auto *in = (const float *) input_items[0];
            auto *out = (float *) output_items[0];

            double acc;
            for (int i = 0; i < noutput_items; i++) {
                acc = 0;
                for (int j = 0; j < d_vlen; ++j) {
                    // Add the newest input to the running sum
                    acc += in[i * d_vlen + j];
                    // If our index exceeds the filter's length, remove the value immediately to
                    // the left of the filter
                    if (j >= d_filt_len)
                        acc -= in[i * d_vlen + j - d_filt_len];
                    // Compute the average for this sample
                    out[i * d_vlen + j] = acc / (double) d_filt_len;
                } // for
            } // for

            // Tell runtime system how many output items we produced.
            return noutput_items;
        } /* function work */

    } /* namespace dragon */
} /* namespace gr */
