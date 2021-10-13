#include <Register.h>
#include <Utils/Bitwise.h>

void FlagRegister::set_zero_flag(bool set)
{
    set ? bitset(m_value, 7) : bitclear(m_value, 7);
}

void FlagRegister::set_subtraction_flag(bool set)
{
    set ? bitset(m_value, 6) : bitclear(m_value, 6);
}

void FlagRegister::set_half_carry_flag(bool set)
{
    set ? bitset(m_value, 5) : bitclear(m_value, 5);
}

void FlagRegister::set_flag_carry(bool set)
{
    set ? bitset(m_value, 4) : bitclear(m_value, 4);
}