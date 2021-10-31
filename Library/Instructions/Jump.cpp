#include <CPU.h>

void CPU::instruction_call(void)
{
    auto address = CPU::fetch_word();
    stack_push(m_pc);
    m_pc.set(address);
}

void CPU::instruction_conditional_call(Condition condition)
{
    bool should_call;

    switch (condition) {
    case Condition::NZ:
        should_call = !m_f.zero_flag();
        break;
    case Condition::Z:
        should_call = m_f.zero_flag();
        break;
    case Condition::NC:
        should_call = !m_f.flag_carry();
        break;
    case Condition::C:
        should_call = m_f.flag_carry();
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

void CPU::instruction_ret(Condition condition)
{
    bool should_ret;

    switch (condition) {
    case Condition::NZ:
        should_ret = !m_f.zero_flag();
        break;
    case Condition::Z:
        should_ret = m_f.zero_flag();
        break;
    case Condition::NC:
        should_ret = !m_f.flag_carry();
        break;
    case Condition::C:
        should_ret = m_f.flag_carry();
        break;
    }

    if (should_ret)
        instruction_ret();
}

void CPU::instruction_reti()
{
    instruction_ret();
    instruction_ei();
}

void CPU::instruction_jp(void)
{
    auto address = CPU::fetch_word();
    m_pc.set(address);
}

void CPU::instruction_jp(WholeRegister& reg)
{
    m_pc.set(reg.value());
}

void CPU::instruction_jp(Condition condition)
{
    bool should_jp;

    switch (condition) {
    case Condition::NZ:
        should_jp = !m_f.zero_flag();
        break;
    case Condition::Z:
        should_jp = m_f.zero_flag();
        break;
    case Condition::NC:
        should_jp = !m_f.flag_carry();
        break;
    case Condition::C:
        should_jp = m_f.flag_carry();
        break;
    }

    if (should_jp)
        instruction_jp();
    else
        CPU::fetch_word();
}

void CPU::instruction_jr(void)
{
    auto jumps = static_cast<int8_t>(fetch_byte());
    m_pc.set(m_pc.value() + static_cast<uint16_t>(jumps));
}

void CPU::instruction_jr(Condition condition)
{
    bool should_jp;

    switch (condition) {
    case Condition::NZ:
        should_jp = !m_f.zero_flag();
        break;
    case Condition::Z:
        should_jp = m_f.zero_flag();
        break;
    case Condition::NC:
        should_jp = !m_f.flag_carry();
        break;
    case Condition::C:
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