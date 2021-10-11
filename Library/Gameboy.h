#pragma once

#include <Cartridge.h>
#include <MMU.h>

#include <memory>

class Gameboy {
public:
    Gameboy() {};
    static std::unique_ptr<Gameboy> initialize(std::string rom_file);

    void run();

    Cartridge& cartridge() { return Cartridge::the(); }

private:
    void main_cycle();
};
