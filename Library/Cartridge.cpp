#include <fstream>
#include <string>

#include <Cartridge.h>
#include <Utils/Constants.h>
#include <Utils/File.h>
#include <Utils/Format.h>

Cartridge::Cartridge(std::vector<uint8_t> rom_data)
    : m_rom(rom_data)
    , m_battery_enabled(false)
    , m_current_rom_bank(1)
    , m_current_ram_bank(0)
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
        m_type_name = "ROM ONLY";
        break;
    case 0x01:
        m_type_name = "MBC1";
        break;
    case 0x02:
        m_type_name = "MBC1+RAM";
        break;
    case 0x03:
        m_type_name = "MBC1+RAM+BATTERY";
        m_battery_enabled = true;
        break;
    case 0x13:
        m_type_name = "MBC3+RAM+BATTERY";
        m_battery_enabled = true;
        break;
    default:
        m_type_name = "UNKNOWN";
    }

    switch (m_header.ram_size) {
    case 0x00:
        m_ram_size = 0;
        break;
    case 0x01:
        m_ram_size = 1024 * 2;
        break;
    case 0x02:
        m_ram_size = 1024 * 8;
        break;
    case 0x03:
        m_ram_size = 1024 * 32;
        break;
    }

    uint16_t checksum = 0;
    for (uint16_t i = 0x0134; i < 0x014C; i++) {
        checksum = checksum - m_rom[i] - 1;
    }

    if (checksum & 0xFF) {
        m_is_valid_checksum = true;
        m_is_loaded = true;
    }

    if (m_battery_enabled) {
        std::string ram_path("./Tests/Roms/legend_of_zelda.gb.battery");
        std::vector<uint8_t> ram = read_rom_into_vector(ram_path);
        if (ram.empty()) {
            outln("Could not find previous ram file.");
            m_ram.reserve(m_ram_size);
        } else {
            m_ram = std::move(ram);
        }
    }

    outln("Cartridge Header Information:");
    outln("\t title: {}", m_header.title);
    outln("\t new_license_code: {} - {}", m_header.new_license_code, new_license_code());
    outln("\t flag_sgb: {}", m_header.flag_sgb);
    outln("\t type: {} - {}", m_header.type, m_type_name);
    outln("\t rom_size: {} KB", m_header.rom_size);
    outln("\t ram_size: {}", m_header.ram_size);
    outln("\t checksum: {}", (checksum & 0xFF) ? "PASSED" : "FAILED");
}

uint8_t Cartridge::read(uint16_t const address)
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

void Cartridge::write(uint16_t const address, uint8_t value)
{
    if (address < 0x2000) {
        if (m_header.type == 1 || m_header.type == 2 || m_header.type == 3 )  {
            m_ram_enabled = ((value & 0xF) == 0xA);
        }
    }

    if ((address >= 0x2000) && (address < 0x4000)) {
        if (m_header.type == 1 || m_header.type == 2 || m_header.type == 3 )  {
            uint8_t lo = value & 0x1F;
            m_current_rom_bank &= 0xE0;
            m_current_rom_bank |= lo;
            if (m_current_rom_bank == 0)
                m_current_rom_bank++;
        }
    }

    if ((address >= 0x4000) && (address < 0x6000)) {
        if (m_header.type == 1 || m_header.type == 2 || m_header.type == 3 )  {
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
    }

    if ((address >= 0x6000) && (address < 0x8000)) {
        if (m_header.type == 1 || m_header.type == 2 || m_header.type == 3 )  {
            uint8_t new_value = value & 0x1;
            m_rom_banking = (new_value == 0) ? true : false;
            if (m_rom_banking)
                m_current_ram_bank = 0;
        }
    }

    if ((address >= 0xA000) && (address < 0xC000)) {
        if (m_ram_enabled) {
            uint16_t write = address - 0xA000;
            m_ram[write + (m_current_ram_bank * 0x2000)] = value;
        }
    }   
}
