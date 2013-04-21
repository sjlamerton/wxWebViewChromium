# Author: Steven Lamerton
# Copyright: (c) 2013 Steven Lamerton
# Licence: wxWindows licence

# Required to find the webview library
cmake_minimum_required(VERSION 2.8.8)

project(wxwebviewchromium)

# Set the module path so we can find FindCEF
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

# Find CEF
find_package(CEF)
include_directories(${CEF_INCLUDE_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# On Linux we require GTK
if(UNIX)
    # Find GTK
    find_package(GTK2 REQUIRED)
    include_directories(${GTK2_INCLUDE_DIRS})
endif()

# Find wxWidgets
find_package(wxWidgets COMPONENTS webview adv  xml stc scintilla core base REQUIRED)
include(${wxWidgets_USE_FILE})

# Addwxwebviewchromium to a simple library so we can use it in all samples
add_library(wvc STATIC webview_chromium.cpp webview_chromium.h)

add_subdirectory(samples)
