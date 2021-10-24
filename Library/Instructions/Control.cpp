#include <CPU.h>

void CPU::instruction_nop(void)
{
}

void CPU::instruction_di(void)
{
    m_interrupt_master_enable = false;
}