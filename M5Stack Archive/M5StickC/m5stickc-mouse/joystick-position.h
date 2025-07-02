#pragma once

#include <Arduino.h>

class JoystickPosition
{
    const int8_t _x;
    const int8_t _y;
    const bool _is_pressed;

public:
    JoystickPosition(const int8_t x, const int8_t y, const bool is_pressed) : _x(x), _y(y), _is_pressed(is_pressed)
    {

    }

    static const JoystickPosition Zero()
    {
        return JoystickPosition(0, 0, false);
    }

    const int8_t getX() const
    {
        return _x;
    }

    const int8_t getY() const
    {
        return _y;
    }

    const bool getIsPressed() const
    {
        return _is_pressed;
    }
};