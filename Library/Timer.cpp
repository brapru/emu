#include <CPU.h>
#include <Timer.h>
#include <Utils/Bitwise.h>
#include <Utils/Format.h>

Timer::Timer(CPU& cpu)
    : m_cpu(cpu)
{
}

void Timer::tick(unsigned int cycles)
{
    m_divider_counter += cycles;

    if (m_divider_counter >= 256) {
        m_divider_counter = 0;
        m_divider.increment();
    }

    if (!m_timer_enabled)
        return;

    m_timer_counter -= cycles;
    while (m_timer_counter <= 0) {
        m_timer_counter += Frequencies::FrequencyCounts[m_timer_frequency];
        m_counter.increment();

        if (m_counter.value() == 0x00) {
            m_counter.set(m_modulo.value());
            m_cpu.request_interrupt(Interrupts::TIMER);
        }
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
        m_divider.set(0x00);
        break;
    case 0xFF05:
        m_counter.set(value);
        break;
    case 0xFF06:
        m_modulo.set(value);
        break;
    case 0xFF07:
        checkbit(value, 2) ? (m_timer_enabled = true) : (m_timer_enabled = false);

        m_control.set(value);
        m_timer_frequency = value & 0x03;
        m_timer_counter = Frequencies::FrequencyCounts[m_timer_frequency];

        break;
    }
}