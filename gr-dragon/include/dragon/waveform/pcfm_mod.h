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

#ifndef INCLUDED_DRAGON_PCFM_MOD_H
#define INCLUDED_DRAGON_PCFM_MOD_H

#include "cpm.h"
#include "../api.h"
#include <gnuradio/hier_block2.h>

namespace gr {
namespace dragon {

/*!
 * \brief Modulates the input with a PCFM scheme.
 * \ingroup dragon
 *
 */
class DRAGON_API pcfm_mod : virtual public gr::hier_block2 {
public:
  typedef boost::shared_ptr<pcfm_mod> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of dragon::pcfm_mod.
   *
   * To avoid accidental use of raw pointers, dragon::pcfm_mod's
   * constructor is in a private implementation
   * class. dragon::pcfm_mod::make is the public interface for
   * creating new instances.
   */
  static sptr make(dragon::cpm::cpm_type type, int code_len, int oversampling,
                   int samp_rate);

//  //! Return the type of shaping filter
//  virtual int type() const = 0;
//
//  //! Return the length of the code
//  virtual int code_len() const = 0;
//
//  //! Return the number of samples per symbol
//  virtual int oversampling() const = 0;
//
//  //! Return the sampling rate
//  virtual int samp_rate() const = 0;
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PCFM_MOD_H */
