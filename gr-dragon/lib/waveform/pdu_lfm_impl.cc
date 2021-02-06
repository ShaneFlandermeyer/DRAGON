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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pdu_lfm_impl.h"
#include <dragon/vector-utils/vector_functions.h>



namespace gr {
namespace dragon {

pdu_lfm::sptr
pdu_lfm::make(float bandwidth, float sweep_time, float samp_rate, float prf,
              uint32_t mode, bool repeat) {
  return gnuradio::get_initial_sptr
      (new pdu_lfm_impl(bandwidth, sweep_time, samp_rate, prf, mode,
                        repeat));
}

/*
 * The private constructor
 */
pdu_lfm_impl::pdu_lfm_impl(float bandwidth, float sweep_time, float samp_rate,
                           float prf, uint32_t mode, bool repeat)
    : block("pdu_lfm",
            gr::io_signature::make(0, 0, 0),
            gr::io_signature::make(0, 0, 0)),
      d_bandwidth(bandwidth),
      d_sweep_time(sweep_time),
      d_samp_rate(samp_rate),
      d_prf(prf),
      d_pri(1 / d_prf),
      d_out_port(PMTCONSTSTR__OUT),
      d_ctrl_port(PMTCONSTSTR__CTRL),
      d_filt_port(PMTCONSTSTR__FILTER),
      d_mode(mode),
      d_repeat(repeat),
      d_pulse_ctr(0) {

  message_port_register_in(d_ctrl_port);
  message_port_register_out(d_out_port);
  message_port_register_out(d_filt_port);
  set_msg_handler(d_ctrl_port,
                  [this](pmt::pmt_t msg) { this->handle_ctrl_msg(msg); });
  generate_waveform();

}

/*
 * Our virtual destructor.
 */
pdu_lfm_impl::~pdu_lfm_impl() = default;

void pdu_lfm_impl::handle_ctrl_msg(pmt::pmt_t msg) {
  // If the message isn't a dictionary, do nothing
  if (!pmt::is_dict(msg))
    return;
  // Get the dictionary and convert the message to a dictionary if it's a pair
  try {
    pmt::pmt_t keys = pmt::dict_keys(msg);
    msg = pmt::car(msg);
  } catch (const pmt::wrong_type &e) {
    // So we fix it:
    GR_LOG_WARN(this->d_logger, "got a pair, not a dict, fixing");
    msg = pmt::dict_add(pmt::make_dict(), pmt::car(msg), pmt::cdr(msg));
  }
  // Handle command to change bandwidth
  if (pmt::dict_has_key(msg, PMTCONSTSTR__BANDWIDTH)) {
    float new_bandwidth = pmt::to_float(
        pmt::dict_ref(msg, PMTCONSTSTR__BANDWIDTH,
                      pmt::from_float(d_bandwidth)));
    if (new_bandwidth > 0) {
      set_bandwidth(new_bandwidth);
    }
    // TODO: Make commands for the rest of the parameters
  }
}

void pdu_lfm_impl::run() {
  while (!d_finished) {
    // Transmit a new waveform if we're in repeat mode or if we aren't in
    // repeat mode but haven't sent anything yet
    if (d_repeat || d_pulse_ctr < 1) {
      if (d_finished) {
        return;
      }
      // Send the waveform through the "out" message port
      this->message_port_pub(d_out_port,
                             pmt::cons(pmt::make_dict(), d_waveform));
      this->message_port_pub(d_filt_port,
                             pmt::cons(pmt::make_dict(), d_match_filt));
      // Only send a new matched filter if we've changed the waveform
//      if (d_updated) {
//        pmt::pmt_t msg = pmt::dict_add(pmt::make_dict(), PMTCONSTSTR__TAPS,
//                                       d_match_filt);
//        this->message_port_pub(d_filt_port, msg);
//        d_updated = false;
//      }
      d_pulse_ctr++; // Increment the pulse counter

      /*
       * Put the thread to sleep for a PRI duration
       *
       * TODO: In theory, this provides a "cheaper" alternative (from a
       *  memory and data throughput perspective) to transmitting
       *  zeros for timing, but we still need to test how flowgraph latency
       *  throws it off (if at all).
       */
      long sleep_time_us = static_cast<long>((d_pri) * 1e6);
      boost::this_thread::sleep(
          boost::posix_time::microseconds(sleep_time_us));
    } else
      d_finished = true;
  } // while
} /* function run */

bool pdu_lfm_impl::start() {
  // Create a new thread for the block
  d_finished = false;
  d_thread = boost::shared_ptr<gr::thread::thread>(
      new gr::thread::thread(boost::bind
                                 (&pdu_lfm_impl::run,
                                  this)));
  return block::start();
}

bool pdu_lfm_impl::stop() {
  // Shut down the thread
  d_finished = true;
  d_thread->interrupt();
  d_thread->join();

  return block::stop();
}

//template<typename T>
//std::vector<T> pdu_lfm_impl::arange(T start, T stop, T step) {
//  std::vector<T> values;
//  for (T value = start; value < stop; value += step)
//    values.push_back(value);
//  return values;
//}

/*!
 * \brief Generates an LFM waveform (and matched filter) from the block params
 */
void pdu_lfm_impl::generate_waveform() {
  float phase; // Phase at each time
  std::vector<float> t; // Time vector (spaced by samp_rate)
  std::vector<gr_complex> out; // Output waveform
  t = arange(static_cast<float>(0),
             static_cast<float>(d_sweep_time),
             static_cast<float>(1 / d_samp_rate));

  if (d_mode == TX_MODE__SIMULATION) // Zero pad
    out = std::vector<gr_complex>(d_samp_rate * d_pri, 0);
  else if (d_mode == TX_MODE__LOOPBACK) // Don't zero pad
    out = std::vector<gr_complex>(d_samp_rate * d_sweep_time, 0);

  // Create the waveform
  for (int ii = 0; ii < t.size(); ii++) {
    phase = -2 * M_PI * (d_bandwidth / 2) * t[ii] + M_PI * d_bandwidth /
        d_sweep_time *
        std::pow(t[ii], 2);
    out[ii] = std::exp(J * phase);
  }
  // Make the waveform a pmt::pmt_t
  d_waveform = init_data(out);

  std::vector<gr_complex> filt(out.begin(), out.begin() + t.size());
  // Create a matched filter (time-reverse and complex conjugate))
  std::reverse(filt.begin(), filt.end()); // Time-reversal
  std::transform(filt.begin(), filt.end(), filt.begin(),
                 [](const std::complex<float> &c) -> std::complex<float> {
                   return std::conj(c);
                 }); // Complex conjugate each element
  d_match_filt = dragon::init_data(filt);
  if (VERBOSE) {
    std::cout << "Waveform Length: " << pmt::length(d_waveform) << std::endl;
    std::cout << "Matched filter length: " << pmt::length(d_match_filt)
              << std::endl << std::endl;
  }
  d_updated = true; // Tell the thread we have a new matched filter
}

/*
 * TODO: Implement setters for the rest of the parameters
 */
void pdu_lfm_impl::set_bandwidth(float bandwidth) {
  std::cout << "Bandwidth Updated: " << d_bandwidth << " -> " << bandwidth <<
  std::endl;
  gr::thread::scoped_lock l(this->d_setlock);

  d_bandwidth = bandwidth;
  generate_waveform();
}

//void pdu_lfm_impl::set_samp_rate(float samp_rate) {
//  gr::thread::scoped_lock l(this->d_setlock);
//
//  d_samp_rate = samp_rate;
//  d_waveform = pmt::make_c32vector(d_sweep_time * samp_rate, 0);
//
//}
//
//void pdu_lfm_impl::set_sweep_time(float sweep_time) {
//  gr::thread::scoped_lock l(this->d_setlock);
//
//  d_sweep_time = sweep_time;
//  d_waveform = pmt::make_c32vector(d_samp_rate * sweep_time, 0);
//
//}
//
//void pdu_lfm_impl::set_prf(float prf) {
//  gr::thread::scoped_lock l(this->d_setlock);
//
//  d_prf = prf;
//  d_pri = 1 / d_prf;
//
//}
//
//void pdu_lfm_impl::set_pri(float pri) {
//  gr::thread::scoped_lock l(this->d_setlock);
//
//  d_pri = pri;
//  d_prf = 1 / d_pri;
//
//}

} /* namespace dragon */
} /* namespace gr */

