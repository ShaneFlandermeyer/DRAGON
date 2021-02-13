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

#ifndef INCLUDED_DRAGON_LFM_SOURCE_H
#define INCLUDED_DRAGON_LFM_SOURCE_H

#include "../api.h"
#include <gnuradio/sync_block.h>

namespace gr {
namespace dragon {

/*!
 * \brief <+description of block+>
 * \ingroup dragon
 *
 */
class DRAGON_API lfm_source : virtual public gr::sync_block {
 public:
  typedef boost::shared_ptr<lfm_source> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of dragon::lfm_source.
   *
   * To avoid accidental use of raw pointers, dragon::lfm_source's
   * constructor is in a private implementation
   * class. dragon::lfm_source::make is the public interface for
   * creating new instances.
   */
  static sptr make(float bandwidth,
      float sweep_time,
      float samp_rate,
      float prf,
      const std::vector<tag_t> &tags);

  /*
   * Callback functions
   */
  virtual void setBandwidth(float bandwidth) = 0;
  virtual void setSweepTime(float sweep_time) = 0;
  virtual void setSampRate(float samp_rate) = 0;
  virtual void setPRF(float prf) = 0;
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_LFM_SOURCE_H */

