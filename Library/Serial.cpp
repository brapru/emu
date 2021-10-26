#include <Serial.h>
#include <Utils/Format.h>

Serial::Serial(MMU& mmu)
    : m_mmu(mmu)
{
}

void Serial::update()
{
    if (m_mmu.read(0xFF02) == 0x81) {
        char c = m_mmu.read(0xFF01);

        m_data.push_back(c);

        m_mmu.write(0xFF02, 0);
    }
}

void Serial::print()
{
    if (!m_data.empty()) {
        out("Serial Output: ");
        for (auto it : m_data)
            out("{}", it);
        outln("");
    }
}