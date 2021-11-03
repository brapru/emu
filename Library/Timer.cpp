#include <CPU.h>
#include <Timer.h>
#include <Utils/Bitwise.h>
#include <Utils/Format.h>

Timer::Timer(CPU& cpu)
    : m_cpu(cpu)
{
}

void Timer::tick()
{
    uint16_t orig_divider = m_divider.value();
    m_divider.increment();

    bool timer_update = false;

    switch (m_control.value() & (0b11)) {
    case 0b00:
        timer_update = (orig_divider & (1 << 9)) && (!(m_divider.value() & (1 << 9)));
        break;
    case 0b01:
        timer_update = (orig_divider & (1 << 3)) && (!(m_divider.value() & (1 << 3)));
        break;
    case 0b10:
        timer_update = (orig_divider & (1 << 5)) && (!(m_divider.value() & (1 << 5)));
        break;
    case 0b11:
        timer_update = (orig_divider & (1 << 7)) && (!(m_divider.value() & (1 << 7)));
        break;
    }

    if (timer_update && checkbit(m_control.value(), 2)) {
        m_counter.increment();

        if (m_counter.value() == 0xFF)
            m_counter.set(m_modulo.value());
        m_cpu.request_interrupt(Interrupts::TIMER);
    }
}

uint8_t Timer::read(uint16_t address)
{
    switch (address) {
    case 0xFF04:
        return m_divider.value();
        break;
    case 0xFF05:
        return m_counter.value();
        break;
    case 0xFF06:
        return m_modulo.value();
        break;
    case 0xFF07:
        return m_control.value();
        break;
    default:
        out("Invalid timer read at address: 0x{:2X}", address);
        return 0;
    }
}

void Timer::write(uint16_t address, uint8_t value)
{
    switch (address) {
    case 0xFF04:
        m_divider.set(value);
        break;
    case 0xFF05:
        m_counter.set(value);
        break;
    case 0xFF06:
        m_modulo.set(value);
        break;
    case 0xFF07:
        m_control.set(value);
        break;
    }
}