#pragma once

#include <CPU.h>
#include <Register.h>
#include <Utils/Bitwise.h>

#include <array>
#include <memory>
#include <queue>
#include <stdint.h>
#include <vector>

constexpr std::array<uint32_t, 4> DEFAULT_COLORS = { 0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000 };

enum class PaletteUpdate {
    Background,
    Primary,
    Secondary
};

namespace LCD {
constexpr uint8_t LINES_PER_FRAME = 154;
constexpr uint8_t DISPLAY_X_RESOLUTION = 160;
constexpr uint8_t DISPLAY_Y_RESOLUTION = 144;
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

namespace FIFO::State {
constexpr uint8_t GET_TILE = 0x00;
constexpr uint8_t GET_TILE_DATA_LOW = 0x01;
constexpr uint8_t GET_TILE_DATA_HIGH = 0x02;
constexpr uint8_t SLEEP = 0x03;
constexpr uint8_t PUSH = 0x04;
}

struct Sprite {
    uint8_t y;
    uint8_t x;
    uint8_t tile;
    uint8_t flags;
    uint8_t hi;
    uint8_t lo;

    bool fetched;

    bool palette() { return flags & 0x10; }
    bool flip_x() { return flags & 0x20; }
    bool flip_y() { return flags & 0x40; }
    bool priority() { return flags & 0x80; }
};

enum class SpriteData {
    Hi,
    Lo
};

class CPU;

class PPU {
public:
    PPU(CPU& cpu);

    void tick(uint32_t cycles);

    void increment_ly_coordinate();

    uint64_t const& current_frame() { return m_current_frame; }
    std::vector<uint32_t> const& display_buffer() { return m_video_buffer; }

    void set_lcd_mode(uint8_t mode) { m_lcd_mode = mode; }

    bool lcd_hblank_interrupts_enabled(void) { return checkbit(m_lcd_status.value(), 3); }
    bool lcd_vblank_interrupts_enabled(void) { return checkbit(m_lcd_status.value(), 4); }
    bool lcd_oam_interrupts_enabled(void) { return checkbit(m_lcd_status.value(), 5); }
    bool lcd_ly_stat_interrupts_enabled(void) { return checkbit(m_lcd_status.value(), 6); }

    bool lcd_bg_and_window_enabled(void) { return checkbit(m_lcd_control.value(), 0); }
    bool lcd_obj_enabled(void) { return checkbit(m_lcd_control.value(), 1); }
    uint8_t lcd_obj_size(void) { return checkbit(m_lcd_control.value(), 2) ? 16 : 8; }
    uint16_t lcd_bg_tile_map_area(void) { return checkbit(m_lcd_control.value(), 3) ? 0x9C00 : 0x9800; }
    uint16_t lcd_bg_window_tile_data_area(void) { return checkbit(m_lcd_control.value(), 4) ? 0x8000 : 0x8800; }
    bool lcd_window_enabled(void) { return checkbit(m_lcd_control.value(), 5); }
    uint16_t lcd_window_tile_map_area(void) { return checkbit(m_lcd_control.value(), 6) ? 0x9C00 : 0x9800; }

    bool is_window_visible()
    {
        return lcd_window_enabled() && m_lcd_window_x.value() >= 0 && m_lcd_window_x.value() <= 166 && m_lcd_window_y.value() < LCD::DISPLAY_Y_RESOLUTION;
    }

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

    uint64_t m_cycles;
    uint64_t m_current_frame;

    std::vector<uint32_t> m_video_buffer;

    std::vector<uint8_t> m_oam_ram;
    std::vector<uint8_t> m_vram;

    uint8_t m_lcd_mode;

    // Pixel FIFO
    std::queue<uint32_t> m_pixel_fifo;
    void reset_pixel_fifo();

    uint8_t m_current_tile;                     // Used to track the tile number in the tile map.
    std::array<uint8_t, 2> m_current_tile_data; // Used to store the pixel data for one row of the fetched tile.
    uint8_t m_position_in_line;                 // Used to track the amount of pixels pushed for the current line.
    uint8_t m_fetched_x;
    uint8_t m_line_x;
    uint8_t m_fifo_x;

    // Fetcher State Machine
    void fetcher_tick();
    void advance_fetcher_state_machine();
    uint8_t m_fetcher_divider;
    uint8_t m_fetcher_state;

    // Sprite
    void overlay_sprite(uint32_t& color, uint8_t const& background_color);
    void add_sprite_from_index(uint8_t index);
    void fetch_visible_sprites(SpriteData offset);
    std::array<Sprite, 10> m_visible_sprites;
    uint8_t m_total_visible_sprites;
    uint8_t m_total_fetched_sprites;

    // Window
    uint8_t m_window_line;
    void fetch_window_tile();

    // Palette
    void update_palette_data(uint8_t value, PaletteUpdate const update);
    std::array<uint32_t, 4> m_pallete;
    std::array<uint32_t, 4> m_sprite_palette_primary;
    std::array<uint32_t, 4> m_sprite_palette_secondary;
    ByteRegister m_palette_data;
};