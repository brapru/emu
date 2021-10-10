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

    WordRegister m_pc = WordRegister(0x0100);
    WordRegister m_sp = WordRegister(0xFFFE);
    WordRegister m_af;
    WordRegister m_bc;
    WordRegister m_de;
    WordRegister m_hl;
    ByteRegister m_a;
    ByteRegister m_b;
    ByteRegister m_c;
    ByteRegister m_d;
    ByteRegister m_e;
    ByteRegister m_h;
    ByteRegister m_l;

    // TODO: Implement Flag Registers
    FlagRegister m_flags;

    bool m_interrupt_master_enable;

    // Instruction.cpp
    void instruction_nop(void);
    void instruction_ld_sp(void);
    void instruction_di(void);
    void instruction_jp(void);
    void instruction_xor(void);
};