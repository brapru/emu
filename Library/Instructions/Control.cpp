#include <CPU.h>

void CPU::instruction_nop(void)
{
}

void CPU::instruction_di(void)
{
    m_interrupt_master_enable = false;
}

void CPU::instruction_ei(void)
{
    m_interrupt_master_enable = true;
}

void CPU::instruction_cpl(void)
{
    auto value = ~m_a.value();
    m_a.set(value);

    m_f.set_subtraction_flag(true);
    m_f.set_half_carry_flag(true);
}