add_library (libemu SHARED Gameboy.cpp Cartridge.cpp CPU.cpp MMU.cpp Register.cpp Instruction.cpp)

find_package(fmt)
target_link_libraries (libemu fmt::fmt)

target_include_directories (libemu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
