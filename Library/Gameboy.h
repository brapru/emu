#pragma once

#include <CPU.h>
#include <Cartridge.h>
#include <MMU.h>

#include <memory>

class Gameboy {
public:
    Gameboy(std::string rom_file);

    void run();

private:
    void main_cycle();

    Cartridge m_cartridge;
    MMU m_mmu;
    CPU m_cpu;
};