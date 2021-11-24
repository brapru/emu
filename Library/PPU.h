#pragma once

#include <CPU.h>
#include <Register.h>
#include <Utils/Bitwise.h>

#include <stdint.h>
#include <vector>

namespace LCD {
constexpr uint8_t LINES_PER_FRAME = 154;
constexpr uint8_t Y_WINDOW_WIDTH = 144;
}

namespace LCD::Cycles {
constexpr uint32_t HBLANK = 204;
constexpr uint32_t VBLANK = 456;
constexpr uint32_t OAM = 80;
constexpr uint32_t VRAM = 172;
}

namespace LCD::Mode {
constexpr uint8_t HBLANK = 0x00;
constexpr uint8_t VBLANK = 0x01;
constexpr uint8_t OAM = 0x02;
constexpr uint8_t VRAM = 0x03;
}

class CPU;

class PPU {
public:
    PPU(CPU& cpu);

    void tick(uint64_t cycles);

    void increment_ly_coordinate();

    uint64_t current_frame() { return m_current_frame; }

    void set_lcd_mode(uint8_t mode) { m_lcd_mode = mode; }
    bool lcd_hblank_interrupts_enabled(void) { return checkbit(m_lcd_status.value(), 3); }
    bool lcd_vblank_interrupts_enabled(void) { return checkbit(m_lcd_status.value(), 4); }
    bool lcd_oam_interrupts_enabled(void) { return checkbit(m_lcd_status.value(), 5); }
    bool lcd_ly_stat_interrupts_enabled(void) { return checkbit(m_lcd_status.value(), 6); }

    uint8_t lcd_read(uint16_t const& address);
    void lcd_write(uint16_t const address, uint8_t const value);

    uint8_t vram_read(uint16_t const& address);
    void vram_write(uint16_t const address, uint8_t const value);

    uint8_t oam_read(uint16_t const& address);
    void oam_write(uint16_t const address, uint8_t const value);

private:
    CPU& m_cpu;

    ByteRegister m_lcd_control;
    ByteRegister m_lcd_status;
    ByteRegister m_lcd_scroll_y;
    ByteRegister m_lcd_scroll_x;
    ByteRegister m_lcd_y_cord;
    ByteRegister m_lcd_y_compare;
    ByteRegister m_lcd_window_y;
    ByteRegister m_lcd_window_x;

    uint8_t m_lcd_mode = LCD::Mode::OAM;

    uint64_t m_cycles;
    uint64_t m_current_frame;

    std::vector<uint8_t> m_oam_ram = std::vector<uint8_t>(0xA0);
    std::vector<uint8_t> m_vram = std::vector<uint8_t>(0x2000);
};