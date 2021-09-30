#include <Register.h>
#include <Utils/Bitwise.h>

void FlagRegister::set_zero_flag(bool set)
{
    if (set)
        bitset(m_value, 7);
}

void FlagRegister::set_subtraction_flag(bool set)
{
    // TODO: Implement
}

void FlagRegister::set_half_carry_flag(bool set)
{
    // TODO: Implement
}

void FlagRegister::set_flag_carry(bool set)
{
    // TODO: Implement
}