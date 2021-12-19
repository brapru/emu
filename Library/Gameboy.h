#pragma once

#include <CPU.h>
#include <Cartridge.h>
#include <Interface.h>
#include <Joypad.h>
#include <MMU.h>
#include <PPU.h>
#include <Serial.h>
#include <Timer.h>

#include <memory>

constexpr unsigned int CYCLES_PER_FRAME = 70224;

class Gameboy {
public:
    Gameboy(std::vector<uint8_t> rom_data);

    bool has_cartridge() { return m_cartridge.is_loaded(); }

    void main_cycle();
    void run();

private:
    unsigned int m_elapsed_cycles = 0;

    SDL_Event m_event;
    bool m_is_running = true;

    uint64_t m_tracked_frame = 0;

    std::unique_ptr<Interface> m_interface;
    Cartridge m_cartridge;
    MMU m_mmu;
    CPU m_cpu;
    PPU m_ppu;
    Timer m_timer;
    Serial m_serial;
    Joypad m_joypad;
};