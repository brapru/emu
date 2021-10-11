#pragma once

#include <Cartridge.h>

class MMU {
public:
    MMU(Cartridge& cartridge);

    uint8_t read(uint16_t const address);
    void write(uint16_t const address, uint8_t const value);

private:
    Cartridge& m_cartridge;
};