#pragma once

#include <MMU.h>
#include <Register.h>
#include <Serial.h>

#include <stdint.h>

class MMU;
class Serial;

enum class RegisterOperation {
    Increment,
    Decrement
};

enum class Condition {
    NZ,
    Z,
    NC,
    C
};

class CPU {
public:
    CPU(MMU& mmu, Serial& m_serial);

    void cycle();

    uint8_t fetch_byte();
    uint16_t fetch_word();

    ByteRegister interrupt_flag(void) { return m_interrupt_flag; }
    ByteRegister interrupt_enable(void) { return m_interrupt_enable; }

    void stack_push(WordRegister& reg);
    void stack_push(WholeRegister& reg);
    uint16_t stack_pop();

private:
    MMU& m_mmu;
    Serial& m_serial;

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

    FlagRegister m_f;

    bool m_interrupt_master_enable;
    ByteRegister m_interrupt_flag;
    ByteRegister m_interrupt_enable;

    // Opcodes.cpp
    void execute_instruction(uint8_t opcode);

    // LD Instructions - Instructions/LD.cpp
    void instruction_ld(ByteRegister& reg);
    void instruction_ld(WordRegister& reg);
    void instruction_ld(WholeRegister& reg);
    void instruction_ld(WholeRegister& reg, RegisterOperation update);
    void instruction_ld(ByteRegister& reg, ByteRegister& from_reg);
    void instruction_ld(ByteRegister& reg, WholeRegister& from_reg);

    void instruction_ld_reg_to_addr(ByteRegister& reg);
    void instruction_ld_reg_to_addr(WordRegister& reg);
    void instruction_ld_reg_to_addr(WholeRegister& whole_reg, ByteRegister& reg);
    void instruction_ld_reg_to_addr(WholeRegister& whole_reg, ByteRegister& reg, RegisterOperation update);

    void instruction_ld_addr_to_reg(ByteRegister& reg);

    void instruction_ldh_a_to_memory();
    void instruction_ldh_memory_to_a();
    void instruction_ld_hl_sp();
    void instruction_ld_sp_hl();

    void instruction_push(WholeRegister& reg);
    void instruction_pop(WholeRegister& reg);

    // Arithmetic and Logic Instructions - Instructions/ArithmeticLogic.cpp
    void instruction_add(ByteRegister& reg);
    void instruction_add(ByteRegister& reg, ByteRegister& from_reg);
    void instruction_add(ByteRegister& reg, WholeRegister& from_reg);

    void instruction_add_hl(WholeRegister& reg);
    void instruction_add_hl(WordRegister& reg);

    void instruction_adc();
    void instruction_adc(ByteRegister& reg);
    void instruction_adc(WholeRegister& reg);

    void instruction_sub();
    void instruction_sub(ByteRegister& reg);
    void instruction_sub(WholeRegister& reg);

    void instruction_sbc();
    void instruction_sbc(ByteRegister& reg);
    void instruction_sbc(WholeRegister& reg);

    void instruction_and(void);
    void instruction_and(ByteRegister& reg);
    void instruction_and(WholeRegister& reg);

    void instruction_cp();
    void instruction_cp(ByteRegister& reg);
    void instruction_cp(WholeRegister& reg);

    void instruction_inc(ByteRegister& reg);
    void instruction_inc(WholeRegister& reg);
    void instruction_inc(WordRegister& reg);
    void instruction_inc_hl_address();

    void instruction_dec(ByteRegister& reg);
    void instruction_dec(WholeRegister& reg);
    void instruction_dec(WordRegister& reg);
    void instruction_dec_hl_address();

    void instruction_or(ByteRegister& reg);
    void instruction_or(WholeRegister& reg);

    void instruction_xor();
    void instruction_xor(ByteRegister& reg);
    void instruction_xor_hl_address();

    // Jump Instructions - Instructions/Jump.cpp
    void instruction_call(void);
    void instruction_conditional_call(uint8_t const& opcode);

    void instruction_ret(void);
    void instruction_ret(uint8_t const& opcode);

    void instruction_jp(void);
    void instruction_jp(WholeRegister& reg);
    void instruction_jp(Condition condition);

    void instruction_jr(void);
    void instruction_jr(uint8_t const& opcode);

    // CPU Control Instructions - Instructions/Control.cpp
    void instruction_nop(void);
    void instruction_di(void);
    void instruction_ei(void);

    // Prefix CB Instructions - Instructions/PrefixCB.cpp
    void instruction_cb();
    void instruction_rra();
    void instruction_rr(ByteRegister& byte);
    void instruction_rr(WholeRegister& byte);
    void instruction_srl(ByteRegister& byte);
    void instruction_srl(WholeRegister& byte);
    void instruction_swap(ByteRegister& reg);
    void instruction_swap(WholeRegister& reg);
};