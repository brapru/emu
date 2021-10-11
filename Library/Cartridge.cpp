#include <fstream>
#include <string>

#include <Cartridge.h>
#include <Utils/Constants.h>
#include <Utils/Format.h>

Cartridge::Cartridge(std::string const& filename)
{
    std::streampos rom_file_size;
    std::ifstream rom_file(filename, std::ios::binary);

    if (!rom_file) {
        outln("Could not load rom file: {}", filename);
        m_is_loaded = false;
    }

    rom_file.seekg(0, std::ios::end);
    rom_file_size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg);

    m_data = new char[rom_file_size];
    rom_file.read(m_data, rom_file_size);
    rom_file.close();

    outln("Loaded {} bytes from ROM file: {}", rom_file_size, filename);
    m_is_loaded = true;

    initialize_header();
}

void Cartridge::initialize_header()
{
    std::memcpy(&m_header, &m_data[0x0100], sizeof(CartridgeHeader));

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
    for (u_int16_t i = 0x0134; i < 0x014C; i++) {
        checksum = checksum - m_data[i] - 1;
    }

    outln("Cartridge Header Information:");
    outln("\t title: {}", m_header.title);
    outln("\t new_license_code: {} - {}", m_header.new_license_code, new_license_code());
    outln("\t flag_sgb: {}", m_header.flag_sgb);
    outln("\t type: {} - {}", m_header.type, m_type);
    outln("\t rom_size: {} KB", rom_size());
    outln("\t ram_size: {} - {}", m_header.ram_size, ram_size());
    outln("\t checksum: {}", (checksum & 0xFF) ? "PASSED" : "FAILED");
}

uint8_t Cartridge::read(uint16_t const address)
{
    return m_data[address];
}

void Cartridge::write(uint16_t const address, uint8_t value)
{
    outln("Cartridge write at address not yet implemented.");
    exit(1);
}