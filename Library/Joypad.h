#pragma once

#include <Register.h>
#include <Utils/Bitwise.h>

#include <stdint.h>

enum class ActionButtons {
    A,
    B,
    Select,
    Start
};

enum class DirectionButtons {
    Right,
    Left,
    Up,
    Down
};

class Joypad {
public:
    Joypad();

    uint8_t read();
    void write(uint8_t const& value);
    void set_button_state(ActionButtons const button, bool pressed);
    void set_button_state(DirectionButtons const button, bool pressed);

    bool actions_selected() { return !(m_select & 0x20); }
    bool direction_selected() { return !(m_select & 0x10); }

private:
    uint8_t m_select;
    uint8_t m_action_buttons;
    uint8_t m_direction_buttons;
};