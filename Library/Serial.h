#pragma once

#include <MMU.h>
#include <Register.h>

#include <vector>

class MMU;

class Serial {
public:
    Serial(MMU& m_mmu);

    uint8_t read_data() { return m_sb.value(); }
    uint8_t read_control() { return m_sc.value(); }

    void write_data(uint8_t value) { m_sb.set(value); }
    void write_control(uint8_t value) { m_sc.set(value); }

    void update();
    void print();

private:
    MMU& m_mmu;

    ByteRegister m_sb;
    ByteRegister m_sc;

    std::vector<char> m_data;
};