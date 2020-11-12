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

#ifndef INCLUDED_DRAGON_DIFFERENCE_H
#define INCLUDED_DRAGON_DIFFERENCE_H

#include <dragon/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace dragon {

/*!
 * \brief <+description of block+>
 * \ingroup dragon
 *
 */
template<class T>
class DRAGON_API difference : virtual public gr::sync_block {
public:
    typedef boost::shared_ptr<difference<T>> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of dragon::difference.
     *
     * To avoid accidental use of raw pointers, dragon::difference's
     * constructor is in a private implementation
     * class. dragon::difference::make is the public interface for
     * creating new instances.
     */
    static sptr make(unsigned int vlen);
};

typedef difference<float> difference_ff;
typedef difference<gr_complex> difference_cc;

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_DIFFERENCE_H */

