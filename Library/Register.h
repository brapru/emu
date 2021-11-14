#pragma once

#include <Utils/Bitwise.h>

#include <stdint.h>

class ByteRegister {
public:
    ByteRegister() { }
    ByteRegister(const uint8_t value)
        : m_value(value)
    {
    }

    virtual void set(uint8_t value) { m_value = value; }

    void set_bit(int bit, bool set);

    void increment() { m_value++; }
    void decrement() { m_value--; }

    uint8_t value() { return m_value; }

protected:
    uint8_t m_value;
};

class FlagRegister : public ByteRegister {
public:
    FlagRegister() = default;

    void set(uint8_t value) { m_value = value & 0xF0; }

    bool zero_flag() { return checkbit(m_value, 7); }
    bool subtraction_flag() { return checkbit(m_value, 6); }
    bool half_carry_flag() { return checkbit(m_value, 5); }
    bool flag_carry() { return checkbit(m_value, 4); }

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

    uint8_t lo() { return ((m_value >> 0) & 0xFF); }
    uint8_t hi() { return ((m_value >> 8) & 0xFF); }

    uint16_t value() { return m_value; }

private:
    uint16_t m_value;
};

class WholeRegister {
public:
    WholeRegister(ByteRegister& hi, ByteRegister& lo)
        : m_hi(hi)
        , m_lo(lo)
    {
    }

    uint8_t lo() { return m_lo.value(); }
    uint8_t hi() { return m_hi.value(); }
    uint16_t value() { return (m_hi.value() << 8) | (m_lo.value() & 0xFF); }

    void set(uint16_t value)
    {
        m_lo.set(((value >> 0) & 0xFF));
        m_hi.set(((value >> 8) & 0xFF));
    }

    void increment() { set(value() + 0x1); }
    void decrement() { set(value() - 0x1); }

private:
    ByteRegister& m_hi;
    ByteRegister& m_lo;
};