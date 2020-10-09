INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DRAGON dragon)

FIND_PATH(
    DRAGON_INCLUDE_DIRS
    NAMES dragon/api.h
    HINTS $ENV{DRAGON_DIR}/include
        ${PC_DRAGON_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DRAGON_LIBRARIES
    NAMES gnuradio-dragon
    HINTS $ENV{DRAGON_DIR}/lib
        ${PC_DRAGON_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/dragonTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DRAGON DEFAULT_MSG DRAGON_LIBRARIES DRAGON_INCLUDE_DIRS)
MARK_AS_ADVANCED(DRAGON_LIBRARIES DRAGON_INCLUDE_DIRS)
