#pragma once

#include <stdint.h>

class ByteRegister {
public:
    ByteRegister() { }
    ByteRegister(const uint8_t value)
        : m_value(value)
    {
    }

    void set(uint8_t value) { m_value = value; }
    void increment() { m_value++; }
    void decrement() { m_value--; }

    uint8_t value() { return m_value; }

protected:
    uint8_t m_value;
};

class FlagRegister : public ByteRegister {
public:
    FlagRegister() = default;

    void set_zero_flag(bool set);
    void set_subtraction_flag(bool set);
    void set_half_carry_flag(bool set);
    void set_flag_carry(bool set);
};

class WordRegister {
public:
    WordRegister() { }
    WordRegister(const uint16_t value)
        : m_value(value)
    {
    }

    void set(uint16_t value) { m_value = value; }
    void increment() { m_value++; }
    void decrement() { m_value--; }

    uint16_t value() { return m_value; }

private:
    uint16_t m_value;
};