#include <PPU.h>
#include <Utils/Address.h>
#include <Utils/Format.h>

PPU::PPU(CPU& cpu)
    : m_cpu(cpu)
    , m_cycles(0)
    , m_current_frame(0)
    , m_fetcher_divider(2)
    , m_fetcher_state(FIFO::State::GET_TILE)
    , m_lcd_mode(LCD::Mode::OAM)
    , m_total_visible_sprites(0)
    , m_total_fetched_sprites(0)
    , m_position_in_line(0)
    , m_fetched_x(0)
    , m_line_x(0)
{
    m_video_buffer = std::vector<uint32_t>(LCD::DISPLAY_Y_RESOLUTION * LCD::DISPLAY_X_RESOLUTION * sizeof(uint32_t));

    m_oam_ram = std::vector<uint8_t>(0xA0, 0);
    m_vram = std::vector<uint8_t>(0x2000, 0);

    m_lcd_control.set(0x91);
    m_palette_data.set(0xFC);

    for (int i = 0; i < 4; i++) {
        m_pallete[i] = DEFAULT_COLORS[i];
        m_sprite_palette_primary[i] = DEFAULT_COLORS[i];
        m_sprite_palette_secondary[i] = DEFAULT_COLORS[i];
    }
}

void PPU::tick(uint32_t cycles)
{
    m_cycles += cycles;

    switch (m_lcd_mode) {
    case LCD::Mode::OAM: {
        if (m_cycles >= LCD::Cycles::OAM) {
            m_cycles = 0;
            set_lcd_mode(LCD::Mode::VRAM);

            m_fetcher_state = FIFO::State::GET_TILE;
            m_position_in_line = 0;
            m_fetched_x = 0;
            m_line_x = 0;
            m_fifo_x = 0;
        }

        if (m_cycles == 0) {
            m_visible_sprites = { 0 };
            m_total_visible_sprites = 0;

            for (int oam_search_index = 0; oam_search_index < 40; oam_search_index++) {
                add_sprite_from_index(oam_search_index * 4);
            }
        }

        break;
    }
    case LCD::Mode::VRAM: {
        fetcher_tick();

        if (m_position_in_line >= LCD::DISPLAY_X_RESOLUTION) {
            reset_pixel_fifo();

            set_lcd_mode(LCD::Mode::HBLANK);

            if (lcd_hblank_interrupts_enabled())
                m_cpu.request_interrupt(Interrupts::LCD_STATUS);
        }
        break;
    }
    case LCD::Mode::VBLANK: {
        if (m_cycles >= LCD::Cycles::VBLANK) {
            m_cycles = 0;

            increment_ly_coordinate();

            if (m_lcd_y_cord.value() >= LCD::LINES_PER_FRAME) {
                set_lcd_mode(LCD::Mode::OAM);
                m_lcd_y_cord.set(0);

                if (lcd_oam_interrupts_enabled())
                    m_cpu.request_interrupt(Interrupts::LCD_STATUS);
            }
        }
        break;
    }
    case LCD::Mode::HBLANK: {
        if (m_cycles >= LCD::Cycles::HBLANK) {
            m_cycles = 0;

            increment_ly_coordinate();

            if (m_lcd_y_cord.value() >= LCD::DISPLAY_Y_RESOLUTION) {
                set_lcd_mode(LCD::Mode::VBLANK);

                m_cpu.request_interrupt(Interrupts::VBLANK);

                if (lcd_vblank_interrupts_enabled())
                    m_cpu.request_interrupt(Interrupts::LCD_STATUS);

                m_current_frame++;
            } else {
                set_lcd_mode(LCD::Mode::OAM);
                if (lcd_oam_interrupts_enabled())
                    m_cpu.request_interrupt(Interrupts::LCD_STATUS);
            }
        }
        break;
    }
    }
}

void PPU::add_sprite_from_index(uint8_t index)
{
    // There can only be 10 total loaded sprites
    if (m_total_visible_sprites >= 10)
        return;

    uint8_t current_y = m_lcd_y_cord.value();
    uint8_t height = lcd_obj_size();

    uint8_t y = m_oam_ram[index] - 16;
    uint8_t x = m_oam_ram[index + 1];
    uint8_t tile = m_oam_ram[index + 2];
    uint8_t flags = m_oam_ram[index + 3];

    Sprite object = { y, x, tile, flags, 0, 0, false };

    // If there is not an X value, then the sprite is not visible
    if (!object.x)
        return;

    // Does this sprite intercept with the current scanline?
    if (object.y <= current_y && (object.y + height) > current_y)
        m_visible_sprites[m_total_visible_sprites++] = object;

    // Sprites need to be sorted for certain games to render properly
    std::sort(m_visible_sprites.begin(), m_visible_sprites.end(),
        [](Sprite const& a, Sprite const& b) {
            return a.x > b.x;
        });
}

