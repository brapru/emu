add_library (emulator SHARED Gameboy.cpp Cartridge.cpp CPU.cpp MMU.cpp Register.cpp Instruction.cpp)

find_package(fmt)
target_link_libraries (emulator fmt::fmt)

target_include_directories (emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
