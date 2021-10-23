#pragma once

#include <MMU.h>
#include <Register.h>

#include <stdint.h>

class MMU;

class CPU {
public:
    CPU(MMU& mmu);

    void cycle();

    uint8_t fetch_byte();
    uint16_t fetch_word();

    ByteRegister interrupt_flag(void) { return m_interrupt_flag; }
    ByteRegister interrupt_enable(void) { return m_interrupt_enable; }

    void stack_push(WordRegister& reg);
    void stack_push(WholeRegister& reg);
    uint16_t stack_pop();

private:
    void execute_instruction(uint8_t opcode);

    MMU& m_mmu;

    WordRegister m_pc = WordRegister(0x0100);
    WordRegister m_sp = WordRegister(0xFFFE);

    WholeRegister m_af;
    WholeRegister m_bc;
    WholeRegister m_de;
    WholeRegister m_hl;

    ByteRegister m_a = ByteRegister(0x01);
    ByteRegister m_b = ByteRegister(0x00);
    ByteRegister m_c = ByteRegister(0x13);
    ByteRegister m_d = ByteRegister(0x00);
    ByteRegister m_e = ByteRegister(0xD8);
    ByteRegister m_h = ByteRegister(0x01);
    ByteRegister m_l = ByteRegister(0x4D);

    // TODO: Implement Flag Registers
    FlagRegister m_f;

    bool m_interrupt_master_enable;
    ByteRegister m_interrupt_flag;
    ByteRegister m_interrupt_enable;

    // Instruction.cpp
    void instruction_nop(void);
    void instruction_ld(WordRegister& reg);
    void instruction_ld(WholeRegister& reg);
    void instruction_ld_inc(WholeRegister& reg);
    void instruction_ld(ByteRegister& reg);
    void instruction_ld(ByteRegister& reg, ByteRegister& from_reg);
    void instruction_ld(ByteRegister& reg, WholeRegister& from_reg);
    void instruction_ld_reg_to_addr(ByteRegister& reg);
    void instruction_ld_reg_to_addr(WholeRegister& whole_reg, ByteRegister& reg);
    void instruction_ld_addr_to_reg(ByteRegister& reg);
    void instruction_ldh_a_to_memory();
    void instruction_ldh_memory_to_a();
    void instruction_inc(ByteRegister& reg);
    void instruction_inc(WholeRegister& reg);
    void instruction_inc(WordRegister& reg);
    void instruction_inc_hl_address();
    void instruction_dec(ByteRegister& reg);
    void instruction_cp();
    void instruction_cp(ByteRegister& reg);
    void instruction_cp(WholeRegister& reg);
    void instruction_di(void);
    void instruction_call(void);
    void instruction_conditional_call(uint8_t const& opcode);
    void instruction_ret(void);
    void instruction_push(WholeRegister& reg);
    void instruction_pop(WholeRegister& reg);
    void instruction_jp(void);
    void instruction_jr(void);
    void instruction_jr(uint8_t const& opcode);
    void instruction_xor(void);
    void instruction_or(ByteRegister& reg);
    void instruction_or(WholeRegister& reg);
    void instruction_and(void);
    void instruction_and(ByteRegister& reg);
    void instruction_and(WholeRegister& reg);
};