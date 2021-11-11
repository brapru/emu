#include <CPU.h>

unsigned long CPU::instruction_call(void)
{
    auto address = CPU::fetch_word();
    stack_push(m_pc);
    m_pc.set(address);

    return 24;
}

unsigned long CPU::instruction_conditional_call(Condition condition)
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

    if (should_call) {
        instruction_call();
        return 24;
    } else {
        CPU::fetch_word();
        return 12;
    }
}

unsigned long CPU::instruction_ret(void)
{
    auto address = stack_pop();
    m_pc.set(address);

    return 16;
}

unsigned long CPU::instruction_ret(Condition condition)
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

    if (should_ret) {
        instruction_ret();
        return 20;
    } else {
        return 8;
    }
}

unsigned long CPU::instruction_reti()
{
    instruction_ret();
    instruction_ei();

    return 16;
}

unsigned long CPU::instruction_rst(uint16_t offset)
{
    stack_push(m_pc);
    m_pc.set(offset);

    return 16;
}

unsigned long CPU::instruction_jp(void)
{
    auto address = CPU::fetch_word();
    m_pc.set(address);

    return 16;
}

unsigned long CPU::instruction_jp(WholeRegister& reg)
{
    m_pc.set(reg.value());

    return 4;
}

unsigned long CPU::instruction_jp(Condition condition)
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
        instruction_jp();
        return 16;
    } else {
        CPU::fetch_word();
        return 12;
    }
}

unsigned long CPU::instruction_jr(void)
{
    auto jumps = static_cast<int8_t>(fetch_byte());
    m_pc.set(m_pc.value() + static_cast<uint16_t>(jumps));

    return 12;
}

unsigned long CPU::instruction_jr(Condition condition)
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
        return 12;
    } else {
        CPU::fetch_byte();
        return 8;
    }
}