cmake_minimum_required (VERSION 2.8.11)
project (emu)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory (Library)
add_subdirectory (Emulator)
add_subdirectory (Tests)
