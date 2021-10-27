#include <CPU.h>

void CPU::instruction_call(void)
{
    auto address = CPU::fetch_word();
    stack_push(m_pc);
    m_pc.set(address);
}

void CPU::instruction_conditional_call(uint8_t const& opcode)
{
    bool should_call;

    switch (opcode) {
    case 0xC4: // NZ
        should_call = !m_f.zero_flag();
        break;
    }

    if (should_call)
        instruction_call();
    else
        CPU::fetch_word();
}

void CPU::instruction_ret(void)
{
    auto address = stack_pop();
    m_pc.set(address);
}

void CPU::instruction_ret(uint8_t const& opcode)
{
    bool should_ret;

    switch (opcode) {
    case 0xC0: // NZ
        should_ret = !m_f.zero_flag();
        break;
    case 0xC8: // Z
        should_ret = m_f.zero_flag();
        break;
    case 0xD0: // NC
        should_ret = !m_f.flag_carry();
        break;
    case 0xD8:
        should_ret = m_f.flag_carry();
        break;
    }

    if (should_ret)
        instruction_ret();
}

void CPU::instruction_jp(void)
{
    auto address = CPU::fetch_word();
    m_pc.set(address);
}

void CPU::instruction_jr(void)
{
    auto jumps = static_cast<int8_t>(fetch_byte());
    m_pc.set(m_pc.value() + static_cast<uint16_t>(jumps));
}

void CPU::instruction_jr(uint8_t const& opcode)
{
    bool should_jp;

    switch (opcode) {
    case 0x20: // NZ
        should_jp = !m_f.zero_flag();
        break;
    case 0x28: // Z
        should_jp = m_f.zero_flag();
        break;
    case 0x30: // NC
        should_jp = !m_f.flag_carry();
        break;
    case 0x38: // C
        should_jp = m_f.flag_carry();
        break;
    }

    if (should_jp) {
        auto jumps = static_cast<int8_t>(fetch_byte());
        m_pc.set(m_pc.value() + static_cast<uint16_t>(jumps));
    } else {
        CPU::fetch_byte();
    }
}