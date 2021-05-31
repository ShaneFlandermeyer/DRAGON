/* -*- c++ -*- */
/*
 * Copyright 2021 gr-waveform author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "linear_fm_source_impl.h"

namespace gr {
namespace waveform {

#pragma message("set the following appropriately and remove this warning")
using output_type = gr_complex;
linear_fm_source::sptr
linear_fm_source::make(float bandwidth, float pulsewidth, float sampleRate) {
  return gnuradio::make_block_sptr<linear_fm_source_impl>(
      bandwidth, pulsewidth, sampleRate);
}

/*
 * The private constructor
 */
linear_fm_source_impl::linear_fm_source_impl(float bandwidth,
    float pulsewidth,
    float sampleRate)
    : gr::sync_block("linear_fm_source",
    gr::io_signature::make(0, 0, 0),
    gr::io_signature::make(1 /* min outputs */,
        1 /*max outputs */, sizeof(output_type))),
      d_bandwidth(bandwidth),
      d_pulsewidth(pulsewidth),
      d_sampleRate(sampleRate) {
  generate_waveform();

}

/*
 * Our virtual destructor.
 */
linear_fm_source_impl::~linear_fm_source_impl() {
}

int
linear_fm_source_impl::work(int noutput_items,
    gr_vector_const_void_star &input_items,
    gr_vector_void_star &output_items) {
  output_type *out = reinterpret_cast<output_type *>(output_items[0]);

#pragma message("Implement the signal processing in your block and remove this warning")
  // Do <+signal processing+>

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

void linear_fm_source_impl::generate_waveform() {
  
}
} /* namespace waveform */
} /* namespace gr */

