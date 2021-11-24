#include <Cartridge.h>
#include <MMU.h>
#include <PPU.h>
#include <Utils/Address.h>
#include <Utils/Format.h>

MMU::MMU(Cartridge& cartridge, CPU& cpu, PPU& ppu, Timer& timer, Serial& serial)
    : m_cartridge(cartridge)
    , m_cpu(cpu)
    , m_ppu(ppu)
    , m_timer(timer)
    , m_serial(serial)
{
}

uint8_t MMU::read(uint16_t const address)
{
    if (address < 0x8000)
        return m_cartridge.read(address);
    else if (address < 0xA000)
        return m_ppu.vram_read(address);
    else if (address < 0xE000)
        return memory_read(address);
    else if (address < 0xFEA0)
        return m_ppu.oam_read(address);
    else if (address < 0xFF80)
        return io_read(address);
    else if (address_in_range(address, 0xFF80, 0xFFFE))
        return memory_read(address);
    else if (address == 0xFFFF)
        return m_cpu.interrupt_enable();

    outln("MMU read at address not yet implemented.");
    exit(1);
}

void MMU::write(uint16_t const address, uint8_t const value)
{
    if (address < 0x8000) {
        m_cartridge.write(address, value);
        return;
    } else if (address < 0xA000) {
        m_ppu.vram_write(address, value);
        return;
    } else if (address < 0xE000) {
        memory_write(address, value);
        return;
    } else if (address < 0xFEA0) {
        m_ppu.oam_write(address, value);
        return;
    } else if (address < 0xFF80) {
        io_write(address, value);
        return;
    } else if (address_in_range(address, 0xFF80, 0xFFFE)) {
        memory_write(address, value);
        return;
    } else if (address == 0xFFFF) {
        m_cpu.set_interrupt_enable(value);
        return;
    }

    outln("MMU write at address 0x:{:2X} not yet implemented", address);
    exit(1);
}

uint8_t MMU::memory_read(uint16_t const address)
{
    return m_memory[address];
}

void MMU::memory_write(uint16_t const address, uint8_t const value)
{
    m_memory[address] = value;
}

uint8_t MMU::io_read(uint16_t const address)
{
    if (address == 0xFF01)
        return m_serial.read_data();
    if (address == 0xFF02)
        return m_serial.read_control();
    if (address_in_range(address, 0xFF04, 0xFF07)) {
        return m_timer.read(address);
    } else if (address_in_range(address, 0xFF24, 0xFF26)) {
        outln("Sound Control Register read at address 0x:{:2X} not yet implemented ", address);
        return 0x00;
    } else if (address == 0xFF0F) {
        return m_cpu.interrupt_flag();
    } else if (address_in_range(address, 0xFF40, 0xFF4B)) {
        return m_ppu.lcd_read(address);
    } else {
        outln("IO read at address 0x:{:2X} not yet implemented", address);
    }
}

void MMU::io_write(uint16_t const address, uint8_t const value)
{
    if (address == 0xFF01) {
        m_serial.write_data(value);
        return;
    }
    if (address == 0xFF02) {
        m_serial.write_control(value);
        return;
    }
    if (address_in_range(address, 0xFF04, 0xFF07)) {
        m_timer.write(address, value);
        return;
    } else if (address_in_range(address, 0xFF24, 0xFF26)) {
        outln("Sound Control Register write at address 0x:{:2X} not yet implemented ", address);
        return;
    } else if (address == 0xFF0F) {
        m_cpu.set_interrupt_flag(value);
        return;
    } else if (address == 0xFF46) {
        m_dma_transfer(value);
        return;
    } else if (address_in_range(address, 0xFF40, 0xFF4B)) {
        m_ppu.lcd_write(address, value);
        return;
    } else {
        outln("IO write at address 0x:{:2X} not yet implemented", address);
        return;
    }
}

void MMU::m_dma_transfer(uint8_t const value)
{
    uint16_t address = value << 8;
    for (int offset = 0; offset < 0xA0; offset++) {
        auto value = memory_read(address + offset);
        m_ppu.oam_write((0xFE00 + offset), value);
    }
}