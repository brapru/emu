include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(COPY Roms/cpu_instrs.gb DESTINATION Roms)

enable_testing()

add_executable (test-emu TestCartridge.cpp)
target_link_libraries (test-emu LINK_PUBLIC libemu gtest_main)

include(GoogleTest)
gtest_discover_tests(test-emu)
