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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "lfm_source_impl.h"
#include <dragon/vector-utils/vector_functions.h>

namespace gr {
namespace dragon {

lfm_source::sptr
lfm_source::make(float bandwidth,
    float sweep_time,
    float samp_rate,
    float prf,
    const std::vector<tag_t> &tags) {
  return gnuradio::get_initial_sptr
      (new lfm_source_impl(bandwidth, sweep_time, samp_rate, prf, tags));
}

/*
 * The private constructor
 */
lfm_source_impl::lfm_source_impl(float bandwidth,
    float sweep_time,
    float samp_rate,
    float prf,
    const std::vector<tag_t> &tags)
    : gr::sync_block("lfm_source",
    gr::io_signature::make(0, 0, 0),
    gr::io_signature::make(1, 1, std::round(sizeof(gr_complex)*samp_rate/prf))),
    // Initialize the waveform parameters
      bandwidth(bandwidth),
      sweep_time(sweep_time),
      samp_rate(samp_rate),
      prf(prf),
      tags(tags),
      pri(1 / prf),
      pulse_ctr(0) {
  // Create the waveform from the above parameters
  generateWaveform();
  // Decide if the user wants tags
  if (tags.empty())
    set_tags = false;
  else
    set_tags = true;


}

/*
 * Our virtual destructor.
 */
lfm_source_impl::~lfm_source_impl() = default;

int
lfm_source_impl::work(int noutput_items,
    gr_vector_const_void_star &input_items,
    gr_vector_void_star &output_items) {

  auto *out = (gr_complex *) output_items[0];

  if (waveform.empty()) // No waveform specified
    return -1;
  for (int i = 0; i < noutput_items; i++) {
    if (param_updated) { // Parameters updated...create a new waveform
      generateWaveform();
      param_updated = false;
    }
    memcpy(out, waveform.data(), waveform.size() * sizeof(gr_complex));
    out += waveform.size();
    if (set_tags) {
      for (auto &tag : tags) {
        this->add_item_tag(0,
            this->nitems_written(0) + i * waveform.size() + tag.offset,
            tag.value, tag.srcid);
      }
    }

  }
  return noutput_items;


  // Tell runtime system how many output items we produced.
  return noutput_items;
}

void lfm_source_impl::generateWaveform() {
  float phase;
  std::vector<float> t;
  t = arange(static_cast<float>(0),
      static_cast<float>(sweep_time),
      static_cast<float>(1 / samp_rate));
  waveform = std::vector<gr_complex>(samp_rate * pri, 0);

  // Create the waveform
  for (int ii = 0; ii < t.size(); ii++) {
    phase = -2.0f * M_PI * (bandwidth / 2) * t[ii] + M_PI * bandwidth /
        sweep_time * std::pow(t[ii], 2);
    waveform[ii] = std::exp(J * phase);
  }

}
void lfm_source_impl::setBandwidth(float bandwidth) {
  this->bandwidth = bandwidth;
  param_updated = true;
}

void lfm_source_impl::setSweepTime(float sweep_time) {
  this->sweep_time = sweep_time;
  param_updated = true;
  }

void lfm_source_impl::setSampRate(float samp_rate) {
  this->samp_rate = samp_rate;
  param_updated = true;
}

void lfm_source_impl::setPRF(float prf) {
  this->prf = prf;
  param_updated = true;
}

} /* namespace dragon */
} /* namespace gr */

