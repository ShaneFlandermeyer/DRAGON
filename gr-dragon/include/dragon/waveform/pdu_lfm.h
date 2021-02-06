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

#ifndef INCLUDED_DRAGON_PDU_LFM_H
#define INCLUDED_DRAGON_PDU_LFM_H

#include "../api.h"
#include <gnuradio/sync_block.h>

namespace gr {
namespace dragon {

/*!
 * \brief Generates an LFM waveform as a PDU message.
 * \ingroup dragon
 *
 */
class DRAGON_API pdu_lfm : virtual public gr::block {
 public:
  typedef boost::shared_ptr<pdu_lfm> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of dragon::pdu_lfm.
   *
   * To avoid accidental use of raw pointers, dragon::pdu_lfm's
   * constructor is in a private implementation
   * class. dragon::pdu_lfm::make is the public interface for
   * creating new instances.
   */
  static sptr
  make(float bandwidth, float sweep_time, float samp_rate, float prf,
       uint32_t mode, bool repeat);

  virtual void set_bandwidth(float bandwidth) = 0;
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PDU_LFM_H */

