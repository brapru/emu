#pragma once

#include <Register.h>

class CPU;

class Timer {
public:
    Timer(CPU& cpu);

    void tick();

    void write(uint16_t address, uint8_t value);
    uint8_t read(uint16_t address);

    ByteRegister divider() { return m_divider; }
    ByteRegister counter() { return m_counter; }
    ByteRegister modulo() { return m_modulo; }
    ByteRegister control() { return m_control; }

private:
    CPU& m_cpu;

    ByteRegister m_divider;
    ByteRegister m_counter;
    ByteRegister m_modulo;
    ByteRegister m_control;
};