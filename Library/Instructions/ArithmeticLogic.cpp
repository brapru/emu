#include <CPU.h>

// Arithmetic Instructions
unsigned long CPU::instruction_add(ByteRegister& reg)
{
    auto add = fetch_byte();
    auto orig = reg.value();
    auto result = add + orig;

    reg.set(static_cast<uint8_t>(result));

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((add & 0xF) + (orig & 0xF) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x100) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_add(ByteRegister& reg, ByteRegister& from_reg)
{
    auto add = from_reg.value();
    auto orig = reg.value();
    auto result = add + orig;

    reg.set(static_cast<uint8_t>(result));

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((add & 0xF) + (orig & 0xF) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x100) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 4;
}

unsigned long CPU::instruction_add(ByteRegister& reg, WholeRegister& from_reg)
{
    auto add = m_mmu.read(from_reg.value());
    auto orig = reg.value();
    auto result = add + orig;

    reg.set(static_cast<uint8_t>(result));

    (reg.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((add & 0xF) + (orig & 0xF) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x100) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_add_hl(WholeRegister& reg)
{
    auto value = reg.value();
    auto orig = m_hl.value();
    auto result = orig + value;

    reg.set(result);

    m_f.set_subtraction_flag(false);
    ((orig & 0xFFFF) + (value & 0xFFFF) > 0xFFFF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x10000) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_add_hl(WordRegister& reg)
{
    auto value = reg.value();
    auto orig = m_hl.value();
    auto result = orig + value;

    reg.set(result);

    m_f.set_subtraction_flag(false);
    ((orig & 0xFFFF) + (value & 0xFFFF) > 0xFFFF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((result & 0x10000) != 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_add_sp()
{
    auto add = static_cast<int8_t>(fetch_byte());
    auto orig = m_sp.value();
    auto result = add + orig;

    m_sp.set(static_cast<int8_t>(result));

    m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    (((add ^ orig ^ (result & 0XFFFF)) & 0x10) == 0x10) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((add ^ orig ^ (result & 0XFFFF)) & 0x100) == 0x100) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 16;
}

unsigned long CPU::instruction_adc()
{
    uint8_t add = fetch_byte();
    uint8_t orig = m_a.value();
    uint8_t carry = m_f.flag_carry() ? 0x01 : 0x00;
    uint8_t result = add + orig + carry;

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((static_cast<int>(add & 0xF) + static_cast<int>(orig & 0xF) + static_cast<int>(carry)) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((static_cast<int>(add & 0xF) + static_cast<int>(orig & 0xF) + static_cast<int>(carry)) > 0xF) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    m_a.set(result);

    return 8;
}

unsigned long CPU::instruction_adc(WholeRegister& reg)
{
    uint8_t add = m_mmu.read(reg.value());
    uint8_t orig = m_a.value();
    uint8_t carry = m_f.flag_carry() ? 0x01 : 0x00;
    uint8_t result = add + orig + carry;

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((static_cast<int>(add & 0xF) + static_cast<int>(orig & 0xF) + static_cast<int>(carry)) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((static_cast<int>(add & 0xF) + static_cast<int>(orig & 0xF) + static_cast<int>(carry)) > 0xF) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    m_a.set(result);

    return 8;
}

unsigned long CPU::instruction_adc(ByteRegister& reg)
{
    uint8_t add = reg.value();
    uint8_t orig = m_a.value();
    uint8_t carry = m_f.flag_carry() ? 0x01 : 0x00;
    uint8_t result = add + orig + carry;

    (result == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((static_cast<int>(add & 0xF) + static_cast<int>(orig & 0xF) + static_cast<int>(carry)) > 0xF) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    ((static_cast<int>(add & 0xF) + static_cast<int>(orig & 0xF) + static_cast<int>(carry)) > 0xF) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    m_a.set(result);

    return 4;
}

unsigned long CPU::instruction_sub()
{
    auto sub = fetch_byte();
    auto orig = m_a.value();

    m_a.set(static_cast<uint8_t>(orig - sub));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_sub(ByteRegister& reg)
{
    auto sub = reg.value();
    auto orig = m_a.value();

    m_a.set(static_cast<uint8_t>(orig - sub));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 4;
}

unsigned long CPU::instruction_sub(WholeRegister& reg)
{
    auto sub = m_mmu.read(reg.value());
    auto orig = m_a.value();

    m_a.set(static_cast<uint8_t>(orig - sub));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_sbc()
{
    auto sub = fetch_byte();
    auto orig = m_a.value();
    auto carry = m_f.flag_carry();
    auto result = sub - orig - carry;

    m_a.set(static_cast<uint8_t>(result));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((sub & 0xF) - (orig & 0xF) - carry) < 0) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (result < 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_sbc(WholeRegister& reg)
{
    auto sub = m_mmu.read(reg.value());
    auto orig = m_a.value();
    auto carry = m_f.flag_carry();
    auto result = sub - orig - carry;

    m_a.set(static_cast<uint8_t>(result));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((sub & 0xF) - (orig & 0xF) - carry) < 0) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (result < 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_sbc(ByteRegister& reg)
{
    auto sub = reg.value();
    auto orig = m_a.value();
    auto carry = m_f.flag_carry();
    auto result = sub - orig - carry;

    m_a.set(static_cast<uint8_t>(result));

    (m_a.value() == 0x00) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((sub & 0xF) - (orig & 0xF) - carry) < 0) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (result < 0) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 4;
}

// Logic Instructions
unsigned long CPU::instruction_and(void)
{
    uint8_t value = fetch_byte();
    uint8_t result = value & m_a.value();

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(true);
    m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_and(ByteRegister& reg)
{
    uint8_t result = reg.value() & m_a.value();

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(true);
    m_f.set_flag_carry(false);

    return 4;
}

unsigned long CPU::instruction_and(WholeRegister& reg)
{
    uint8_t value = m_mmu.read(reg.value());
    uint8_t result = value & m_a.value();

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(true);
    m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_cp()
{
    auto sub = fetch_byte();
    auto orig = m_a.value();

    auto result = static_cast<int8_t>(orig - sub);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_cp(ByteRegister& reg)
{
    auto sub = reg.value();
    auto orig = m_a.value();

    auto result = static_cast<int8_t>(orig - sub);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 4;
}

unsigned long CPU::instruction_cp(WholeRegister& reg)
{
    auto sub = m_mmu.read(reg.value());
    auto orig = m_a.value();

    auto result = static_cast<int8_t>(orig - sub);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    (((orig & 0x0F) < (sub & 0x0F))) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((orig & 0xFF) < (sub & 0xFF))) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_inc(ByteRegister& reg)
{
    reg.increment();

    (reg.value() == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((reg.value() & 0x0F) == 0x00) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);

    return 4;
}

unsigned long CPU::instruction_inc_hl_address()
{
    auto value = m_mmu.read(m_hl.value());
    auto result = static_cast<uint8_t>(value + 1);

    m_mmu.write(m_hl.value(), result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    ((result & 0x0F) == 0x00) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);

    return 12;
}

unsigned long CPU::instruction_inc(WholeRegister& reg)
{
    reg.increment();

    return 8;
}

unsigned long CPU::instruction_inc(WordRegister& reg)
{
    reg.increment();

    return 8;
}

unsigned long CPU::instruction_dec(ByteRegister& reg)
{
    reg.decrement();

    (reg.value() == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    ((reg.value() & 0x0F) == 0x0F) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);

    return 4;
}

unsigned long CPU::instruction_dec_hl_address()
{
    auto value = m_mmu.read(m_hl.value());
    auto result = static_cast<uint8_t>(value - 1);

    m_mmu.write(m_hl.value(), result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(true);
    ((result & 0x0F) == 0x0F) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);

    return 12;
}

unsigned long CPU::instruction_dec(WholeRegister& reg)
{
    reg.decrement();

    return 8;
}

unsigned long CPU::instruction_dec(WordRegister& reg)
{
    reg.decrement();

    return 8;
}

unsigned long CPU::instruction_or()
{
    auto value = fetch_byte();

    auto result = value | m_a.value();

    m_a.set(value);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_or(ByteRegister& reg)
{
    auto value = reg.value() | m_a.value();

    m_a.set(value);

    (value == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);

    return 4;
}

unsigned long CPU::instruction_or(WholeRegister& reg)
{
    auto value = m_mmu.read(reg.value());
    auto or_value = value | m_a.value();

    m_a.set(or_value);

    (or_value == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_xor()
{
    auto value = fetch_byte();
    auto result = m_a.value() ^ value;

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);

    return 8;
}

unsigned long CPU::instruction_xor(ByteRegister& reg)
{
    auto result = m_a.value() ^ reg.value();

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);

    return 4;
}

unsigned long CPU::instruction_xor_hl_address()
{
    auto value = m_mmu.read(m_hl.value());
    auto result = m_a.value() ^ value;

    m_a.set(result);

    (result == 0) ? m_f.set_zero_flag(true) : m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(false);

    return 8;
}
