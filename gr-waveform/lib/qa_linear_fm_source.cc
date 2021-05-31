/* -*- c++ -*- */
/*
 * Copyright 2021 gr-waveform author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include <waveform/linear_fm_source.h>
#include <gnuradio/attributes.h>
#include <boost/test/unit_test.hpp>
#include <gnuradio/top_block.h>

#include <gnuradio/blocks/head.h>
#include <gnuradio/blocks/null_source.h>
#include <gnuradio/blocks/null_sink.h>
#include <gnuradio/blocks/vector_sink.h>

namespace gr {
namespace waveform {

#define VERBOSE 0

/*
 * Test if block can be instantiated
 */
BOOST_AUTO_TEST_CASE(t0) {
    auto lfm = linear_fm_source::make(1, 1, 1);

    BOOST_REQUIRE(lfm);
}

/*
 * Test the actual waveform data over one period
 */
BOOST_AUTO_TEST_CASE(t1) {
    top_block_sptr tb = make_top_block("top");
    // Waveform parameters
    float bandwidth = 10e6;
    float pulsewidth = 10e-6;
    float sampleRate = 10e6;
    // Test blocks
    auto lfm = linear_fm_source::make(bandwidth, pulsewidth, sampleRate);
    auto head = blocks::head::make(sizeof(gr_complex), pulsewidth*sampleRate);
    auto sink = blocks::vector_sink_c::make();

    // Create the flowgraph
    tb->connect(lfm, 0, head, 0);
    tb->connect(head, 0, sink, 0);
    tb->run();

    // Control case
    float time;
    float phase;
    gr_complex j(0, 1);
    std::vector<gr_complex> expected(pulsewidth*sampleRate);
    for (int ii = 0; ii < expected.size(); ii++) {
      time = ii / sampleRate;
      phase = -2 * M_PI * (bandwidth / 2) * time + M_PI * bandwidth /
          pulsewidth * std::pow(time, 2);
      expected[ii] = std::exp(j * phase);
    }
    // Get the flowgraph output
    auto actual = sink->data();
    // Test the output
    BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(),
    actual.begin(), actual.end());
}

} /* namespace waveform */
} /* namespace gr */
