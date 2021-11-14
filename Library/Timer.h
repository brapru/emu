#pragma once

#include <Register.h>

class CPU;

namespace Frequencies {
constexpr int FrequencyCounts[] = { 1024, 16, 64, 256 };
constexpr uint8_t Frequency4096 = 0x00;
constexpr uint8_t Frequency262144 = 0x01;
constexpr uint8_t Frequency65536 = 0x02;
constexpr uint8_t Frequency16384 = 0x03;
}

class Timer {
public:
    Timer(CPU& cpu);

    void tick(unsigned int cycles);

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

    bool m_timer_enabled = true;

    unsigned int m_divider_counter;
    unsigned int m_timer_frequency = Frequencies::Frequency4096;
    int m_timer_counter = Frequencies::FrequencyCounts[Frequencies::Frequency4096];
};