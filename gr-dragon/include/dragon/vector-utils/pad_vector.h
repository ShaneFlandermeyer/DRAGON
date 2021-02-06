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

#ifndef INCLUDED_DRAGON_PAD_VECTOR_H
#define INCLUDED_DRAGON_PAD_VECTOR_H

#include "../api.h"
#include <gnuradio/sync_block.h>

namespace gr {
namespace dragon {

/*!
 * \brief Resizes an input vector to a given size
 * \ingroup dragon
 *
 * Equivalent to std::vector::resize(n,0)
 */
class DRAGON_API pad_vector : virtual public gr::sync_block {
 public:
  typedef boost::shared_ptr<pad_vector> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of dragon::pad_vector.
   *
   * To avoid accidental use of raw pointers, dragon::pad_vector's
   * constructor is in a private implementation
   * class. dragon::pad_vector::make is the public interface for
   * creating new instances.
   */
  static sptr make(uint32_t old_len, uint32_t new_len);
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PAD_VECTOR_H */

