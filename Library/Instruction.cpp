#include <CPU.h>
#include <Utils/Format.h>

void CPU::instruction_nop(void)
{
    out("NOP ");
}

void CPU::instruction_jp(void)
{
    auto address = CPU::fetch_word();
    out("JP 0x{:04X} ", address);
    m_pc.set(address);
}

void CPU::instruction_xor(void)
{
    auto value = fetch_byte();
    out("XOR A 0x{:02X}", value);

    auto result = m_a.value() ^ value & 0xFF;

    m_flags.set_zero_flag(result == 0);
    m_flags.set_subtraction_flag(false);
    m_flags.set_half_carry_flag(false);
    m_flags.set_flag_carry(false);

    m_a.set(result);
}