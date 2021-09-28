#pragma once

#include <Register.h>
#include <stdint.h>

class CPU {
public:
    static CPU& the();

    void cycle();

    uint8_t fetch_byte();
    uint16_t fetch_word();

private:
    CPU() { }
    CPU(CPU const&);
    void operator=(CPU const&);

    Register<uint16_t> m_pc = Register<uint16_t>(0x0100);
    Register<uint16_t> m_sp = Register<uint16_t>(0xFFFE);
    Register<uint16_t> m_af;
    Register<uint16_t> m_bc;
    Register<uint16_t> m_de;
    Register<uint16_t> m_hl;
    Register<uint8_t> m_a;
    Register<uint8_t> m_b;
    Register<uint8_t> m_c;
    Register<uint8_t> m_d;
    Register<uint8_t> m_e;
    Register<uint8_t> m_h;
    Register<uint8_t> m_l;

    // TODO: Implement Flag Registers
};