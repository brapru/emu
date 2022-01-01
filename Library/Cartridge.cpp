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
    else if (header.type == 0x01 || header.type == 0x02 || header.type == 0x03)
        return std::make_shared<MBC1>(std::move(rom_data));

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

MBC1::MBC1(std::vector<uint8_t> rom_data)
    : Cartridge(std::move(rom_data))
    , m_battery_enabled(false)
    , m_current_rom_bank(1)
    , m_current_ram_bank(0)
{
}

uint8_t MBC1::read(uint16_t const address)
{
    if ((address >= 0x4000) && (address <= 0x7FFF)) {
        uint16_t read = address - 0x4000;
        return m_rom[read + (m_current_rom_bank * 0x4000)];
    }

    if ((address >= 0xA000) && (address <= 0xBFFF)) {
        uint16_t read = address - 0xA000;
        return m_ram[read + (m_current_ram_bank * 0x2000)];
    }

    return m_rom[address];
}

void MBC1::write(uint16_t const address, uint8_t value)
{
    if (address < 0x2000) {
        m_ram_enabled = ((value & 0xF) == 0xA);
    }

    if ((address >= 0x2000) && (address < 0x4000)) {
        uint8_t lo = value & 0x1F;
        m_current_rom_bank &= 0xE0;
        m_current_rom_bank |= lo;
        if (m_current_rom_bank == 0)
            m_current_rom_bank++;
    }

    if ((address >= 0x4000) && (address < 0x6000)) {
        if (m_rom_banking) {
            m_current_rom_bank &= 0x1F;
            value &= 0xE0;
            m_current_rom_bank |= value;
            if (m_current_rom_bank == 0)
                m_current_rom_bank++;
        } else {
            m_current_ram_bank = value & 0x3;
        }
    }

    if ((address >= 0x6000) && (address < 0x8000)) {
        uint8_t new_value = value & 0x1;
        m_rom_banking = (new_value == 0) ? true : false;
        if (m_rom_banking)
            m_current_ram_bank = 0;
    }

    if ((address >= 0xA000) && (address < 0xC000)) {
        if (m_ram_enabled) {
            uint16_t write = address - 0xA000;
            m_ram[write + (m_current_ram_bank * 0x2000)] = value;
        }
    }
}