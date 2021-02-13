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

#ifndef INCLUDED_DRAGON_VECTOR_FUNCTIONS_H
#define INCLUDED_DRAGON_VECTOR_FUNCTIONS_H

#include <dragon/api.h>
#include <vector>
#include <pmt/pmt.h>
#include <gnuradio/gr_complex.h>

namespace gr {
namespace dragon {

/*!
 * \brief Converts a std::vector<gr_complex> to a pmt vector
 *
 * TODO: Implement this as a template function.
 */
static inline pmt::pmt_t init_data(std::vector<gr_complex> data) {
  return pmt::init_c32vector(data.size(), (const gr_complex *) &data[0]);
}

/*!
 * \brief Converts a pmt vector to a std::vector<gr_complex>
 */
static inline std::vector<gr_complex> init_std_vector(pmt::pmt_t data) {
  std::vector<gr_complex> out =
      std::vector<gr_complex>(pmt::length(data), 0);
  for (uint32_t ii = 0; ii < pmt::length(data); ii++)
    out[ii] = pmt::c32vector_ref(data, ii);
  return out;
}

/*!
 * \brief Creates a std::vector<float> ranging from [start, stop), linearly
 *  spaced by step
 */
template<typename T>
std::vector<T> arange(T start, T stop, T step) {
  std::vector<T> values;
  for (T value = start; value < stop; value += step)
    values.push_back(value);

  return values;
}
} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_VECTOR_FUNCTIONS_H */

