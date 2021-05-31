# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT waveform_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT waveform_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND waveform_python_files
    linear_fm_source_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(waveform 
   ../..
   gr::waveform
   "${waveform_python_files}")

install(TARGETS waveform_python DESTINATION ${GR_PYTHON_DIR}/waveform COMPONENT pythonapi)
