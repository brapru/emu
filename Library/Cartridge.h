#pragma once

#include <string>
#include <vector>

struct CartridgeHeader {
    uint8_t entry[4];
    uint8_t nintendo_logo[0x30];
    uint8_t title[16];
    uint16_t new_license_code;
    uint8_t flag_sgb;
    uint8_t type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t destination_code;
    uint8_t old_license_code;
    uint8_t version;
    uint8_t checksum;
    uint16_t global_checksum;
};

class Cartridge {
public:
    Cartridge(std::vector<uint8_t> rom_data);

    bool is_loaded() { return m_is_loaded; }

    uint8_t read(uint16_t const address);
    void write(uint16_t const address, uint8_t value);

    std::string title() { return std::string((char*)m_header.title); }
    std::string new_license_code() { return m_new_license_code; }

    bool is_valid_checksum() { return m_is_valid_checksum; }

private:
    bool m_is_loaded = false;
    std::vector<uint8_t> m_rom;
    std::vector<uint8_t> m_ram;

    uint8_t m_current_rom_bank;
    uint8_t m_current_ram_bank;

    CartridgeHeader m_header;
    std::string m_new_license_code;
    std::string m_type_name;
    uint32_t m_ram_size;
    
    bool m_battery_enabled;
    bool m_ram_enabled;
    bool m_rom_banking;

    bool m_is_valid_checksum;

    void initialize_header();
};