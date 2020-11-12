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

#ifndef INCLUDED_DRAGON_PHASE_CODE_GENERATOR_IMPL_H
#define INCLUDED_DRAGON_PHASE_CODE_GENERATOR_IMPL_H

#include <dragon/phase_code_generator.h>

namespace gr
{
    namespace dragon
    {

        class phase_code_generator_impl : public phase_code_generator
        {
        private:
            // Constants
            const long double PI = 3.141592653589793238L;
            // Member Variables
            unsigned int d_code_len; // Length of the code
            unsigned int d_offset; //
            bool d_repeat;
            bool d_set_tags;
            int d_repetitions;
            std::string d_code_type;
            std::vector<tag_t> d_tags;
            std::vector<float> d_code_vec;
        public:
            phase_code_generator_impl(const std::string code_type, int code_len, bool repeat,
                                      int repetitions, const std::vector<tag_t> tags);

            ~phase_code_generator_impl() override;

            void rewind()
            { d_offset = 0; }

            std::vector<float> get_code_vector(const std::string code_type, int code_len) const;


            // Where all the action really happens
            int work(
                    int noutput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items
            );
        };

    } // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PHASE_CODE_GENERATOR_IMPL_H */

