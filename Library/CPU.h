#pragma once

#include <MMU.h>
#include <Register.h>

#include <stdint.h>

class CPU {
public:
    CPU(MMU& mmu);

    void cycle();

    uint8_t fetch_byte();
    uint16_t fetch_word();

private:
    void execute_instruction(uint8_t opcode);

    MMU& m_mmu;

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
    void instruction_ld(WordRegister& reg);
    void instruction_ld(ByteRegister& reg);
    void instruction_ld(ByteRegister& reg, ByteRegister& from_reg);
    void instruction_ld_reg_to_addr(ByteRegister& reg);
    void instruction_di(void);
    void instruction_jp(void);
    void instruction_xor(void);
};