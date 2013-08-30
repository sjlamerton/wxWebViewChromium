# Author: Steven Lamerton
# Copyright: (c) 2013 Steven Lamerton
# Licence: wxWindows licence

# Find module for the Chromium Embedded Framework

include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

set(CEF_ROOT_DIR "" CACHE PATH "CEF root directory")

find_path(CEF_INCLUDE_DIR "include/cef_version.h"
          HINTS ${CEF_ROOT_DIR})

# Find the dll_wrapper
find_library(CEF_WRAPPER_LIBRARY_RELEASE NAMES libcef_dll_wrapper
             HINTS "${CEF_ROOT_DIR}" "${CEF_ROOT_DIR}/lib" "${CEF_ROOT_DIR}/out"
             PATH_SUFFIXES "Release" "Release/lib")
find_library(CEF_WRAPPER_LIBRARY_DEBUG NAMES libcef_dll_wrapper
             HINTS "${CEF_ROOT_DIR}" "${CEF_ROOT_DIR}/lib" "${CEF_ROOT_DIR}/out"
             PATH_SUFFIXES "Debug" "Debug/lib")
select_library_configurations(CEF_WRAPPER)

# Find the library itself
find_library(CEF_LIBRARY_RELEASE NAMES libcef
             HINTS "${CEF_ROOT_DIR}" "${CEF_ROOT_DIR}/lib"
             PATH_SUFFIXES "Release")
find_library(CEF_LIBRARY_DEBUG NAMES libcef
             HINTS "${CEF_ROOT_DIR}" "${CEF_ROOT_DIR}/lib"
             PATH_SUFFIXES "Debug")
select_library_configurations(CEF)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(CEF DEFAULT_MSG
                                  CEF_INCLUDE_DIR)

mark_as_advanced(CEF_INCLUDE_DIR)

