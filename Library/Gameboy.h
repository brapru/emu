#pragma once

#include <CPU.h>
#include <Cartridge.h>
#include <Interface.h>
#include <MMU.h>
#include <PPU.h>
#include <Serial.h>
#include <Timer.h>

#include <memory>

class Gameboy {
public:
    Gameboy(std::vector<uint8_t> rom_data);

    bool has_cartridge() { return m_cartridge.is_loaded(); }

    void main_cycle();
    void run();

private:
    std::unique_ptr<Interface> m_interface;

    Cartridge m_cartridge;
    MMU m_mmu;
    CPU m_cpu;
    PPU m_ppu;
    Timer m_timer;
    Serial m_serial;
};