#include <Timer.h>

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