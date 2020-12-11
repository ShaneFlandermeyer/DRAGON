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

#ifndef INCLUDED_DRAGON_MOVING_TARGET_SIM_H
#define INCLUDED_DRAGON_MOVING_TARGET_SIM_H

#include <dragon/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace dragon {

/*!
 * \brief <+description of block+>
 * \ingroup dragon
 *
 */
class DRAGON_API moving_target_sim

: virtual public gr::block {
public:
typedef boost::shared_ptr <moving_target_sim> sptr;

/*!
 * \brief Return a shared_ptr to a new instance of dragon::moving_target_sim.
 *
 * To avoid accidental use of raw pointers, dragon::moving_target_sim's
 * constructor is in a private implementation
 * class. dragon::moving_target_sim::make is the public interface for
 * creating new instances.
 */
static sptr
make(std::vector<float> tgt_rng, std::vector<float> tgt_vel, std::vector<float> tgt_rcs,
     float tx_freq, float prf, float samp_rate,uint32_t mode);

virtual void set_tgt_rng(std::vector<float> tgt_rng) = 0;
virtual void set_tgt_vel(std::vector<float> tgt_vel) = 0;
virtual void set_tgt_rcs(std::vector<float> tgt_rcs) = 0;
virtual void set_tx_freq(float tx_freq) = 0;
virtual void set_prf(float prf) = 0;
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_MOVING_TARGET_SIM_H */

