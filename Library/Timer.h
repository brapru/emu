#pragma once

#include <Register.h>

class Timer {
public:
    Timer() {};

    void write(uint16_t address, uint8_t value);

    ByteRegister divider() { return m_divider; }
    ByteRegister counter() { return m_counter; }
    ByteRegister modulo() { return m_modulo; }
    ByteRegister control() { return m_control; }

private:
    ByteRegister m_divider;
    ByteRegister m_counter;
    ByteRegister m_modulo;
    ByteRegister m_control;
};