void PPU::fetch_visible_sprites(SpriteData offset)
{
    uint8_t current_y = m_lcd_y_cord.value();

    for (auto& object : m_visible_sprites) {
        if (!object.fetched)
            continue;

        uint8_t tile_y = (current_y - object.y) * 2;

        /* Flip Y */
        if (object.flip_y()) {
            tile_y ^= lcd_obj_size() ? 0xF : 7;
        }

        uint16_t line_address = (lcd_obj_size() ? object.tile : object.tile & 0xFE) * 0x10 + tile_y;

        if (offset == SpriteData::Hi)
            object.hi = vram_read(line_address);
        if (offset == SpriteData::Lo)
            object.lo = vram_read(line_address + 1);
    }
}

void PPU::overlay_sprite(uint32_t& color, uint8_t const& background_color)
{
    for (auto& object : m_visible_sprites) {
        if (!object.fetched)
            continue;

        int sprite_x = (object.x - 8) + (m_lcd_scroll_x.value() % 8);

        if (sprite_x + 8 < m_fifo_x)
            continue;

        int offset = m_fifo_x - sprite_x;

        if (offset < 0 || offset > 7)
            continue;

        uint8_t pixel = (7 - offset);

        uint8_t flip_xor = object.flip_x() ? 0x7 : 0;
        pixel ^= flip_xor;

        uint8_t hi = !!(object.hi & (1 << pixel));
        uint8_t lo = !!(object.lo & (1 << pixel)) << 1;

        if (!(hi | lo))
            continue;

        if (!object.priority() || background_color == 0) {
            color = (object.palette()) ? m_sprite_palette_secondary[hi | lo] : m_sprite_palette_primary[hi | lo];
            break;
        }
        object.fetched = false;
    }
}

void PPU::increment_ly_coordinate()
{
    m_lcd_y_cord.increment();

    if (m_lcd_y_cord.value() == m_lcd_y_compare.value()) {
        m_lcd_status.set_bit(2, true);

        if (lcd_ly_stat_interrupts_enabled())
            m_cpu.request_interrupt(Interrupts::LCD_STATUS);
    } else {
        m_lcd_status.set_bit(2, false);
    }
}

void PPU::fetcher_tick()
{
    // The fetcher runs at half the speed of the PPU, so we need to handle
    // the fetcher every 2 clock cycles.
    if (--m_fetcher_divider == 0) {
        m_fetcher_divider = 2;
        advance_fetcher_state_machine();
    }

    if (m_pixel_fifo.size() > 8) {
        uint32_t data = m_pixel_fifo.front();
        m_pixel_fifo.pop();

        if (m_line_x >= (m_lcd_scroll_x.value() % 8)) {
            int index = m_position_in_line + (m_lcd_y_cord.value() * LCD::DISPLAY_X_RESOLUTION);
            m_video_buffer[index] = data;

            m_position_in_line++;
        }

        m_line_x++;
    }
}

void PPU::advance_fetcher_state_machine()
{
    switch (m_fetcher_state) {
    case FIFO::State::GET_TILE: {
        if (lcd_bg_and_window_enabled()) {
            uint16_t map = lcd_bg_tile_map_area();
            uint8_t y = (m_lcd_y_cord.value() + m_lcd_scroll_y.value());
            uint8_t x = (m_fetched_x + m_lcd_scroll_x.value());

            m_current_tile = vram_read(map + (x / 8) + (((y / 8)) * 32));

            if (lcd_bg_window_tile_data_area() == 0x8800) {
                m_current_tile += 128;
            }
        }

        m_total_fetched_sprites = 0;
        if (lcd_obj_enabled() && m_total_visible_sprites != 0) {
            for (auto& object : m_visible_sprites) {
                int sprite_x = (object.x - 8) + (m_lcd_scroll_x.value() % 8);

                if ((sprite_x >= m_fetched_x && sprite_x < m_fetched_x + 8) || ((sprite_x + 8) >= m_fetched_x && (sprite_x + 8) < m_fetched_x + 8)) {
                    object.fetched = true;
                    m_total_fetched_sprites++;
                }

                if (m_total_fetched_sprites >= 3)
                    break;
            }
        }

        m_fetcher_state = FIFO::State::GET_TILE_DATA_LOW;
        m_fetched_x += 8;
        break;
    }

    case FIFO::State::GET_TILE_DATA_LOW: {
        uint16_t map = lcd_bg_window_tile_data_area();
        uint8_t y = ((m_lcd_y_cord.value() + m_lcd_scroll_y.value()) % 8) * 2;

        m_current_tile_data[0] = vram_read(map + (m_current_tile * 16) + y);

        fetch_visible_sprites(SpriteData::Lo);

        m_fetcher_state = FIFO::State::GET_TILE_DATA_HIGH;
        break;
    }

    case FIFO::State::GET_TILE_DATA_HIGH: {
        uint16_t map = lcd_bg_window_tile_data_area();
        uint8_t y = ((m_lcd_y_cord.value() + m_lcd_scroll_y.value()) % 8) * 2;

        m_current_tile_data[1] = vram_read(map + (m_current_tile * 16) + y + 1);

        fetch_visible_sprites(SpriteData::Hi);

        m_fetcher_state = FIFO::State::SLEEP;
        break;
    }

    case FIFO::State::SLEEP: {
        m_fetcher_state = FIFO::State::PUSH;
        break;
    }

    case FIFO::State::PUSH: {
        if (m_pixel_fifo.size() > 8)
            return;

        for (int pixel = 7; pixel >= 0; pixel--) {
            uint8_t hi = !!(m_current_tile_data[0] & (1 << pixel));
            uint8_t lo = !!(m_current_tile_data[1] & (1 << pixel)) << 1;

            uint32_t color = m_pallete[hi | lo];

            if (!lcd_bg_and_window_enabled())
                color = m_pallete[0];

            if (lcd_obj_enabled() && m_total_fetched_sprites) {
                overlay_sprite(color, hi | lo);
            }

            m_pixel_fifo.push(color);
            m_fifo_x++;
        }

        m_fetcher_state = FIFO::State::GET_TILE;
        break;
    }
    }
}

