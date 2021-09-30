#pragma once

template<typename T>
inline void bitset(T& byte, int bit)
{
    byte |= 1 << bit;
}

template<typename T>
inline void bitclear(T& byte, int bit)
{
    byte &= ~(1 << bit);
}