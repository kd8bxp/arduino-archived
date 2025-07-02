#include "joystick-hat.h"

void JoystickHat::begin(const uint8_t sda, const uint8_t scl)
{
    _wire->begin(sda, scl);
}

const JoystickPosition JoystickHat::read()
{
    _wire->beginTransmission(i2c_address);
    _wire->write(reg_read_3bytes);
    _wire->endTransmission();

    _wire->requestFrom(i2c_address, request_read_3bytes);

    if (_wire->available()) {
        int8_t x = _wire->read();
        int8_t y = _wire->read();
        int8_t is_not_pressed = _wire->read();

        return JoystickPosition(x, y, is_not_pressed == 0);
    }

    return JoystickPosition::Zero();
}