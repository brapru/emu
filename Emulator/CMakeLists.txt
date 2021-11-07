if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_executable (emu wasm.cpp)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
else()
    add_executable (emu sdl.cpp)
endif()

target_link_libraries (emu LINK_PUBLIC libemu)