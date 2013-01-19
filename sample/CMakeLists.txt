# Author: Steven Lamerton
# Copyright: (c) 2013 Steven Lamerton
# Licence: wxWindows licence

# Set up the exe
add_executable(simple WIN32 simple.cpp simple.h)

target_link_libraries(simple wvc ${wxWidgets_LIBRARIES} 
                                 ${GTK2_LIBRARIES}
                                 debug ${CEF_LIBRARY_DEBUG}
                                 optimized ${CEF_LIBRARY_RELEASE}
                                 debug ${CEF_WRAPPER_LIBRARY_DEBUG}
                                 optimized ${CEF_WRAPPER_LIBRARY_RELEASE})

