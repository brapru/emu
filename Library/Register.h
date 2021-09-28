#pragma once

template<class T>
class Register {
public:
    Register() { }
    Register(const T value)
        : m_value(value)
    {
    }

    void increment() { m_value++; }
    void decrement() { m_value--; }
    void set(T value) { m_value = value; }

    T value() { return m_value; }

private:
    T m_value;
};