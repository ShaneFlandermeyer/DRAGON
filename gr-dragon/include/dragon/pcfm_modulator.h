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

#ifndef INCLUDED_DRAGON_PCFM_MODULATOR_H
#define INCLUDED_DRAGON_PCFM_MODULATOR_H

#include <dragon/api.h>
#include <gnuradio/hier_block2.h>

namespace gr {
  namespace dragon {

    /*!
     * \brief <+description of block+>
     * \ingroup dragon
     *
     */
    class DRAGON_API pcfm_modulator : virtual public gr::hier_block2
    {
     public:
      typedef boost::shared_ptr<pcfm_modulator> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dragon::pcfm_modulator.
       *
       * To avoid accidental use of raw pointers, dragon::pcfm_modulator's
       * constructor is in a private implementation
       * class. dragon::pcfm_modulator::make is the public interface for
       * creating new instances.
       */
      static sptr make(int code_len, int oversampling, int samp_rate);
    };

  } // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PCFM_MODULATOR_H */

