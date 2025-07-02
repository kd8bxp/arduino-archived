#pragma once

#include <Arduino.h>
#include "src/M5StickC/M5StickC.h"

#include "joystick-position.h"

class JoystickHat
{
    static constexpr const uint8_t i2c_address = 0x38;
    static constexpr const uint8_t reg_read_3bytes = 0x02;
    static constexpr const uint8_t request_read_3bytes = 3;

    TwoWire* const _wire;

public:
    JoystickHat(TwoWire* const wire = &Wire) : _wire(wire)
    {

    }

    void begin(const uint8_t sda = 0, const uint8_t scl = 26);

    const JoystickPosition read();
};
