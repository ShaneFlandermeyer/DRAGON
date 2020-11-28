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
#include "phase_code_generator_impl.h"

namespace gr {
namespace dragon {

phase_code_generator::sptr
phase_code_generator::make(dragon::phase_code::code_type type, unsigned code_len,
                           bool repeat, const std::vector<tag_t> tags) {
    return gnuradio::get_initial_sptr
            (new phase_code_generator_impl(type, code_len, repeat, tags));
}


/*
 * The private constructor
 */
phase_code_generator_impl::phase_code_generator_impl(dragon::phase_code::code_type type,
                                                     unsigned code_len, bool repeat,
                                                     const std::vector<tag_t> tags)
        : gr::sync_block("phase_code_generator",
                         gr::io_signature::make(0, 0, 0),
                         gr::io_signature::make(1, 1, sizeof(float) * code_len)),
          d_type(type),
          d_code_len(code_len),
          d_repeat(repeat),
          d_tags(tags),
          d_phase_code(dragon::phase_code::get_code(type, code_len)),
          d_offset(0) {
    if (tags.empty()) {
        d_set_tags = false;
    } else {
        d_set_tags = true;
    }
}

/*
 * Our virtual destructor.
 */
phase_code_generator_impl::~phase_code_generator_impl() = default;

int
phase_code_generator_impl::work(int noutput_items,
                                gr_vector_const_void_star &input_items,
                                gr_vector_void_star &output_items) {
    auto *out = (float *) output_items[0];
    if (d_repeat) { // Transmit the code repeatedly
        unsigned size = d_code_len;
        unsigned offset = d_offset;
        if (size == 0) // Code is length 0. Do nothing
            return -1;
        if (d_set_tags) { // Add tags to stream
            for (int i = 0; i < noutput_items; i++) {
                memcpy(out, (const void *) &d_phase_code[0], size * sizeof(float));
                out += size;
                for (auto &tag : d_tags) {
                    this->add_item_tag(0,
                                       this->nitems_written(0) + i +
                                       tag.offset, tag.value, tag.srcid);
                } // for
            } // for
        } else { // No tags specified
            for (int i = 0; i < static_cast<int> (noutput_items * d_code_len); i++) {
                out[i] = d_phase_code[offset++];
                if (offset >= size) {
                    offset = 0;
                } // if
            } // for
        } // else
        d_offset = offset;
        return noutput_items;
    } else { // Don't repeat
        // A full code has been generated; don't do anything else
        if (d_offset >= d_code_len)
            return -1;
        // Number of items to grab from the code vector. If this exceeds the amount of
        // space we have in the output buffer, grab as much as possible
        unsigned n = std::min(d_code_len - d_offset,
                              (unsigned) noutput_items * d_code_len);
        // Move n samples of the code vector to the output
        memcpy(out, &d_phase_code[d_offset],n*sizeof(float));
        // Add tags to the output
        for (auto & d_tag : d_tags) {
            if ((d_tag.offset >= d_offset) && (d_tag.offset < d_offset
                                                                      + n))
                this->add_item_tag(0, d_tag.offset, d_tag.key,
                                   d_tag.value, d_tag.srcid);
        } // for
        d_offset += n; // Increment the index
        // Return the number of vectors formed. Will be either 1 or 0
        return n / d_code_len;

    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace dragon */
} /* namespace gr */

