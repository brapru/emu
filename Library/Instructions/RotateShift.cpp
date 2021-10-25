#include <CPU.h>
#include <Utils/Bitwise.h>
#include <Utils/Format.h>

void CPU::instruction_rr(ByteRegister& reg)
{
    bool orig_carry = m_f.flag_carry();

    bool flag_carry = checkbit(reg.value(), 0);
    m_f.set_flag_carry(flag_carry);

    auto result = reg.value() >> 1;

    (orig_carry) ? bitset(result, 7) : bitclear(result, 7);

    reg.set(result);

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
}

void CPU::instruction_rr(WholeRegister& reg)
{
    auto value = m_mmu.read(reg.value());

    bool orig_carry = m_f.flag_carry();

    bool flag_carry = checkbit(value, 0);
    m_f.set_flag_carry(flag_carry);

    auto result = value >> 1;

    (orig_carry) ? bitset(result, 7) : bitclear(result, 7);

    reg.set(result);

    (value == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
}

void CPU::instruction_srl(ByteRegister& reg)
{
    auto result = reg.value() >> 1;

    bool flag_carry = checkbit(reg.value(), 0);

    reg.set(result);

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    (flag_carry) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_srl(WholeRegister& reg)
{
    auto value = m_mmu.read(reg.value());
    auto result = value >> 1;

    bool flag_carry = checkbit(value, 0);

    reg.set(result);

    (value == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    (flag_carry) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}