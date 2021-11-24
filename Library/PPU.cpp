#include <PPU.h>
#include <Utils/Address.h>
#include <Utils/Format.h>

PPU::PPU(CPU& cpu)
    : m_cpu(cpu)
{
}

void PPU::tick(uint64_t cycles)
{
    m_cycles += cycles;

    switch (m_lcd_mode) {
    case LCD::Mode::OAM: {
        if (m_cycles >= LCD::Cycles::OAM) {
            m_cycles -= LCD::Cycles::OAM;
            set_lcd_mode(LCD::Mode::VRAM);
        }
        break;
    }
    case LCD::Mode::VRAM: {
        if (m_cycles >= LCD::Cycles::VRAM) {
            m_cycles -= LCD::Cycles::VRAM;
            set_lcd_mode(LCD::Mode::HBLANK);

            if (lcd_hblank_interrupts_enabled())
                m_cpu.request_interrupt(Interrupts::LCD_STATUS);
        }
        break;
    }
    case LCD::Mode::VBLANK: {
        if (m_cycles >= LCD::Cycles::VBLANK) {
            m_cycles -= LCD::Cycles::VBLANK;

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
            m_cycles -= LCD::Cycles::HBLANK;

            increment_ly_coordinate();

            if (m_lcd_y_cord.value() >= LCD::Y_WINDOW_WIDTH) {
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