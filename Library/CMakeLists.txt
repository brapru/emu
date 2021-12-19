add_library (libemu SHARED 
    Gameboy.cpp 
    Cartridge.cpp 
    CPU.cpp 
    PPU.cpp
    Interface.cpp 
    MMU.cpp 
    Register.cpp 
    Opcodes.cpp 
    Timer.cpp 
    Serial.cpp
    Joypad.cpp 
    Instructions/Load.cpp 
    Instructions/Jump.cpp 
    Instructions/ArithmeticLogic.cpp 
    Instructions/Control.cpp 
    Instructions/PrefixCB.cpp
)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-s USE_SDL=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")

    set(SDL2_LIBRARY "-s USE_SDL=2 -s LLD_REPORT_UNDEFINED")
    set(EMSCRIPTEN_INCLUDE_DIR Tools/emsdk/upstream/emscripten/cache/sysroot/include/)

    include_directories(${EMSCRIPTEN_INCLUDE_DIR})
else()
    find_library(SDL2_LIBRARY SDL2)
    find_library(SDL2_MAIN SDL2main)

    find_package(fmt)
    target_link_libraries (libemu fmt::fmt)
endif()

target_include_directories (libemu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDE_DIR})
target_link_libraries (libemu ${SDL2_MAIN} ${SDL2_LIBRARY})
