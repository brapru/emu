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

namespace RST {
constexpr uint16_t RST0 = 0x00;
constexpr uint16_t RST1 = 0x08;
constexpr uint16_t RST2 = 0x10;
constexpr uint16_t RST3 = 0x18;
constexpr uint16_t RST4 = 0x20;
constexpr uint16_t RST5 = 0x28;
constexpr uint16_t RST6 = 0x30;
constexpr uint16_t RST7 = 0x38;
}

namespace Interrupts {
constexpr uint16_t VBLANK = 0x40;
constexpr uint16_t LCD_STATUS = 0x48;
constexpr uint16_t TIMER = 0x50;
constexpr uint16_t SERIAL = 0x58;
constexpr uint16_t JOYPAD = 0x60;
}

class CPU {
public:
    CPU(MMU& mmu, Serial& m_serial);

    unsigned int cycle();

    uint8_t fetch_byte();
    uint16_t fetch_word();

    ByteRegister interrupt_flag(void) { return m_interrupt_flag; }
    ByteRegister interrupt_enable(void) { return m_interrupt_enable; }

    void request_interrupt(uint16_t const& interrupt) { m_interrupt_flag.set(interrupt); }
    void handle_interrupts();
    uint8_t has_interrupts() { return m_interrupt_flag.value() & m_interrupt_enable.value(); }

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

    bool m_is_halted = false;
    bool m_interrupt_master_enable = false;

    ByteRegister m_interrupt_flag;
    ByteRegister m_interrupt_enable;

    // Opcodes.cpp
    unsigned long m_execute_instruction(uint8_t opcode);

    // LD Instructions - Instructions/LD.cpp
    unsigned long instruction_ld(ByteRegister& reg);
    unsigned long instruction_ld(WordRegister& reg);
    unsigned long instruction_ld(WholeRegister& reg);
    unsigned long instruction_ld(WholeRegister& reg, RegisterOperation update);
    unsigned long instruction_ld(ByteRegister& reg, ByteRegister& from_reg);
    unsigned long instruction_ld(ByteRegister& reg, WholeRegister& from_reg);

    unsigned long instruction_ld_reg_to_addr(ByteRegister& reg);
    unsigned long instruction_ld_reg_to_addr(WholeRegister& reg);
    unsigned long instruction_ld_reg_to_addr(WordRegister& reg);
    unsigned long instruction_ld_reg_to_addr(WholeRegister& whole_reg, ByteRegister& reg);
    unsigned long instruction_ld_reg_to_addr(WholeRegister& whole_reg, ByteRegister& reg, RegisterOperation update);

    unsigned long instruction_ld_addr_to_reg(ByteRegister& reg);

    unsigned long instruction_ld_hl();
    unsigned long instruction_ldh_a_to_memory();
    unsigned long instruction_ldh_memory_to_a();
    unsigned long instruction_ld_hl_sp();
    unsigned long instruction_ld_sp_hl();

    unsigned long instruction_ldh_c_to_a();
    unsigned long instruction_ldh_a_to_c();

    unsigned long instruction_push(WholeRegister& reg);
    unsigned long instruction_pop(WholeRegister& reg);

    // Arithmetic and Logic Instructions - Instructions/ArithmeticLogic.cpp
    void instruction_add(ByteRegister& reg);
    void instruction_add(ByteRegister& reg, ByteRegister& from_reg);
    void instruction_add(ByteRegister& reg, WholeRegister& from_reg);

    void instruction_add_hl(WholeRegister& reg);
    void instruction_add_hl(WordRegister& reg);

    void instruction_add_sp();

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

    void instruction_or();
    void instruction_or(ByteRegister& reg);
    void instruction_or(WholeRegister& reg);

    void instruction_xor();
    void instruction_xor(ByteRegister& reg);
    void instruction_xor_hl_address();

    // Jump Instructions - Instructions/Jump.cpp
    void instruction_call(void);
    void instruction_conditional_call(Condition condition);

    void instruction_ret(void);
    void instruction_ret(Condition condition);
    void instruction_reti(void);

    void instruction_rst(uint16_t offset);

    void instruction_jp(void);
    void instruction_jp(WholeRegister& reg);
    void instruction_jp(Condition condition);

    void instruction_jr(void);
    void instruction_jr(Condition condition);

    // CPU Control Instructions - Instructions/Control.cpp
    unsigned long instruction_nop(void);
    unsigned long instruction_di(void);
    unsigned long instruction_ei(void);
    unsigned long instruction_cpl(void);
    unsigned long instruction_scf(void);
    unsigned long instruction_ccf(void);

    // Prefix CB Instructions - Instructions/PrefixCB.cpp
    unsigned long instruction_cb();

    unsigned long instruction_rr(ByteRegister& byte);
    unsigned long instruction_rr(WholeRegister& byte);
    unsigned long instruction_rra();
    unsigned long instruction_rrc(ByteRegister& reg);
    unsigned long instruction_rrc(WholeRegister& reg);
    unsigned long instruction_rrca();

    unsigned long instruction_rl(ByteRegister& reg);
    unsigned long instruction_rl(WholeRegister& reg);
    unsigned long instruction_rla();
    unsigned long instruction_rlc(ByteRegister& reg);
    unsigned long instruction_rlc(WholeRegister& reg);
    unsigned long instruction_rlca();

    unsigned long instruction_sra(ByteRegister& reg);
    unsigned long instruction_sra(WholeRegister& reg);

    unsigned long instruction_srl(ByteRegister& reg);
    unsigned long instruction_srl(WholeRegister& reg);

    unsigned long instruction_sla(ByteRegister& reg);
    unsigned long instruction_sla(WholeRegister& reg);

    unsigned long instruction_swap(ByteRegister& reg);
    unsigned long instruction_swap(WholeRegister& reg);
};