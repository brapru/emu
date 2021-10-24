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

void CPU::instruction_push(WholeRegister& reg)
{
    stack_push(reg);
}

void CPU::instruction_pop(WholeRegister& reg)
{
    auto value = stack_pop();
    reg.set(value);
}