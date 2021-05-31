if(NOT PKG_CONFIG_FOUND)
    INCLUDE(FindPkgConfig)
endif()
PKG_CHECK_MODULES(PC_WAVEFORM waveform)

FIND_PATH(
    WAVEFORM_INCLUDE_DIRS
    NAMES waveform/api.h
    HINTS $ENV{WAVEFORM_DIR}/include
        ${PC_WAVEFORM_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    WAVEFORM_LIBRARIES
    NAMES gnuradio-waveform
    HINTS $ENV{WAVEFORM_DIR}/lib
        ${PC_WAVEFORM_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/waveformTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(WAVEFORM DEFAULT_MSG WAVEFORM_LIBRARIES WAVEFORM_INCLUDE_DIRS)
MARK_AS_ADVANCED(WAVEFORM_LIBRARIES WAVEFORM_INCLUDE_DIRS)
