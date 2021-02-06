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
#include "cl_db_impl.h"
#include <dragon/constants.h>

namespace gr {
namespace dragon {

cl_db::sptr
cl_db::make(int ocl_platform_type,
            int dev_selector,
            int platform_id,
            int dev_id,
            uint32_t mode,
            int set_debug) {
  return gnuradio::get_initial_sptr
      (new cl_db_impl(ocl_platform_type,
                      dev_selector,
                      platform_id,
                      dev_id,
                      mode,
                      set_debug));
}

/*
 * The private constructor
 */
cl_db_impl::cl_db_impl(int ocl_platform_type,
                       int dev_selector,
                       int platform_id,
                       int dev_id,
                       uint32_t mode,
                       int set_debug)
    : gr::sync_block("cl_db",
                     gr::io_signature::make(1, 1, sizeof(float)),
                     gr::io_signature::make(1, 1, sizeof(float))),
      GRCLBase(DTYPE_FLOAT, sizeof(float), ocl_platform_type, dev_selector,
               platform_id, dev_id, set_debug, false),
      mode(mode) {
  int max_input_items = gr::block::max_noutput_items();
  if (max_input_items == 0)
    max_input_items = 8192;

  setBufferLength(max_input_items);

  try {
    if (context_type != CL_DEVICE_TYPE_CPU)
      gr::block::set_output_multiple(preferred_work_group_size_multiple);
    else
      gr::block::set_output_multiple(32);
  }
  catch (...) {

  }

}

/*
 * Our virtual destructor.
 */
cl_db_impl::~cl_db_impl() {
  if (curr_buffer_size > 0)
    stop();
}

bool cl_db_impl::stop() {
  curr_buffer_size = 0;

  if (in_buffer) {
    delete in_buffer;
    in_buffer = nullptr;
  }

  if (out_buffer) {
    delete out_buffer;
    out_buffer = nullptr;
  }

  return GRCLBase::stop();
}

void cl_db_impl::buildKernel(int num_items) {
  max_const_items = (int) ((float) max_const_mem_size / (float) data_size);
  bool use_const;

  if (num_items > max_const_items)
    use_const = false;
  else
    use_const = true;

  if (debug_mode) {
    if (use_const)
      std::cout
          << "OpenCL INFO: dB Const building kernel with __constant params..."
          << std::endl;
    else
      std::cout
          << "OpenCL Info: dB - too many items for constant memory. Building kernel with __global params..."
          << std::endl;
  }

  src_std_str = "";
  function_name = "op_db";

  if (use_const)
    src_std_str +=
        "__kernel void op_db(__constant float * a, __global float * restrict c) {\n";
  else
    src_std_str +=
        "__kernel void op_db(__global float * a, __global float * restrict c) {\n";

  src_std_str += "  size_t index = get_global_id(0);\n";

  if (mode == VOLTAGE)
    src_std_str += "  c[index] = 20 * log10(a[index]);\n";
  else if (mode == POWER)
    src_std_str += "  c[index] = 10 * log10(a[index]);\n";
  src_std_str += "}\n";

  GRCLBase::CompileKernel((const char *) src_std_str.c_str(),
                          (const char *) function_name.c_str());

}

void cl_db_impl::setBufferLength(int num_items) {

  delete in_buffer;
  delete out_buffer;

  in_buffer = new cl::Buffer(*context,
                             CL_MEM_READ_ONLY,
                             num_items * data_size);

  out_buffer = new cl::Buffer(*context,
                              CL_MEM_READ_WRITE,
                              num_items * data_size);

  buildKernel(num_items);

  curr_buffer_size = num_items;

}

int cl_db_impl::processOpenCL(int noutput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star &output_items) {
  if (kernel == nullptr)
    return 0;

  if (noutput_items > curr_buffer_size)
    setBufferLength(noutput_items);

  int in_size = noutput_items * data_size;

  // Protect context from switching
  gr::thread::scoped_lock guard(d_mutex);

  queue->enqueueWriteBuffer(*in_buffer, CL_FALSE, 0, in_size, input_items[0]);

  // Set kernel arguments
  kernel->setArg(0, *in_buffer);
  kernel->setArg(1, *out_buffer);

  cl::NDRange local_wg_size = cl::NullRange;

  if (context_type != CL_DEVICE_TYPE_CPU &&
      noutput_items % preferred_work_group_size_multiple == 0) {
    local_wg_size = cl::NDRange(preferred_work_group_size_multiple);
  }

  // Do the work
  queue->enqueueNDRangeKernel(*kernel, cl::NullRange,
                              cl::NDRange(noutput_items), local_wg_size);

  // Map out_buffer to the host pointer. This enforces a sync with the host
  queue->enqueueReadBuffer(*out_buffer, CL_TRUE, 0, in_size,
                           (void *) output_items[0]);

  // Tell the runtime system how many output items we produced
  return noutput_items;

}

int
cl_db_impl::work(int noutput_items,
                 gr_vector_const_void_star &input_items,
                 gr_vector_void_star &output_items) {
  if (debug_mode)
    std::cout << "clLog noutput_items: " << noutput_items << std::endl;
  int ret_val = processOpenCL(noutput_items, input_items, output_items);

  // Tell runtime system how many output items we produced.
  return ret_val;
}
} /* namespace dragon */
} /* namespace gr */

