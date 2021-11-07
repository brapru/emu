add_executable (emu main.cpp)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

target_link_libraries (emu LINK_PUBLIC libemu)