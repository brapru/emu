#include <CPU.h>

unsigned long CPU::instruction_nop(void)
{
    return 4;
}

unsigned long CPU::instruction_halt(void)
{
    m_is_halted = true;

    return 1;
}

unsigned long CPU::instruction_di(void)
{
    m_interrupt_master_enable = false;

    return 4;
}

unsigned long CPU::instruction_ei(void)
{
    m_interrupt_master_enable = true;

    return 4;
}

unsigned long CPU::instruction_cpl(void)
{
    auto value = ~m_a.value();
    m_a.set(value);

    m_f.set_subtraction_flag(true);
    m_f.set_half_carry_flag(true);

    return 4;
}

unsigned long CPU::instruction_scf(void)
{
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(true);

    return 4;
}

unsigned long CPU::instruction_ccf(void)
{
    m_f.set_subtraction_flag(false);
    m_f.set_half_carry_flag(false);
    m_f.set_flag_carry(!m_f.flag_carry());

    return 4;
}