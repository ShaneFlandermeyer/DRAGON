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

#ifndef INCLUDED_DRAGON_CL_DB_IMPL_H
#define INCLUDED_DRAGON_CL_DB_IMPL_H

#include <dragon/cl_db.h>
#include <dragon/GRCLBase.h>

namespace gr {
namespace dragon {

class cl_db_impl : public cl_db, public GRCLBase {
 private:
  std::string src_std_str;
  std::string function_name;

  uint32_t mode;
  cl::Buffer *in_buffer = nullptr;
  cl::Buffer *out_buffer = nullptr;
  int curr_buffer_size = 0;

  void buildKernel(int num_items);

 public:
  cl_db_impl(int ocl_platform_type,
             int dev_selector,
             int platform_id,
             int dev_id,
             uint32_t mode,
             int set_debug);
  ~cl_db_impl();
  bool stop();

  void setBufferLength(int num_items);

  int processOpenCL(int noutput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items);

  // Where all the action really happens
  int work(
      int noutput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items
  );
};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_CL_DB_IMPL_H */

