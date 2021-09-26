#pragma once

class MMU {
public:
    MMU() {};

    static uint8_t read(uint16_t const address);
    static void write(uint16_t const address, uint8_t const value);
};