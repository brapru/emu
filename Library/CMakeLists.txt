add_library (libemu SHARED Gameboy.cpp Cartridge.cpp CPU.cpp Interface.cpp MMU.cpp Register.cpp Opcodes.cpp Timer.cpp Serial.cpp Instructions/Load.cpp Instructions/Jump.cpp Instructions/ArithmeticLogic.cpp Instructions/Control.cpp Instructions/PrefixCB.cpp)

find_package(fmt)
target_link_libraries (libemu fmt::fmt)

find_library(SDL2_LIBRARY SDL2)
find_library(SDL2_MAIN SDL2main)

target_include_directories (libemu PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries (libemu ${SDL2_LIBRARY})
target_link_libraries(libemu ${SDL2_MAIN}) 

target_include_directories (libemu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})