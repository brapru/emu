#include <Cartridge.h>
#include <MMU.h>
#include <Utils/Format.h>

MMU::MMU(Cartridge& cartridge)
    : m_cartridge(cartridge)
{
}

uint8_t MMU::read(uint16_t const address)
{
    if (address < 0x8000)
        return m_cartridge.read(address);

    outln("MMU read at address not yet implemented.");
    exit(1);
}

void MMU::write(uint16_t const address, uint8_t const value)
{
    if (address < 0x8000) {
        m_cartridge.write(address, value);
        return;
    }

    outln("MMU write at address not yet implemented.");
    exit(1);
}