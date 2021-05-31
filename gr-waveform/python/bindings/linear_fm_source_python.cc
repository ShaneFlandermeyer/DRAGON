/*
 * Copyright 2021 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(linear_fm_source.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(b3c0ac8b75fba6685957e8eaaa9c2aa7)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <waveform/linear_fm_source.h>
// pydoc.h is automatically generated in the build directory
#include <linear_fm_source_pydoc.h>

void bind_linear_fm_source(py::module &m) {

  using linear_fm_source = gr::waveform::linear_fm_source;


  py::class_ < linear_fm_source, gr::sync_block, gr::block, gr::basic_block,
      std::shared_ptr
          < linear_fm_source >> (m, "linear_fm_source", D(linear_fm_source))

              .def(py::init(&linear_fm_source::make),
                  D(linear_fm_source, make)
              );

}







