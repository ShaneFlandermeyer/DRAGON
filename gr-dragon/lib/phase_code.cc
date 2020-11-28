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
#include <dragon/phase_code.h>

#include <cfloat>
#include <cmath>
#include <gnuradio/math.h>

namespace gr {
namespace dragon {
std::vector<float> generate_p4_code(int code_len) {
    std::vector<float> phi(code_len);
        for (int m = 0; m < code_len; ++m) {
            phi[m] = (2 * GR_M_PI / code_len) * m * ((m - code_len) / 2.0);
        } // for
    return phi;
}

std::vector<float> generate_barker_code(int code_len) {
    // TODO: Implement Me!
    return std::vector<float>(0);
}

std::vector<float> phase_code::get_code(code_type type, unsigned code_len) {
    switch (type) {
        case P4:
            return generate_p4_code(static_cast<int>(code_len));
        case BARKER:
            return generate_barker_code(static_cast<int>(code_len));
    }
} /* function get_code */
} /* namespace dragon */
} /* namespace gr */

