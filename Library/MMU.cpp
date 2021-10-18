#include <Cartridge.h>
#include <MMU.h>
#include <Utils/Format.h>

MMU::MMU(Cartridge& cartridge, Timer& timer)
    : m_cartridge(cartridge)
    , m_timer(timer)
{
}

uint8_t MMU::read(uint16_t const address)
{
    if (address < 0x8000)
        return m_cartridge.read(address);
    if (address < 0xE000)
        return memory_read(address);

    outln("MMU read at address not yet implemented.");
    exit(1);
}

void MMU::write(uint16_t const address, uint8_t const value)
{
    if (address < 0x8000) {
        m_cartridge.write(address, value);
        return;
    }
    if (address < 0xE000) {
        memory_write(address, value);
        return;
    }

    outln("MMU write at address 0x:{:2X} not yet implemented", address);
    exit(1);
}

uint8_t MMU::memory_read(uint16_t const address)
{
    return m_memory.at(address);
}

void MMU::memory_write(uint16_t const address, uint8_t const value)
{
    m_memory.at(address) = value;
}