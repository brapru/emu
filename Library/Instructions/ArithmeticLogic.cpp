#include <CPU.h>

// Arithmetic Instructions
void CPU::instruction_add(ByteRegister& reg)
{
    auto add = fetch_byte();
    auto orig = reg.value();
    auto result = add + orig;

    reg.set(static_cast<uint8_t>(result));

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((add & 0xF) + (orig & 0xF) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x100) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_add(ByteRegister& reg, ByteRegister& from_reg)
{
    auto add = from_reg.value();
    auto orig = reg.value();
    auto result = add + orig;

    reg.set(static_cast<uint8_t>(result));

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((add & 0xF) + (orig & 0xF) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x100) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_add(ByteRegister& reg, WholeRegister& from_reg)
{
    auto add = m_mmu.read(from_reg.value());
    auto orig = reg.value();
    auto result = add + orig;

    reg.set(static_cast<uint8_t>(result));

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((add & 0xF) + (orig & 0xF) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x100) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_add_hl(WholeRegister& reg)
{
    auto value = reg.value();
    auto orig = m_hl.value();
    auto result = orig + value;

    reg.set(result);

    m_f.set_subtraction_flag(false);
    ((orig & 0xFFFF) + (value & 0xFFFF) > 0xFFFF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x10000) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_add_hl(WordRegister& reg)
{
    auto value = reg.value();
    auto orig = m_hl.value();
    auto result = orig + value;

    reg.set(result);

    m_f.set_subtraction_flag(false);
    ((orig & 0xFFFF) + (value & 0xFFFF) > 0xFFFF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x10000) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_adc()
{
    auto add = fetch_byte();
    auto orig = m_a.value();
    auto carry = m_f.zero_flag();
    auto result = add + orig + carry;

    m_a.set(static_cast<uint8_t>(result));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    (((add & 0xF) + (orig & 0xF) + carry) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (result > 0xFF) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_adc(WholeRegister& reg)
{
    auto add = m_mmu.read(reg.value());
    auto orig = m_a.value();
    auto carry = m_f.zero_flag();
    auto result = add + orig + carry;

    m_a.set(static_cast<uint8_t>(result));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    (((add & 0xF) + (orig & 0xF) + carry) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (result > 0xFF) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_adc(ByteRegister& reg)
{
    auto add = reg.value();
    auto orig = m_a.value();
    auto carry = m_f.zero_flag();
    auto result = add + orig + carry;

    m_a.set(static_cast<uint8_t>(result));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    (((add & 0xF) + (orig & 0xF) + carry) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (result > 0xFF) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_sub()
{
    auto sub = fetch_byte();
    auto orig = m_a.value();

    m_a.set(static_cast<uint8_t>(orig - sub));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_sub(ByteRegister& reg)
{
    auto sub = reg.value();
    auto orig = m_a.value();

    m_a.set(static_cast<uint8_t>(orig - sub));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_sub(WholeRegister& reg)
{
    auto sub = m_mmu.read(reg.value());
    auto orig = m_a.value();

    m_a.set(static_cast<uint8_t>(orig - sub));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_sbc()
{
    auto sub = fetch_byte();
    auto orig = m_a.value();
    auto carry = m_f.zero_flag();
    auto result = sub - orig - carry;

    m_a.set(static_cast<uint8_t>(result));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((sub & 0xF) - (orig & 0xF) - carry) < 0) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (result < 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_sbc(WholeRegister& reg)
{
    auto sub = m_mmu.read(reg.value());
    auto orig = m_a.value();
    auto carry = m_f.zero_flag();
    auto result = sub - orig - carry;

    m_a.set(static_cast<uint8_t>(result));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((sub & 0xF) - (orig & 0xF) - carry) < 0) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (result < 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_sbc(ByteRegister& reg)
{
    auto sub = reg.value();
    auto orig = m_a.value();
    auto carry = m_f.zero_flag();
    auto result = sub - orig - carry;

    m_a.set(static_cast<uint8_t>(result));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((sub & 0xF) - (orig & 0xF) - carry) < 0) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (result < 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

// Logic Instructions
void CPU::instruction_and(void)
{
    auto value = fetch_byte();
    auto result = value & m_a.value();

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(true);
    m_f.set_flag_carry(false);
}

void CPU::instruction_and(ByteRegister& reg)
{
    auto result = reg.value() & m_a.value();

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(true);
    m_f.set_flag_carry(false);
}

void CPU::instruction_and(WholeRegister& reg)
{
    auto value = m_mmu.read(reg.value());
    auto and_value = value | m_a.value();

    m_a.set(and_value);

    (and_value == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(true);
    m_f.set_flag_carry(false);
}

void CPU::instruction_cp()
{
    auto sub = fetch_byte();
    auto orig = m_a.value();

    auto result = static_cast<int8_t>(orig - sub);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_cp(ByteRegister& reg)
{
    auto sub = reg.value();
    auto orig = m_a.value();

    auto result = static_cast<int8_t>(orig - sub);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_cp(WholeRegister& reg)
{
    auto sub = m_mmu.read(reg.value());
    auto orig = m_a.value();

    auto result = static_cast<int8_t>(orig - sub);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_inc(ByteRegister& reg)
{
    reg.increment();

    (reg.value() == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((reg.value() & 0x0F) == 0x00) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
}

void CPU::instruction_inc_hl_address()
{
    auto value = m_mmu.read(m_hl.value());
    auto result = static_cast<uint8_t>(value + 1);

    m_mmu.write(m_hl.value(), result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((result & 0x0F) == 0x00) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
}

void CPU::instruction_inc(WholeRegister& reg)
{
    reg.increment();
}

void CPU::instruction_inc(WordRegister& reg)
{
    reg.increment();
}

void CPU::instruction_dec(ByteRegister& reg)
{
    reg.decrement();

    (reg.value() == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    ((reg.value() & 0x0F) == 0x0F) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
}

void CPU::instruction_dec_hl_address()
{
    auto value = m_mmu.read(m_hl.value());
    auto result = static_cast<uint8_t>(value - 1);

    m_mmu.write(m_hl.value(), result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    ((result & 0x0F) == 0x0F) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
}

void CPU::instruction_dec(WholeRegister& reg)
{
    reg.decrement();
}

void CPU::instruction_dec(WordRegister& reg)
{
    reg.decrement();
}

void CPU::instruction_or(ByteRegister& reg)
{
    auto value = reg.value() | m_a.value();

    m_a.set(value);

    (value == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);
}

void CPU::instruction_or(WholeRegister& reg)
{
    auto value = m_mmu.read(reg.value());
    auto or_value = value | m_a.value();

    m_a.set(or_value);

    (or_value == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);
}

void CPU::instruction_xor()
{
    auto value = fetch_byte();
    auto result = m_a.value() ^ value;

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);
}

void CPU::instruction_xor(ByteRegister& reg)
{
    auto result = m_a.value() ^ reg.value();

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);
}

void CPU::instruction_xor_hl_address()
{
    auto value = m_mmu.read(m_hl.value());
    auto result = m_a.value() ^ value;

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);
}