#pragma once

#include <CPU.h>
#include <Cartridge.h>
#include <Serial.h>
#include <Timer.h>

#include <vector>

class CPU;
class PPU;
class Serial;

class MMU {
public:
    MMU(Cartridge& cartridge, CPU& cpu, PPU& ppu, Timer& timer, Serial& serial);

    uint8_t read(uint16_t const address);
    void write(uint16_t const address, uint8_t const value);

    bool valid_cartridge() { return m_cartridge.is_valid_checksum(); }

private:
    uint8_t memory_read(uint16_t const address);
    void memory_write(uint16_t const address, uint8_t const value);

    void io_write(uint16_t const address, uint8_t const value);
    uint8_t io_read(uint16_t const address);

    void m_dma_transfer(uint8_t const value);

    Cartridge& m_cartridge;
    CPU& m_cpu;
    PPU& m_ppu;
    Timer& m_timer;
    Serial& m_serial;
    std::vector<uint8_t> m_memory = std::vector<uint8_t>(0x10000);
};