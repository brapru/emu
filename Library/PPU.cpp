#include <PPU.h>
#include <Utils/Address.h>

uint8_t PPU::vram_read(uint16_t const& address)
{
    if (address >= 0x8000 && address <= 0x9FFF)
        return m_vram[address - 0x8000];

    return m_vram[address];
}

void PPU::vram_write(uint16_t const address, uint8_t const value)
{
    if (address >= 0x8000 && address <= 0x9FFF)
        m_vram[address - 0x8000] = value;

    m_vram[address] = value;
}

uint8_t PPU::oam_read(uint16_t const& address)
{
    if (address >= 0xFE00 && address <= 0xFE9F)
        return m_oam_ram[address - 0xFE00];

    return m_oam_ram[address];
}

void PPU::oam_write(uint16_t const address, uint8_t const value)
{
    if (address >= 0xFE00 && address <= 0xFE9F)
        m_oam_ram[address - 0xFE00] = value;
    m_oam_ram[address] = value;
}