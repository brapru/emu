#include <Joypad.h>
#include <Utils/Format.h>

Joypad::Joypad()
    : m_select(0x0)
    , m_action_buttons(0x0)
    , m_direction_buttons(0x0)
{
}

uint8_t Joypad::read()
{
    uint8_t button = 0x00;

    if (actions_selected())
        button |= m_action_buttons;

    if (direction_selected())
        button |= m_direction_buttons;

    return ((m_select | 0x0F) ^ button);
};

void Joypad::write(uint8_t const& value)
{
    m_select = value & 0x30;
};

void Joypad::set_button_state(ActionButtons const button, bool pressed)
{
    uint8_t bit = static_cast<uint8_t>(button);
    pressed ? bitset(m_action_buttons, bit) : bitclear(m_action_buttons, bit);
};

void Joypad::set_button_state(DirectionButtons const button, bool pressed)
{
    uint8_t bit = static_cast<uint8_t>(button);
    pressed ? bitset(m_direction_buttons, bit) : bitclear(m_direction_buttons, bit);
};