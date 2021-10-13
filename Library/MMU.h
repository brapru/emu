#pragma once

#include <Cartridge.h>

#include <vector>

class MMU {
public:
    MMU(Cartridge& cartridge);

    uint8_t read(uint16_t const address);
    void write(uint16_t const address, uint8_t const value);

    bool valid_cartridge() { return m_cartridge.is_valid_checksum(); }

private:
    uint8_t memory_read(uint16_t const address);
    void memory_write(uint16_t const address, uint8_t const value);

    Cartridge& m_cartridge;
    std::vector<uint8_t> m_memory = std::vector<uint8_t>(0x10000);
};