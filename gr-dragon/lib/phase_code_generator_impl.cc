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

namespace gr
{
    namespace dragon
    {

        phase_code_generator::sptr
        phase_code_generator::make(const std::string code_type, int code_len, bool repeat,
                                   int repetitions, const std::vector<tag_t> tags)
        {
            return gnuradio::get_initial_sptr
                    (new phase_code_generator_impl(code_type, code_len, repeat,
                                                   repetitions, tags));
        }


        /*
         * The private constructor
         */
        phase_code_generator_impl::phase_code_generator_impl(const std::string code_type,
                                                             int code_len, bool repeat,
                                                             int repetitions,
                                                             const std::vector<tag_t> tags)
                : gr::sync_block("phase_code_generator",
                                 gr::io_signature::make(0, 0, 0),
                                 gr::io_signature::make(1, 1, sizeof(float) * code_len)),
                  d_code_type(code_type),
                  d_code_len(code_len),
                  d_repeat(repeat),
                  d_repetitions(repetitions), // TODO: This parameter does nothing rn
                  d_tags(tags),
                  d_code_vec(
                          phase_code_generator_impl::get_code_vector(code_type,
                                                                     code_len)),
                  d_offset(0)
        {
            if (tags.empty()) {
                d_set_tags = false;
            } else {
                d_set_tags = true;
            }
        }

        /*
         * Our virtual destructor.
         */
        phase_code_generator_impl::~phase_code_generator_impl()
        = default;

        int
        phase_code_generator_impl::work(int noutput_items,
                                        gr_vector_const_void_star &input_items,
                                        gr_vector_void_star &output_items)
        {
            auto *out = (float *) output_items[0];
            if (d_repeat) { // Transmit the code repeatedly
                unsigned int size = d_code_len;
                unsigned int offset = d_offset;
                if (size == 0)
                    return -1;
                if (d_set_tags) {
                    for (int i = 0; i < noutput_items; ++i) {
                        memcpy(out, (const void *) &d_code_vec[0], size * sizeof(float));
                        out += size;
                        // Add tags to the output stream
                        for (auto &d_tag : d_tags) {
                            this->add_item_tag(0,
                                               this->nitems_written(0) + i + d_tag
                                                       .offset,
                                               d_tag.value,
                                               d_tag.srcid);
                        } // for
                    } // for
                } else { // No tags specified
                    for (int i = 0;
                         i < static_cast<int> (noutput_items * d_code_len); ++i) {
                        out[i] = d_code_vec[offset++];
                        if (offset >= size) {
                            offset = 0;
                        } // if
                    } // for
                } // else
                d_offset = offset;
                return noutput_items;
            } else { // Don't repeat once the code vector has been fully read
                if (d_offset >= d_code_len)
                    return -1;
                // Number of items to grab from the code vector. If this exceeds the
                // amount of space we have in the output, grab as much as possible
                unsigned n = std::min((unsigned) d_code_len - d_offset,
                                      (unsigned) noutput_items * d_code_len);
                // Move the code vector to the output
                for (unsigned i = 0; i < n; ++i) {
                    out[i] = d_code_vec[d_offset + i];
                } // for
                // Add tags to the output
                for (unsigned t = 0; t < d_code_len; t++) {
                    if ((d_tags[t].offset >= d_offset) && (d_tags[t].offset < d_offset
                                                                              + n))
                        this->add_item_tag(0, d_tags[t].offset, d_tags[t].key,
                                           d_tags[t].value, d_tags[t].srcid);
                } // for
                d_offset += n; // Increment the index
                // Return the number of vectors formed. Will be either 1 or 0
                return n / d_code_len;
            } // else
        } /* Function work */

        std::vector<float>
        phase_code_generator_impl::get_code_vector(const std::string code_type,
                                                   int code_len) const
        {
            std::vector<float> phi(code_len);
            if (code_type == "P4") {
                for (int m = 0; m < code_len; ++m) {
                    phi[m] = (2 * PI / code_len) * m * ((m - code_len) / 2.0);
                } // for

            } // if
            return phi;
        }

    } /* namespace dragon */
} /* namespace gr */

