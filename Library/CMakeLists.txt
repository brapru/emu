add_library (libemu SHARED Gameboy.cpp Cartridge.cpp CPU.cpp MMU.cpp Register.cpp Opcodes.cpp Timer.cpp Serial.cpp Instructions/Load.cpp Instructions/Jump.cpp Instructions/ArithmeticLogic.cpp Instructions/Control.cpp Instructions/RotateShift.cpp)

find_package(fmt)
target_link_libraries (libemu fmt::fmt)

target_include_directories (libemu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
