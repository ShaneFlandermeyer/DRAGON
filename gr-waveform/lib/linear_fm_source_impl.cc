/* -*- c++ -*- */
/*
 * Copyright 2021 gr-waveform author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "linear_fm_source_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace waveform {

using output_type = gr_complex;
linear_fm_source::sptr
linear_fm_source::make(float bandwidth, float pulsewidth, float sampleRate)
{
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
                     gr::io_signature::make(1, 1, sizeof(output_type))),
      d_bandwidth(bandwidth),
      d_pulsewidth(pulsewidth),
      d_sampleRate(sampleRate),
      d_waveform(std::vector<gr_complex>(sampleRate*pulsewidth))
{
    generate_waveform();
}

/*
 * Our virtual destructor.
 */
linear_fm_source_impl::~linear_fm_source_impl() {}

int linear_fm_source_impl::work(int noutput_items,
                                gr_vector_const_void_star& input_items,
                                gr_vector_void_star& output_items)
{
    output_type* out = reinterpret_cast<output_type*>(output_items[0]);
    // TODO: This functionality does not allow the user to change the waveform with
    //  messages. To handle this, implement a "waveform changed" flag in a message
    //  function so that the waveform does not change until the current waveform has
    //  been fully transmitted
    for (int iBuffer = 0; iBuffer < noutput_items; iBuffer++) {
        out[iBuffer] = d_waveform[d_currentSample % d_waveform.size()];
        d_currentSample++;
    }


    // Tell runtime system how many output items we produced.
    return noutput_items;
}

void linear_fm_source_impl::generate_waveform() {

    float sampleInterval = 1 / d_sampleRate;
    gr_complex  J(0,1); // TODO: Move this to a constants file
    float time, phase;
    for (int ii = 0; ii < (int)(d_pulsewidth*d_sampleRate); ii++) {
        time = (float) ii * sampleInterval;
        phase = M_PI * d_bandwidth * time * (time / d_pulsewidth -1);
        d_waveform[ii] = std::exp(J*phase);

    }

}



} /* namespace waveform */
} /* namespace gr */
