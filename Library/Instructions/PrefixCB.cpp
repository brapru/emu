#include <CPU.h>
#include <Utils/Bitwise.h>
#include <Utils/Format.h>

unsigned long CPU::instruction_rr(ByteRegister& reg)
{
    bool orig_carry = m_f.flag_carry();

    bool flag_carry = checkbit(reg.value(), 0);
    m_f.set_flag_carry(flag_carry);

    uint8_t result = reg.value() >> 1;

    (orig_carry) ? bitset(result, 7) : bitclear(result, 7);

    reg.set(result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);

    return 8;
}

unsigned long CPU::instruction_rr(WholeRegister& reg)
{
    uint8_t value = m_mmu.read(reg.value());

    bool orig_carry = m_f.flag_carry();

    bool flag_carry = checkbit(value, 0);
    m_f.set_flag_carry(flag_carry);

    uint8_t result = value >> 1;

    (orig_carry) ? bitset(result, 7) : bitclear(result, 7);

    m_mmu.write(reg.value(), result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);

    return 16;
}

unsigned long CPU::instruction_rra(void)
{
    instruction_rr(m_a);
    m_f.set_zero_flag(false);

    return 4;
}

unsigned long CPU::instruction_rrc(ByteRegister& reg)
{
    bool flag_carry = checkbit(reg.value(), 0);
    m_f.set_flag_carry(flag_carry);

    uint8_t result = reg.value() >> 1;

    (flag_carry) ? bitset(result, 7) : bitclear(result, 7);

    reg.set(result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);

    return 8;
}

unsigned long CPU::instruction_rrc(WholeRegister& reg)
{
    uint8_t value = m_mmu.read(reg.value());

    bool flag_carry = checkbit(value, 0);
    m_f.set_flag_carry(flag_carry);

    uint8_t result = value >> 1;

    (flag_carry) ? bitset(result, 7) : bitclear(result, 7);

    m_mmu.write(reg.value(), result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);

    return 16;
}

unsigned long CPU::instruction_rrca()
{
    instruction_rrc(m_a);
    m_f.set_zero_flag(false);

    return 4;
}

unsigned long CPU::instruction_rl(ByteRegister& reg)
{
    bool orig_carry = m_f.flag_carry();

    bool flag_carry = checkbit(reg.value(), 7);
    m_f.set_flag_carry(flag_carry);

    uint8_t result = reg.value() << 1;

    (orig_carry) ? bitset(result, 0) : bitclear(result, 0);

    reg.set(result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);

    return 8;
}

unsigned long CPU::instruction_rl(WholeRegister& reg)
{
    uint8_t value = m_mmu.read(reg.value());

    bool orig_carry = m_f.flag_carry();

    bool flag_carry = checkbit(value, 7);
    m_f.set_flag_carry(flag_carry);

    uint8_t result = value << 1;

    (orig_carry) ? bitset(result, 0) : bitclear(result, 0);

    m_mmu.write(reg.value(), result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);

    return 16;
}

unsigned long CPU::instruction_rla(void)
{
    instruction_rl(m_a);
    m_f.set_zero_flag(false);

    return 4;
}

unsigned long CPU::instruction_rlc(ByteRegister& reg)
{
    bool flag_carry = checkbit(reg.value(), 7);
    m_f.set_flag_carry(flag_carry);

    uint8_t result = reg.value() << 1;

    (flag_carry) ? bitset(result, 0) : bitclear(result, 0);

    reg.set(result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);

    return 8;
}

unsigned long CPU::instruction_rlc(WholeRegister& reg)
{
    uint8_t value = m_mmu.read(reg.value());

    bool flag_carry = checkbit(value, 7);
    m_f.set_flag_carry(flag_carry);

    uint8_t result = value << 1;

    (flag_carry) ? bitset(result, 0) : bitclear(result, 0);

    m_mmu.write(reg.value(), result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);

    return 16;
}

unsigned long CPU::instruction_rlca()
{
    instruction_rlc(m_a);
    m_f.set_zero_flag(false);

    return 4;
}

unsigned long CPU::instruction_sra(ByteRegister& reg)
{
    bool flag_carry = checkbit(reg.value(), 0);

    uint8_t result = (reg.value() >> 1) | (reg.value() & 0x80);

    reg.set(result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    (flag_carry) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_sra(WholeRegister& reg)
{
    uint8_t value = m_mmu.read(reg.value());

    bool flag_carry = checkbit(value, 0);

    uint8_t result = (value >> 1) | (value & 0x80);

    m_mmu.write(reg.value(), result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    (flag_carry) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 16;
}

unsigned long CPU::instruction_srl(ByteRegister& reg)
{
    bool flag_carry = checkbit(reg.value(), 0);

    uint8_t result = reg.value() >> 1;
    bitclear(result, 7);

    reg.set(result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    (flag_carry) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_srl(WholeRegister& reg)
{
    uint8_t value = m_mmu.read(reg.value());

    bool flag_carry = checkbit(value, 0);

    uint8_t result = value >> 1;
    bitclear(result, 7);

    m_mmu.write(reg.value(), result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    (flag_carry) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 16;
}

unsigned long CPU::instruction_sla(ByteRegister& reg)
{
    bool flag_carry = checkbit(reg.value(), 7);

    uint8_t result = static_cast<uint8_t>(reg.value() << 1);

    reg.set(result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    (flag_carry) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_sla(WholeRegister& reg)
{
    uint8_t value = m_mmu.read(reg.value());

    bool flag_carry = checkbit(reg.value(), 7);

    uint8_t result = static_cast<uint8_t>(value << 1);

    m_mmu.write(reg.value(), result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    (flag_carry) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 16;
}

unsigned long CPU::instruction_swap(ByteRegister& reg)
{
    auto lo = ((reg.value() >> 0) & 0xF);
    auto hi = ((reg.value() >> 4) & 0xF);

    auto result = (lo << 4) | (hi & 0xF);

    reg.set(result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_swap(WholeRegister& reg)
{
    auto address = reg.value();
    auto value = m_mmu.read(address);
    auto lo = ((value >> 0) & 0xF);
    auto hi = ((value >> 4) & 0xF);

    auto result = (lo << 4) | (hi & 0xF);

    m_mmu.write(address, result);

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);

    return 16;
}