#include <Timer.h>
#include <Utils/Format.h>

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