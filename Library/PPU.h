#pragma once

#include <stdint.h>
#include <vector>

class PPU {
public:
    PPU() {};
    uint8_t vram_read(uint16_t const& address);
    void vram_write(uint16_t const address, uint8_t const value);

    uint8_t oam_read(uint16_t const& address);
    void oam_write(uint16_t const address, uint8_t const value);

private:
    std::vector<uint8_t> m_oam_ram = std::vector<uint8_t>(0xA0);
    std::vector<uint8_t> m_vram = std::vector<uint8_t>(0x2000);
    // uint8_t m_oam_ram[0xA0];
    // uint8_t m_vram[0x2000];
};