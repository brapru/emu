#include <CPU.h>

void CPU::instruction_ld(ByteRegister& reg)
{
    auto value = CPU::fetch_byte();
    reg.set(value);
}

void CPU::instruction_ld(WordRegister& reg)
{
    auto value = CPU::fetch_word();
    reg.set(value);
}

void CPU::instruction_ld(WholeRegister& reg)
{
    auto value = CPU::fetch_word();
    reg.set(value);
}

void CPU::instruction_ld(ByteRegister& reg, ByteRegister& from_reg)
{
    reg.set(from_reg.value());
}

void CPU::instruction_ld(ByteRegister& reg, WholeRegister& from_reg)
{
    auto value = m_mmu.read(from_reg.value());
    reg.set(value);
}

void CPU::instruction_ld(WholeRegister& reg, RegisterOperation update)
{
    auto value = m_mmu.read(reg.value());
    m_a.set(value);
    if (update == RegisterOperation::Increment)
        reg.increment();
    if (update == RegisterOperation::Decrement)
        reg.decrement();
}

void CPU::instruction_ld_reg_to_addr(ByteRegister& reg)
{
    auto address = CPU::fetch_word();
    CPU::m_mmu.write(address, reg.value());
}

void CPU::instruction_ld_reg_to_addr(WordRegister& reg)
{
    auto address = CPU::fetch_word();
    CPU::m_mmu.write(address, reg.lo());
    CPU::m_mmu.write(address + 1, reg.hi());
}

void CPU::instruction_ld_reg_to_addr(WholeRegister& whole_reg, ByteRegister& reg)
{
    auto address = whole_reg.value();
    CPU::m_mmu.write(address, reg.value());
}

void CPU::instruction_ld_reg_to_addr(WholeRegister& whole_reg, ByteRegister& reg, RegisterOperation update)
{
    auto address = whole_reg.value();
    CPU::m_mmu.write(address, reg.value());

    if (update == RegisterOperation::Increment)
        whole_reg.increment();
    if (update == RegisterOperation::Decrement)
        whole_reg.decrement();
}

void CPU::instruction_ld_hl()
{
    auto value = CPU::fetch_byte();
    m_mmu.write(m_hl.value(), value);
}

void CPU::instruction_ld_addr_to_reg(ByteRegister& reg)
{
    auto address = CPU::fetch_word();
    auto value = m_mmu.read(address);

    reg.set(value);
}

void CPU::instruction_ldh_a_to_memory()
{
    auto lo = fetch_byte();
    auto address = ((uint16_t)0xFF << 8) | lo;

    CPU::m_mmu.write(address, m_a.value());
}

void CPU::instruction_ldh_memory_to_a()
{
    auto lo = fetch_byte();
    auto address = ((uint16_t)0xFF << 8) | lo;

    auto value = CPU::m_mmu.read(address);
    m_a.set(value);
}

void CPU::instruction_ld_hl_sp()
{
    auto add = static_cast<int8_t>(fetch_byte());
    auto value = m_sp.value();

    auto result = static_cast<int>(value + add);

    m_hl.set(result);

    m_f.set_zero_flag(false);
    m_f.set_subtraction_flag(false);
    (((add ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10) ? m_f.set_half_carry_flag(true) : m_f.set_half_carry_flag(false);
    (((add ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100) ? m_f.set_flag_carry(true) : m_f.set_flag_carry(false);
}

void CPU::instruction_ld_sp_hl()
{
    m_sp.set(m_hl.value());
}

void CPU::instruction_ldh_c_to_a()
{
    auto lo = m_c.value();
    auto address = ((uint16_t)0xFF << 8) | lo;

    auto value = m_mmu.read(address);
    m_a.set(value);
}

void CPU::instruction_ldh_a_to_c()
{
    auto lo = m_c.value();
    auto address = ((uint16_t)0xFF << 8) | lo;

    m_mmu.write(address, m_a.value());
}

void CPU::instruction_push(WholeRegister& reg)
{
    stack_push(reg);
}

void CPU::instruction_pop(WholeRegister& reg)
{
    auto value = stack_pop();
    reg.set(value);
}