#pragma once

#include <CPU.h>
#include <Cartridge.h>
#include <Interface.h>
#include <MMU.h>
#include <Serial.h>
#include <Timer.h>

#include <memory>

class Gameboy {
public:
    Gameboy(std::vector<uint8_t> rom_data);

    void run();

private:
    void main_cycle();

    std::unique_ptr<Interface> m_interface;

    Cartridge m_cartridge;
    MMU m_mmu;
    CPU m_cpu;
    Timer m_timer;
    Serial m_serial;
};