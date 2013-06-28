# Author: Steven Lamerton
# Copyright: (c) 2013 Steven Lamerton
# Licence: wxWindows licence

# Set up the exe
add_executable(webview WIN32 webview.cpp)
add_executable(subprocess WIN32 subprocess.cpp)

target_link_libraries(webview wvc ${wxWidgets_LIBRARIES} 
                                  ${GTK2_LIBRARIES}
                                  debug ${CEF_LIBRARY_DEBUG}
                                  optimized ${CEF_LIBRARY_RELEASE}
                                  debug ${CEF_WRAPPER_LIBRARY_DEBUG}
                                  optimized ${CEF_WRAPPER_LIBRARY_RELEASE})

target_link_libraries(subprocess wvc ${wxWidgets_LIBRARIES} 
                                     ${GTK2_LIBRARIES}
                                     debug ${CEF_LIBRARY_DEBUG}
                                     optimized ${CEF_LIBRARY_RELEASE}
                                     debug ${CEF_WRAPPER_LIBRARY_DEBUG}
                                     optimized ${CEF_WRAPPER_LIBRARY_RELEASE})

add_dependencies(webview subprocess)
