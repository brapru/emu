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

void CPU::instruction_rra(void)
{
    instruction_rr(m_a);
    m_f.set_zero_flag(false);
}

void CPU::instruction_rl(ByteRegister& reg)
{
    bool orig_carry = m_f.flag_carry();

    bool flag_carry = checkbit(reg.value(), 7);
    m_f.set_flag_carry(flag_carry);

    auto result = reg.value() << 1;

    (orig_carry) ? bitset(result, 0) : bitclear(result, 0);

    reg.set(result);

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
}

void CPU::instruction_rla(void)
{
    instruction_rl(m_a);
    m_f.set_zero_flag(false);
}

void CPU::instruction_rlc(ByteRegister& reg)
{
    bool flag_carry = checkbit(reg.value(), 7);
    m_f.set_flag_carry(flag_carry);

    auto result = reg.value() << 1;

    (flag_carry) ? bitset(result, 0) : bitclear(result, 0);

    reg.set(result);

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
}

void CPU::instruction_rlca()
{
    instruction_rlc(m_a);
    m_f.set_zero_flag(false);
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

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    (flag_carry) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_swap(ByteRegister& reg)
{
    auto lo = ((reg.value() >> 0) & 0xF);
    auto hi = ((reg.value() >> 4) & 0xF);

    auto result = (hi << 8) | (lo & 0xF);

    reg.set(result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
}

void CPU::instruction_swap(WholeRegister& reg)
{
    auto address = reg.value();
    auto value = m_mmu.read(address);
    auto lo = ((value >> 0) & 0xF);
    auto hi = ((value >> 4) & 0xF);

    auto result = (hi << 8) | (lo & 0xF);

    m_mmu.write(address, value);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
}