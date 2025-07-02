/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "Arduino.h"
#include "Wire.h"
#include "m5_unit_joystick2.hpp"

M5UnitJoystick2 joystick2;

void setup()
{
    Serial.begin(115200);
    joystick2.begin(&Wire, JOYSTICK2_ADDR, 32, 33);
    joystick2.set_rgb_color(0x00ff00);
}

void loop()
{
    uint8_t bootloader_ver, firmware_ver;
    uint8_t adc_x, adc_y;
    bootloader_ver = joystick2.get_bootloader_version();
    firmware_ver   = joystick2.get_firmware_version();
    joystick2.get_joy_adc_8bits_value_xy(&adc_x, &adc_y);
    Serial.printf("bootloader ver:%d, firmware ver:%d, x adc:%d, y adc:%d\n", bootloader_ver, firmware_ver, adc_x,
                  adc_y);
    delay(100);
}