void PPU::reset_pixel_fifo()
{
    std::queue<uint32_t> reset;
    std::swap(m_pixel_fifo, reset);
}

uint8_t PPU::lcd_read(uint16_t const& address)
{
    switch (address) {
    case 0xFF40:
        return m_lcd_control.value();
    case 0xFF41:
        return m_lcd_status.value();
    case 0xFF42:
        return m_lcd_scroll_y.value();
    case 0xFF43:
        return m_lcd_scroll_x.value();
    case 0xFF44:
        return m_lcd_y_cord.value();
    case 0xFF45:
        return m_lcd_y_compare.value();
    case 0xFF47:
        return m_palette_data.value();
    case 0xFF48:
        return m_palette_data.value();
    case 0xFF49:
        return m_palette_data.value();
    case 0xFF4A:
        return m_lcd_window_y.value();
    case 0xFF4B:
        return m_lcd_window_x.value();
    default:
        outln("LCD read at address 0x:{:2X} not yet implemented ", address);
        return 0x00;
    }
}

void PPU::lcd_write(uint16_t const address, uint8_t const value)
{
    switch (address) {
    case 0xFF40:
        m_lcd_control.set(value);
        return;
    case 0xFF41:
        m_lcd_status.set(value);
        return;
    case 0xFF42:
        m_lcd_scroll_y.set(value);
        return;
    case 0xFF43:
        m_lcd_scroll_x.set(value);
        return;
    case 0xFF44:
        m_lcd_y_cord.set(value);
        return;
    case 0xFF45:
        m_lcd_y_compare.set(value);
        return;
    case 0xFF47:
        update_palette_data(value, PaletteUpdate::Background);
        return;
    case 0xFF48:
        update_palette_data(value, PaletteUpdate::Primary);
        return;
    case 0xFF49:
        update_palette_data(value, PaletteUpdate::Secondary);
        return;
    case 0xFF4A:
        m_lcd_window_y.set(value);
        return;
    case 0xFF4B:
        m_lcd_window_x.set(value);
        return;
    default:
        outln("LCD write at address 0x:{:2X} not yet implemented ", address);
        return;
    }
}

uint8_t PPU::vram_read(uint16_t const& address)
{
    if (address >= 0x8000 && address <= 0x9FFF)
        return m_vram[address - 0x8000];

    return m_vram[address];
}

void PPU::vram_write(uint16_t const address, uint8_t const value)
{
    if (address >= 0x8000 && address <= 0x9FFF)
        m_vram[address - 0x8000] = value;

    m_vram[address] = value;
}

uint8_t PPU::oam_read(uint16_t const& address)
{
    if (address >= 0xFE00 && address <= 0xFE9F)
        return m_oam_ram[address - 0xFE00];

    return m_oam_ram[address];
}

void PPU::oam_write(uint16_t const address, uint8_t const value)
{
    if (address >= 0xFE00 && address <= 0xFE9F)
        m_oam_ram[address - 0xFE00] = value;
    m_oam_ram[address] = value;
}

void PPU::update_palette_data(uint8_t value, PaletteUpdate const update)
{
    uint32_t* palette = m_pallete.data();

    if (update == PaletteUpdate::Primary)
        palette = m_sprite_palette_primary.data();
    if (update == PaletteUpdate::Secondary)
        palette = m_sprite_palette_secondary.data();

    palette[0] = DEFAULT_COLORS[value & 0x03];
    palette[1] = DEFAULT_COLORS[(value >> 2) & 0x03];
    palette[2] = DEFAULT_COLORS[(value >> 4) & 0x03];
    palette[3] = DEFAULT_COLORS[(value >> 6) & 0x03];
}