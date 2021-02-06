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

#ifndef INCLUDED_DRAGON_CONSTANTS_H
#define INCLUDED_DRAGON_CONSTANTS_H

#include "api.h"
#include <pmt/pmt.h>

/*!
 * \brief Contains constants and enums used by the blocks
 */
namespace gr {
namespace dragon {

static const uint32_t TX_MODE__SIMULATION = 0;
static const uint32_t TX_MODE__LOOPBACK = 1;

static const uint32_t EARLY_BURST_BEHAVIOR__APPEND = 0;
static const uint32_t EARLY_BURST_BEHAVIOR__DROP = 1;
static const uint32_t EARLY_BURST_BEHAVIOR__BALK = 2;

// message port names
static const pmt::pmt_t PMTCONSTSTR__FILTER = pmt::mp("filter");
static const pmt::pmt_t PMTCONSTSTR__MSG = pmt::mp("msg");
static const pmt::pmt_t PMTCONSTSTR__OUT = pmt::mp("out");
static const pmt::pmt_t PMTCONSTSTR__IN = pmt::mp("in");
static const pmt::pmt_t PMTCONSTSTR__DICT = pmt::mp("dict");
static const pmt::pmt_t PMTCONSTSTR__DATA = pmt::mp("data");
static const pmt::pmt_t PMTCONSTSTR__EMIT = pmt::mp("emit");
static const pmt::pmt_t PMTCONSTSTR__CTRL = pmt::mp("ctrl");
static const pmt::pmt_t PMTCONSTSTR__CONF = pmt::mp("conf");
static const pmt::pmt_t PMTCONSTSTR__BURSTS = pmt::mp("bursts");
static const pmt::pmt_t PMTCONSTSTR__DEBUG = pmt::mp("debug");
static const pmt::pmt_t PMTCONSTSTR__WINDOW = pmt::mp("window");
static const pmt::pmt_t PMTCONSTSTR__BURST_ID = pmt::mp("burst_id");

static const pmt::pmt_t PMTCONSTSTR__EOB_OFFSET = pmt::mp("eob_offset");
static const pmt::pmt_t PMTCONSTSTR__RX_TIME = pmt::mp("rx_time");
static const pmt::pmt_t PMTCONSTSTR__SAMP_RATE = pmt::mp("sample_rate");
static const pmt::pmt_t PMTCONSTSTR__SYM_RATE = pmt::mp("symbol_rate");

static const pmt::pmt_t PMTCONSTSTR__BURST_TIME = pmt::mp("burst_time");
static const pmt::pmt_t PMTCONSTSTR__START_TIME = pmt::mp("start_time");
static const pmt::pmt_t PMTCONSTSTR__START_TIME_OFFSET = pmt::mp(
    "start_time_offset");
static const pmt::pmt_t PMTCONSTSTR__TX_TIME = pmt::mp("tx_time");
static const pmt::pmt_t PMTCONSTSTR__TX_SOB = pmt::mp("tx_sob");
static const pmt::pmt_t PMTCONSTSTR__TX_EOB = pmt::mp("tx_eob");
static const pmt::pmt_t PMTCONSTSTR__TAPS = pmt::mp("taps");

// Radar parameters
static const pmt::pmt_t PMTCONSTSTR__BANDWIDTH = pmt::mp("bandwidth");
static const pmt::pmt_t PMTCONSTSTR__SWEEP_TIME = pmt::mp("sweep_time");
static const pmt::pmt_t PMTCONSTSTR__PRF = pmt::mp("prf");
static const pmt::pmt_t PMTCONSTSTR__PRI = pmt::mp("pri");

static const uint32_t MODE_MESSAGE = 0;
static const uint32_t MODE_TIMED_PDU = 1;

static const uint64_t TX_UNLIMITED = 0xFFFFFFFFFFFFFFFF; // = -1
static const uint64_t TX_OFF = 0;                        // no transmissions


// noise generation distributions
enum noise_dist {
  UNIFORM = 0, GAUSSIAN = 1
};

enum window_type {
  TUKEY_WIN = 0, GAUSSIAN_WIN = 1
}; // end enum window_type

//! pdu align modes
enum align_modes {
  //! do not emit a packet if sync word is not found
  ALIGN_DROP = 0,

  //! forward packet if sync word is not found
  ALIGN_FORWARD = 1,

  //! emit empty pdu if sync word is not found
  ALIGN_EMPTY = 2
};

enum db_mode {
  VOLTAGE = 0,
  POWER = 1
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_CONSTANTS_H */

