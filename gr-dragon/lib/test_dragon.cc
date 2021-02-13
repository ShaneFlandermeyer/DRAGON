/**
 * Filename: test_dragon.cc
 * Description: (Very incomplete) Unit testing script for the blocks
 * User: Shane Flandermeyer
 * Date: 1/12/21
 */

#include <dragon/GRCLBase.h>
//#include "waveform/lfm_source_impl.h"
#include <gnuradio/block.h>
//#include <gnuradio/gr_complex.h>

// TODO: Implement some unit tests.
bool testGRCLBase() {
  std::cout << "#################################################" << std::endl;

  std::cout << "Testing GRCLBase" << std::endl;

  gr::dragon::GRCLBase *test = nullptr;
  try {
    test = new gr::dragon::GRCLBase(DTYPE_COMPLEX, sizeof(gr_complex),0,
                                    true, false);
  }
  catch (...) {
    std::cout << "ERROR: error setting up OpenCL environment." << std::endl;

    if (test != nullptr)
      delete test;

    return false;

  }

  delete test;
  return true;
}

//bool testLFMSource() {
//  std::cout << "#################################################" << std::endl;
//  std::cout << "Testing LFM Source" << std::endl;
//  gr::dragon::lfm_source_impl *test = nullptr;
//  try {
//    double bandwidth = 5e6;
//    double sweep_time = 20e-6;
//    double samp_rate = 20e6;
//    double prf = 5000;
//    std::vector<gr::tag_t> tags(0);
//    test = new gr::dragon::lfm_source_impl(bandwidth, sweep_time,
//        samp_rate,
//        prf,tags);
//  }
//  catch (...) {
//    std::cout << "ERROR: error setting up LFM waveform source." << std::endl;
//
//      delete test;
//
//    return false;
//
//  }
//
//  delete test;
//  return true;
//
//}
int main(int argc, char **argv) {
  bool was_successful;
  was_successful = testGRCLBase();
//  was_successful = testLFMSource();

  return was_successful ? 0 : 1;

}