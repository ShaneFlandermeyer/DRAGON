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

#ifndef INCLUDED_DRAGON_PULSE_SHAPER_H
#define INCLUDED_DRAGON_PULSE_SHAPER_H

#include "cpm.h"
#include <dragon/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace dragon {

    /*!
     * \brief <+description of block+>
     * \ingroup dragon
     *
     */
    class DRAGON_API pulse_shaper : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<pulse_shaper> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dragon::pulse_shaper.
       *
       * To avoid accidental use of raw pointers, dragon::pulse_shaper's
       * constructor is in a private implementation
       * class. dragon::pulse_shaper::make is the public interface for
       * creating new instances.
       */
      static sptr make(dragon::cpm::cpm_type type, int vlen, int filt_len, int oversampling, float h, double beta);
    };

  } // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PULSE_SHAPER_H */

