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

#ifndef INCLUDED_DRAGON_LFM_SOURCE_IMPL_H
#define INCLUDED_DRAGON_LFM_SOURCE_IMPL_H

#include <dragon/waveform/lfm_source.h>

namespace gr {
namespace dragon {

class lfm_source_impl : public lfm_source {
 private:

  /*
   * Constants
   */
  const gr_complex J = gr_complex(0, 1);

  bool param_updated = false; //
  /*
   * Waveform parameters
   */
  float bandwidth;
  float samp_rate;
  float sweep_time;
  float prf;
  float pri;
  std::vector<gr_complex> waveform; // Output waveform

  std::vector<tag_t> tags;
  bool set_tags; // Indicates whether or not tags should be appended

  uint32_t pulse_ctr; // Tracks pulse progression over time
  /*
   * Keeps track of how much of the waveform we've transmitted at the start
   * of work()
   */
  uint32_t offset;

  /*
   * Private functions
   */
  void generateWaveform(); // Generates a new LFM waveform

 public:
  lfm_source_impl(float bandwidth,
      float sweep_time,
      float samp_rate,
      float prf,
      const std::vector<tag_t>& tags);
  ~lfm_source_impl() override;

  // Where all the action really happens
  int work(
      int noutput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items
  );

  /*
   * Callback functions
   */
  void setBandwidth(float bandwidth) override;
  void setSweepTime(float sweep_time) override;
  void setSampRate(float samp_rate) override;
  void setPRF(float prf) override;



};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_LFM_SOURCE_IMPL_H */

