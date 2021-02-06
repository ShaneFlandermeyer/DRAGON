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

#ifndef INCLUDED_DRAGON_GRCLBASE_H
#define INCLUDED_DRAGON_GRCLBASE_H

// Use CPP exception handling.
// Note: If you include cl.hpp, the compiler just won't find cl::Error class.
// You have to use cl2.hpp to get it to go away
#define __CL_ENABLE_EXCEPTIONS
// Disable the deprecated functions warning.  If you want to keep support for 1.2 devices
// You need to use the deprecated functions.  This #define makes the warning go away.
// #define CL_USE_DEPRECATED_OPENCL_1_2_APIS
// #define CL_VERSION_1_2
#define CL_VERSION_2_2
#define CL_TARGET_OPENCL_VERSION 220

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else
//#include <CL/cl.h>
#include <CL/cl.hpp>
#endif

#include "clSComplex.h"
#include "api.h"

#include <boost/thread/mutex.hpp>

#define DTYPE_COMPLEX 1
#define DTYPE_FLOAT 2
#define DTYPE_INT 3
#define DTYPE_SHORT 4
#define DTYPE_BYTE 5
#define DTYPE_PACKEDXY 6

#define OCLTYPE_GPU 1
#define OCLTYPE_ACCELERATOR 2
#define OCLTYPE_CPU 3
#define OCLTYPE_ANY 4

#define OCLDEVICESELECTOR_FIRST 1
#define OCLDEVICESELECTOR_SPECIFIC 2

namespace gr {
namespace dragon {

extern bool
    CLPRINT_NITEMS;  // Enable this in GRCLBase.cpp to print the number of items passed to the work functions if debug is enabled for the module

class DRAGON_API GRCLBase {
 protected:
  int data_type;
  size_t data_size;
  int device_type;
  int device_selector;
  int platform_id;
  int device_idx;
  int preferred_work_group_size_multiple = 1;
  int max_work_group_size = 1;
  int max_const_mem_size = 0;
  bool has_svm;
  bool has_svm_fine_grained;
  int optimal_buffer_type = CL_MEM_USE_HOST_PTR;

  bool has_single_fma_support = false;  // Fused Multiply/Add
  bool has_double_fma_support = false;  // Fused Multiply/Add
  bool has_double_precision_support = false;

  int max_const_items = 0;

  bool debug_mode = false;



  boost::mutex d_mutex;

  virtual void cleanup();

  // opencl variables
  cl::Program *program = nullptr;
  cl::Context *context = nullptr;
  std::vector<cl::Device> devices;
  int dev_idx = 0;
  cl::Program::Sources *sources = nullptr;
  cl::CommandQueue *queue = nullptr;
  cl::Kernel *kernel = nullptr;

  cl_device_type context_type;

  std::string platform_name;
  std::string platform_vendor;
  std::vector<std::string> device_names;
  std::vector<std::string> device_types;

  bool CompileKernel(const char *kernel_code,
                     const char *kernel_function_name,
                     bool exit_on_fail = true);

  virtual void InitOpenCL(int data_type,
                          size_t data_size,
                          int device_type,
                          int device_selector,
                          int platform_id,
                          int device_idx,
                          bool set_debug = false,
                          bool out_of_order_queue = false);

 public:
  bool hasSVMAvailable() const { return has_svm; };

  int MaxConstItems() const { return max_const_items; };

  std::string debugDisplayName;

  void SetDebugMode(bool new_mode) { debug_mode = new_mode; };

  int ActiveContextType() const { return context_type; };

  std::string getPlatformName() { return platform_name; };
  std::string getVendorName() { return platform_vendor; };

  GRCLBase(int idata_type,
           size_t dsize,
           int openCLPlatformType,
           bool setDebug = false,
           bool outOfOrderQueue = false); // selects First of specified type
  GRCLBase(int idata_type,
           size_t dsize,
           int openCLPlatformType,
           int devSelector,
           int platformId,
           int devId,
           bool setDebug = false,
           bool outOfOrderQueue = false);
  virtual ~GRCLBase();
  virtual bool stop();

  cl_device_type GetContextType() const;

};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_GRCLBASE_H */

