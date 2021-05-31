/* -*- c++ -*- */
/*
 * Copyright 2021 gr-waveform author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_WAVEFORM_LINEAR_FM_SOURCE_H
#define INCLUDED_WAVEFORM_LINEAR_FM_SOURCE_H

#include <waveform/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace waveform {

    /*!
     * \brief <+description of block+>
     * \ingroup waveform
     *
     */
    class WAVEFORM_API linear_fm_source : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<linear_fm_source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of waveform::linear_fm_source.
       *
       * To avoid accidental use of raw pointers, waveform::linear_fm_source's
       * constructor is in a private implementation
       * class. waveform::linear_fm_source::make is the public interface for
       * creating new instances.
       */
      static sptr make(float bandwidth, float pulsewidth, float sampleRate);
    };

  } // namespace waveform
} // namespace gr

#endif /* INCLUDED_WAVEFORM_LINEAR_FM_SOURCE_H */

