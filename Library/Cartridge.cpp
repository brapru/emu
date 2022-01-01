#include <fstream>
#include <string>

#include <Cartridge.h>
#include <Utils/Constants.h>
#include <Utils/Format.h>

Cartridge::Cartridge(std::vector<uint8_t> rom_data)
    : m_rom(rom_data)
{
    uint16_t checksum = 0;
    for (uint16_t i = 0x0134; i < 0x014C; i++) {
        checksum = checksum - m_rom[i] - 1;
    }

    if (checksum & 0xFF) {
        m_is_valid_checksum = true;
        m_is_loaded = true;
    }
}

std::shared_ptr<Cartridge> create_cartridge_instance(std::vector<uint8_t> rom_data)
{
    CartridgeHeader header;
    std::memcpy(&header, &rom_data[0x0100], sizeof(CartridgeHeader));

    outln("Cartridge Header Information:");
    outln("\t title: {}", header.title);
    outln("\t new_license_code: {}", header.new_license_code);
    outln("\t flag_sgb: {}", header.flag_sgb);
    outln("\t type: {}", header.type);
    outln("\t rom_size: {} KB", header.rom_size);
    outln("\t ram_size: {}", header.ram_size);

    if (header.type == 0x00)
        return std::make_shared<NoMBC>(std::move(rom_data));
    else
        outln("Cartridge type currently not implemented.");
    exit(1);
};

NoMBC::NoMBC(std::vector<uint8_t> rom_data)
    : Cartridge(std::move(rom_data))
{
}

uint8_t NoMBC::read(uint16_t const address)
{
    return m_rom[address];
}

void NoMBC::write(uint16_t const address, uint8_t value)
{
    outln("Cartridge writes are not supported for ROM ONLY types.");
}