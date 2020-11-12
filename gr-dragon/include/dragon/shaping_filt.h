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

#ifndef INCLUDED_DRAGON_SHAPING_FILT_H
#define INCLUDED_DRAGON_SHAPING_FILT_H

#include <dragon/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace dragon {

    /*!
     * \brief <+description of block+>
     * \ingroup dragon
     *
     */
    class DRAGON_API shaping_filt : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<shaping_filt> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dragon::shaping_filt.
       *
       * To avoid accidental use of raw pointers, dragon::shaping_filt's
       * constructor is in a private implementation
       * class. dragon::shaping_filt::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string filt_type, int filt_len, int vlen);
    };

  } // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_SHAPING_FILT_H */

