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

#include <iostream>
#include <dragon/GRCLBase.h>

#define DEBUG_MODE 1

namespace gr {
namespace dragon {

/*!
 * Initializes an OpenCL instance. Specifically, the following steps are taken:
 *  1. Query the list of available platforms
 *  2. Specify the device type  (GPU, CPU, Accelerator, Any)
 *  3. Choose whether to use the host pointer to access data (for CPUs) or
 *      copy the data to the OpenCL device (GPUs, Accelerators, etc.)
 *  4. If a specific device ID is specified, try to query that device. If
 *      not, choose the first platform that has the specified device type
 *  5. Create a context from this device.
 *  6. Create a command queue from the context.
 */
void GRCLBase::InitOpenCL(int data_type,
                          size_t data_size,
                          int device_type,
                          int device_selector,
                          int platform_id,
                          int device_idx,
                          bool set_debug,
                          bool out_of_order_queue) {

  this->data_type = data_type;
  this->data_size = data_size;
  this->device_type = device_type;
  this->device_selector = device_selector;
  this->platform_id = platform_id;
  this->device_idx = device_idx;
  debug_mode = set_debug;

  std::vector<cl::Platform> platformList;

  // Get the list of all platforms in the system
  try {
    cl::Platform::get(&platformList);
  }
  catch (...) {
    std::string errMsg = "OpenCL Error: Unable to get platform list.";
    throw cl::Error(CL_DEVICE_NOT_FOUND, (const char *) errMsg.c_str());
  }

  // Now we set up our OpenCL space
  try {
    // Set default to GPU
    std::string openclString = "GPU";
    cl_device_type clType = CL_DEVICE_TYPE_GPU;

    // See if we have to adjust anything.
    switch (device_type) {
      case OCLTYPE_CPU:
        clType = CL_DEVICE_TYPE_CPU;
        openclString = "CPU";
        break;
      case OCLTYPE_ACCELERATOR:
        clType = CL_DEVICE_TYPE_ACCELERATOR;
        openclString = "Accelerator";
        break;
      case OCLTYPE_ANY:
        try {
          // Any means any so we're really taking first available.

          // Test for GPU first...
          cl_context_properties cprops[] = {
              CL_CONTEXT_PLATFORM, (cl_context_properties) (platformList[0])(),
              0};
          cl::Context testContext(clType, cprops);

          clType = CL_DEVICE_TYPE_GPU;
          openclString = "Any/GPU Preferred";
        }
        catch (cl::Error &err) {
          try {
            // Will take any time.  So whatever's left.
            try {
              cl_context_properties cprops[] = {
                  CL_CONTEXT_PLATFORM,
                  (cl_context_properties) (platformList[0])(), 0};
              cl::Context testContext(CL_DEVICE_TYPE_CPU, cprops);

              clType = CL_DEVICE_TYPE_CPU;
              openclString = "Any/CPU";
            }
            catch (cl::Error &err) {
              cl_context_properties cprops[] = {
                  CL_CONTEXT_PLATFORM,
                  (cl_context_properties) (platformList[0])(), 0};
              cl::Context testContext(CL_DEVICE_TYPE_ALL, cprops);

              clType = CL_DEVICE_TYPE_ALL;
              openclString = "Any/Any";

            }
          }
          catch (cl::Error &err) {
            clType = CL_DEVICE_TYPE_ALL;
            openclString = "Any/ALL";
          }
        }
        break;
    }

    context_type = clType;

    // set buffer type for a zero-copy mode
    if (context_type == CL_DEVICE_TYPE_CPU) {
      optimal_buffer_type = CL_MEM_USE_HOST_PTR;
    } else {
      optimal_buffer_type = CL_MEM_ALLOC_HOST_PTR;
    }

    context = nullptr;

    // So here we have a list of platforms and if we've selected ANY we know if we have GPU or CPU in platform[0]

    // Given the type we want now we have to make some choices.
    // If we asked for a specific platform and device Id we set that up
    // else:
    // We first need to find a platform that has devices of the type we asked for
    // If we asked for the first device, we need to take just 1 device from its list of type-specific devices
    // If we asked for ALL then we leave the list as is.

    if (device_selector == OCLDEVICESELECTOR_SPECIFIC) {
      // Let's make sure that device number is present.

      if ((platform_id + 1) > platformList.size()) {
        std::string errMsg =
            "The requested platform Id " + std::to_string(platform_id)
                + " does not exist.";
        throw cl::Error(CL_DEVICE_NOT_FOUND, (const char *) errMsg.c_str());
      }

      cl_context_properties cprops[] = {CL_CONTEXT_PLATFORM,
                                        (cl_context_properties) (platformList[platform_id])(),
                                        0};
      try {
        context = new cl::Context(clType, cprops);
      }
      catch (...) {
        std::string errMsg =
            "The requested platform Id " + std::to_string(platform_id)
                + " does not have a device of the requested type ("
                + openclString + ")";
        throw cl::Error(CL_DEVICE_NOT_FOUND, (const char *) errMsg.c_str());
      }

      platform_name = platformList[platform_id].getInfo<CL_PLATFORM_NAME>();
      platform_vendor = platformList[platform_id].getInfo<CL_PLATFORM_VENDOR>();
    } else {
      // Find the first platform that has the requested device type.
      // Note this is the first platform, not the first device.  So if the system has 2 NVIDIA cards,
      // The platform would be NVIDIA.  The specific cards would be the devices under that platform.
      for (int i = 0; i < platformList.size(); i++) {
        try {
          try {
            cl_context_properties cprops[] = {
                CL_CONTEXT_PLATFORM,
                (cl_context_properties) (platformList[i])(), 0};
            context = new cl::Context(clType, cprops);
            // If we didn't throw an exception, we're good here
            platform_name = platformList[i].getInfo<CL_PLATFORM_NAME>();
            platform_vendor = platformList[i].getInfo<CL_PLATFORM_VENDOR>();
            break;
          }
          catch (cl::Error &err) {
            // doesn't have this type.  Continue
          }
        }
        catch (...) {
          context = nullptr;
        }
      }

    }

    if (context == nullptr) {
      std::string errMsg = "No OpenCL devices of type " + openclString;
      throw cl::Error(CL_DEVICE_NOT_FOUND, (const char *) errMsg.c_str());
    }

    // Use this link as a reference to get other platform info:
    // https://gist.github.com/dogukancagatay/8419284

    // Query the set of devices attached to the context
    try {
      devices = context->getInfo<CL_CONTEXT_DEVICES>();
    }
    catch (...) {
      std::string errMsg =
          "OpenCL Error: unable to enumerate " + platform_name + " devices.";
      throw cl::Error(CL_DEVICE_NOT_FOUND, (const char *) errMsg.c_str());
    }

    try {
      device_names.emplace_back(devices[device_idx].getInfo<CL_DEVICE_NAME>());
    }
    catch (...) {
      std::string errMsg = "Error getting device info for " + openclString;
      throw cl::Error(CL_DEVICE_NOT_FOUND, (const char *) errMsg.c_str());
    }

    try {
      switch (devices[device_idx].getInfo<CL_DEVICE_TYPE>()) {
        case CL_DEVICE_TYPE_GPU:
          device_types.emplace_back("GPU");
          break;
        case CL_DEVICE_TYPE_CPU:
          device_types.emplace_back("CPU");
          break;
        case CL_DEVICE_TYPE_ACCELERATOR:
          device_types.emplace_back("Accelerator");
          break;
        case CL_DEVICE_TYPE_ALL:
          device_types.emplace_back("ALL");
          break;
        case CL_DEVICE_TYPE_CUSTOM:
          device_types.emplace_back("CUSTOM");
          break;
      }
    }
    catch (...) {
      std::cout << "GRCLBase: Error getting CL_DEVICE_TYPE" << std::endl;
      exit(0);
    }

    this->data_size = 0;

    switch (data_type) {
      case DTYPE_FLOAT:
        this->data_size = sizeof(float);
        break;
      case DTYPE_INT:
        this->data_size = sizeof(int);
        break;
      case DTYPE_COMPLEX:
        this->data_size = sizeof(SComplex);
        break;
      default:
        this->data_size = data_size;
        break;
    }
  }
  catch (cl::Error &e) {
    std::cout << "OpenCL Error " + std::to_string(e.err()) + ": " << e.what()
              << std::endl;
    switch (device_type) {
      case OCLTYPE_CPU:
        std::cout << "Attempted Context Type: CPU" << std::endl;
        break;
      case OCLTYPE_ACCELERATOR:
        std::cout << "Attempted Context Type: Accelerator" << std::endl;
        break;
      case OCLTYPE_GPU:
        std::cout << "Attempted Context Type: GPU" << std::endl;
        break;
      case OCLTYPE_ANY:
        std::cout << "Attempted Context Type: ANY" << std::endl;
        break;
    }

    exit(0);
  }

  try {
    max_const_mem_size =
        devices[device_idx].getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>();
  }
  catch (cl::Error &e) {
    std::cout << "Error getting device constant memory size." << std::endl;
  }

  // This can be overridden in derived classes.
  // This calculation assumes only a single input stream.
  max_const_items = max_const_mem_size / this->data_size;



  // Create command queue
  try {
    // device_idx will either be 0 or if a specific platform and device id were
    // specified, that one.
    if (!out_of_order_queue)
      // standard queue used for all parallel data processing streams (SIMD)
      queue = new cl::CommandQueue(*context, devices[device_idx], 0);
    else
      // Used for task-parallel kernels where kernels can execute in any order
      queue = new cl::CommandQueue(*context,
                                   devices[device_idx],
                                   CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
  }
  catch (...) {
    std::string errMsg =
        "OpenCL Error: Unable to create OpenCL command queue on "
            + platform_name;
    throw cl::Error(CL_DEVICE_NOT_FOUND, (const char *) errMsg.c_str());
  }
}
GRCLBase::GRCLBase(int idata_type,
                   size_t data_size,
                   int openCLPlatformType,
                   int devSelector,
                   int platformId,
                   int devId,
                   bool setDebug,
                   bool outOfOrderQueue) {
  InitOpenCL(idata_type,
             data_size,
             openCLPlatformType,
             devSelector,
             platformId,
             devId,
             setDebug,
             outOfOrderQueue);
}

GRCLBase::GRCLBase(int idata_type,
                   size_t data_size,
                   int openCLPlatformType,
                   bool setDebug,
                   bool outOfOrderQueue) {
  InitOpenCL(idata_type,
             data_size,
             openCLPlatformType,
             OCLDEVICESELECTOR_FIRST,
             0,
             0,
             setDebug,
             outOfOrderQueue);
}

cl_device_type GRCLBase::GetContextType() const {
  return context_type;
}

bool GRCLBase::CompileKernel(const char *kernel_code,
                             const char *kernel_function_name,
                             bool exit_on_fail) {
  try {
    // Create and program from source
    if (program) {
      delete program;
      program = nullptr;
    }
    if (sources) {
      delete sources;
      sources = nullptr;
    }
    sources = new cl::Program::Sources(1, std::make_pair(kernel_code, 0));
    program = new cl::Program(*context, *sources);

    // Build program
    program->build(devices);

    kernel = new cl::Kernel(*program, (const char *) kernel_function_name);
  }
  catch (cl::Error &e) {
    std::cout << "OpenCL Error compiling kernel for " << kernel_function_name
              << std::endl;
    std::cout << "OpenCL Error " << e.err() << ": " << e.what() << std::endl;
    std::cout << kernel_code << std::endl;
    if (exit_on_fail)
      exit(0);
    else
      return false;
  }

  try {
    preferred_work_group_size_multiple =
        kernel->getWorkGroupInfo<CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE>(
            devices[0]);
  }
  catch (cl::Error &e) {
    std::cout << "Error getting kernel preferred work group size multiple"
              << std::endl;
    std::cout << "OpenCL Error " << e.err() << ": " << e.what() << std::endl;
  }

  try {
    max_work_group_size =
        kernel->getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(devices[0]);
  }
  catch (cl::Error &e) {
    std::cout << "Error getting kernel max work group size" << std::endl;
    std::cout << "OpenCL Error " << e.err() << ": " << e.what() << std::endl;
  }

  return true;
}

void GRCLBase::cleanup() {
  /* Cleanup order:
   * Memory
   * Command queue
   * kernel
   * program
   * context
   */

  try {
    if (queue != nullptr) {
      delete queue;
      queue = nullptr;
    }
  }
  catch (...) {
    queue = nullptr;
    std::cout << "queue delete error." << std::endl;
  }

  try {
    if (kernel != nullptr) {
      delete kernel;
      kernel = nullptr;
    }
  }
  catch (...) {
    kernel = nullptr;
    std::cout << "Kernel delete error." << std::endl;
  }

  try {
    if (program != nullptr) {
      delete program;
      program = nullptr;
    }
  }
  catch (...) {
    program = nullptr;
    std::cout << "program delete error." << std::endl;
  }

  try {
    if (context != nullptr) {
      delete context;
      context = nullptr;
    }
  }
  catch (...) {
    context = nullptr;
    std::cout << "program delete error." << std::endl;
  }
}

GRCLBase::~GRCLBase() = default;

bool GRCLBase::stop() {
  cleanup();

  return true;
}

} /* namespace dragon */
} /* namespace gr */

