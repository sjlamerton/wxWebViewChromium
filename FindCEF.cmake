# Author: Steven Lamerton
# Copyright: (c) 2013 Steven Lamerton
# Licence: wxWindows licence

# Find module for the Chromium Embedded Framework

include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

set(CEF_ROOT_DIR "" CACHE PATH "CEF root directory")

find_path(CEF_INCLUDE_DIR "include/cef_version.h"
          HINTS ${CEF_ROOT_DIR})

# On Windows find the dll_wrapper
if(WIN32)
find_library(CEF_WRAPPER_LIBRARY_RELEASE NAMES libcef_dll_wrapper
             HINTS "${CEF_ROOT_DIR}/lib"
             PATH_SUFFIXES "Release")
find_library(CEF_WRAPPER_LIBRARY_DEBUG NAMES libcef_dll_wrapper
             HINTS "${CEF_ROOT_DIR}/lib"
             PATH_SUFFIXES "Debug")
select_library_configurations(CEF_WRAPPER)
endif()

# Find the library itself
find_library(CEF_LIBRARY_RELEASE NAMES libcef
             HINTS "${CEF_ROOT_DIR}/lib"
             PATH_SUFFIXES "Release")
find_library(CEF_LIBRARY_DEBUG NAMES libcef
             HINTS "${CEF_ROOT_DIR}/lib"
             PATH_SUFFIXES "Debug")
select_library_configurations(CEF)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(CEF DEFAULT_MSG
                                  CEF_INCLUDE_DIR)

mark_as_advanced(CEF_INCLUDE_DIR)