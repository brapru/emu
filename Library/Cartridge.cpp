#include <fstream>
#include <string>

#include <Cartridge.h>
#include <Utils/Constants.h>
#include <Utils/Format.h>

Cartridge::Cartridge(std::vector<uint8_t> rom_data)
    : m_rom(rom_data)
{
    initialize_header();
}

void Cartridge::initialize_header()
{
    if (m_rom.empty())
        return;

    std::memcpy(&m_header, &m_rom[0x0100], sizeof(CartridgeHeader));

    switch (m_header.new_license_code) {
    case 0x00:
        m_new_license_code = "NONE";
        break;
    default:
        m_new_license_code = "UNKNOWN";
    }

    switch (m_header.type) {
    case 0x00:
        m_type = "ROM ONLY";
        break;
    case 0x01:
        m_type = "MBC1";
        break;
    default:
        m_type = "UNKNOWN";
    }

    switch (m_header.ram_size) {
    case 0x00:
        m_ram_size = "No RAM";
        break;
    default:
        m_ram_size = "UNKNOWN";
    }

    uint16_t checksum = 0;
    for (uint16_t i = 0x0134; i < 0x014C; i++) {
        checksum = checksum - m_rom[i] - 1;
    }

    if (checksum & 0xFF) {
        m_is_valid_checksum = true;
        m_is_loaded = true;
    }

    outln("Cartridge Header Information:");
    outln("\t title: {}", m_header.title);
    outln("\t new_license_code: {} - {}", m_header.new_license_code, m_new_license_code);
    outln("\t flag_sgb: {}", m_header.flag_sgb);
    outln("\t type: {} - {}", m_header.type, m_type);
    outln("\t rom_size: {} KB", m_header.rom_size);
    outln("\t ram_size: {} - {}", m_header.ram_size, m_ram_size);
    outln("\t checksum: {}", (checksum & 0xFF) ? "PASSED" : "FAILED");
}

uint8_t Cartridge::read(uint16_t const address)
{
    return m_rom[address];
}

void Cartridge::write(uint16_t const address, uint8_t value)
{
    outln("Cartridge write at address not yet implemented.");
}