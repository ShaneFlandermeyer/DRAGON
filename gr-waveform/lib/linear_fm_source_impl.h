/* -*- c++ -*- */
/*
 * Copyright 2021 gr-waveform author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_WAVEFORM_LINEAR_FM_SOURCE_IMPL_H
#define INCLUDED_WAVEFORM_LINEAR_FM_SOURCE_IMPL_H

#include <waveform/linear_fm_source.h>

namespace gr {
namespace waveform {

class linear_fm_source_impl : public linear_fm_source
{
private:
    unsigned long long int d_currentSample;
    float d_bandwidth;
    float d_pulsewidth;
    float d_sampleRate;
    std::vector<gr_complex> d_waveform;
    // Private functions
    void generate_waveform();

public:
    linear_fm_source_impl(float bandwidth, float pulsewidth, float sampleRate);
    ~linear_fm_source_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace waveform
} // namespace gr

#endif /* INCLUDED_WAVEFORM_LINEAR_FM_SOURCE_IMPL_H */
