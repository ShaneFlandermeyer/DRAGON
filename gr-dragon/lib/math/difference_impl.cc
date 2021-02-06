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
#include "difference_impl.h"

namespace gr {
namespace dragon {
template<class T>
typename difference<T>::sptr
difference<T>::make(unsigned int vlen) {
    return gnuradio::get_initial_sptr
            (new difference_impl<T>(vlen));
}


/*
 * The private constructor
 */
template<class T>
difference_impl<T>::difference_impl(unsigned int vlen)
        : gr::sync_block("difference",
                         gr::io_signature::make(1, 1, sizeof(T) * vlen),
                         gr::io_signature::make(1, 1,
                                                sizeof(T) * vlen)),
          d_vlen(vlen) {
//            set_history(2);
}

/*
 * Our virtual destructor.
 */
template<class T>
difference_impl<T>::~difference_impl() {
}

template<class T>
int difference_impl<T>::work(int noutput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items) {
    const T *in = reinterpret_cast<const T *>(input_items[0]);
    T *out = reinterpret_cast<T *>(output_items[0]);
    // Since the difference operation will produce vlen-1 items, set the first
    // output item to the first input value
    for (int i = 0; i < noutput_items; i++) {
        out[i * d_vlen] = in[i * d_vlen];
        for (int j = 1; j < d_vlen; ++j) {
            out[i * d_vlen + j] = (in[i * d_vlen + j] - in[i * d_vlen + j - 1]);
        }
    }
//    out[0] = in[0];
//    for (int i = 1; i < noutput_items * vlen; ++i) {
//        out[i] = (in[i] - in[i - 1]);
//    }
    return noutput_items;
}

template
class difference<gr_complex>;

template
class difference<float>;

} /* namespace dragon */
} /* namespace gr */
