#pragma once

#include <array>
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

    uint8_t virtual read(uint16_t const address) = 0;
    void virtual write(uint16_t const address, uint8_t value) = 0;

    CartridgeHeader header() { return m_header; }

    bool is_valid_checksum() { return m_is_valid_checksum; }

protected:
    std::vector<uint8_t> m_rom;

private:
    bool m_is_loaded = false;

    CartridgeHeader m_header;
    std::string m_new_license_code;
    std::string m_type;
    std::string m_ram_size;

    bool m_is_valid_checksum;

    void initialize_header();
};

std::shared_ptr<Cartridge> create_cartridge_instance(std::vector<uint8_t> rom_data);

class NoMBC : public Cartridge {
public:
    NoMBC(std::vector<uint8_t> rom_data);

    uint8_t read(uint16_t const address) override;
    void write(uint16_t const address, uint8_t value) override;
};

class MBC1 : public Cartridge {
public:
    MBC1(std::vector<uint8_t> rom_data);

    uint8_t read(uint16_t const address) override;
    void write(uint16_t const address, uint8_t value) override;

private:
    std::vector<uint8_t> m_ram;

    uint8_t m_current_rom_bank;
    uint8_t m_current_ram_bank;

    bool m_battery_enabled;
    bool m_ram_enabled;
    bool m_rom_banking;
};

class MBC3 : public Cartridge {
public:
    MBC3(std::vector<uint8_t> rom_data);

    uint8_t read(uint16_t const address) override;
    void write(uint16_t const address, uint8_t value) override;

private:
    std::vector<uint8_t> m_ram;

    uint8_t m_current_rom_bank;
    uint8_t m_current_ram_bank;

    std::array<uint8_t, 0x05> m_rtc_registers;

    bool m_battery_enabled;
    bool m_ram_enabled;
    bool m_rom_banking;
};