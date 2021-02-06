/**
 * Filename: test_dragon.cc
 * Description: (Very incomplete) Unit testing script for the blocks
 * User: Shane Flandermeyer
 * Date: 1/12/21
 */

#include <dragon/GRCLBase.h>
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
int main(int argc, char **argv) {
  bool was_successful;
  was_successful = testGRCLBase();

  return was_successful ? 0 : 1;

}