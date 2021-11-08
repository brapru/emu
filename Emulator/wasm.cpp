#include <Gameboy.h>
#include <Utils/File.h>
#include <Utils/Format.h>

#include <iostream>
#include <memory>
#include <string>

std::unique_ptr<Gameboy> vm;

extern "C" void load()
{
    std::vector<uint8_t> rom_data = read_rom_into_vector("rom.gb");
    vm.reset(new Gameboy(rom_data));
}

int main(int argc, char** argv)
{
    std::vector<uint8_t> empty;
    vm = std::make_unique<Gameboy>(empty);
    if (!vm) {
        outln("Failed to initialize the Gameboy.");
        exit(1);
    }

    vm->run();

    return 0;
}