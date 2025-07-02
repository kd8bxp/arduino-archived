/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _M5_UNIT_JOYSTICK2_HPP_
#define _M5_UNIT_JOYSTICK2_HPP_

#include "Arduino.h"
#include "Wire.h"

#define JOYSTICK2_ADDR                        0x63
#define JOYSTICK2_ADC_VALUE_12BITS_REG        0x00
#define JOYSTICK2_ADC_VALUE_8BITS_REG         0x10
#define JOYSTICK2_BUTTON_REG                  0x20
#define JOYSTICK2_RGB_REG                     0x30
#define JOYSTICK2_ADC_VALUE_CAL_REG           0x40
#define JOYSTICK2_OFFSET_ADC_VALUE_12BITS_REG 0x50
#define JOYSTICK2_OFFSET_ADC_VALUE_8BITS_REG  0x60
#define JOYSTICK2_FIRMWARE_VERSION_REG        0xFE
#define JOYSTICK2_BOOTLOADER_VERSION_REG      0xFC
#define JOYSTICK2_I2C_ADDRESS_REG             0xFF

typedef enum { ADC_8BIT_RESULT = 0, ADC_16BIT_RESULT } adc_mode_t;

/**
 * @brief Unit Joystick2 control API
 */
class M5UnitJoystick2 {
public:
    /**
     * @brief Unit Joystick2 init
     * @param wire I2C Wire number
     * @param addr I2C address
     * @param sda I2C SDA Pin
     * @param scl I2C SCL Pin
     * @param speed I2C clock
     * @return 1 success, 0 false
     */
    bool begin(TwoWire *wire = &Wire, uint8_t addr = JOYSTICK2_ADDR, uint8_t sda = 21, uint8_t scl = 22,
               uint32_t speed = 400000UL);

    /**
     * @brief set Unit Joystick2 I2C address
     * @param addr I2C address
     * @return 1 success, 0 false
     */
    uint8_t set_i2c_address(uint8_t addr);

    /**
     * @brief get Unit Joystick2 I2C address
     * @return I2C address
     */
    uint8_t get_i2c_address(void);

    /**
     * @brief get Unit Joystick2 firmware version
     * @return firmware version
     */
    uint8_t get_firmware_version(void);

    /**
     * @brief get Unit Joystick2 bootloader version
     * @return bootloader version
     */
    uint8_t get_bootloader_version(void);

    /**
     * @brief get Unit Joystick2 x-axis ADC value
     * @param adc_bits ADC_8BIT_RESULT or ADC_16BIT_RESULT
     * @return x-axis ADC value
     */
    uint16_t get_joy_adc_value_x(adc_mode_t adc_bits);

    /**
     * @brief get Unit Joystick2 y-axis ADC value
     * @param adc_bits ADC_8BIT_RESULT or ADC_16BIT_RESULT
     * @return y-axis ADC value
     */
    uint16_t get_joy_adc_value_y(adc_mode_t adc_bits);

    /**
     * @brief get Unit Joystick2 button value
     * @return 0 press, 1 no press
     */
    uint8_t get_button_value(void);

    /**
     * @brief set Unit Joystick2 rgb color
     * @param color rgb color
     */
    void set_rgb_color(uint32_t color);

    /**
     * @brief get Unit Joystick2 rgb color
     * @return rgb color
     */
    uint32_t get_rgb_color(void);

    /**
     * @brief get Unit Joystick2 mapped cal value
     * @param x_neg_min pointer of x-axis negative minimum value
     * @param x_neg_max pointer of x-axis negative maximum value
     * @param x_pos_min pointer of x-axis positive minimum value
     * @param x_pos_max pointer of x-axis positive maximum value
     * @param y_neg_min pointer of y-axis negative minimum value
     * @param y_neg_max pointer of y-axis negative maximum value
     * @param y_pos_min pointer of y-axis positive minimum value
     * @param y_pos_max pointer of y-axis positive maximum value
     */
    void get_joy_adc_value_cal(uint16_t *x_neg_min, uint16_t *x_neg_max, uint16_t *x_pos_min, uint16_t *x_pos_max,
                               uint16_t *y_neg_min, uint16_t *y_neg_max, uint16_t *y_pos_min, uint16_t *y_pos_max);

    /**
     * @brief set Unit Joystick2 mapped cal value
     * @param x_neg_min x-axis negative minimum value
     * @param x_neg_max x-axis negative maximum value
     * @param x_pos_min x-axis positive minimum value
     * @param x_pos_max x-axis positive maximum value
     * @param y_neg_min y-axis negative minimum value
     * @param y_neg_max y-axis negative maximum value
     * @param y_pos_min y-axis positive minimum value
     * @param y_pos_max y-axis positive maximum value
     */
    void set_joy_adc_value_cal(uint16_t x_neg_min, uint16_t x_neg_max, uint16_t x_pos_min, uint16_t x_pos_max,
                               uint16_t y_neg_min, uint16_t y_neg_max, uint16_t y_pos_min, uint16_t y_pos_max);

    /**
     * @brief get Unit Joystick2 x-axis 12bits mapped value
     * @return x-axis 12bits mapped value
     */
    int16_t get_joy_adc_12bits_offset_value_x(void);

    /**
     * @brief get Unit Joystick2 y-axis 12bits mapped value
     * @return y-axis 12bits mapped value
     */
    int16_t get_joy_adc_12bits_offset_value_y(void);

    /**
     * @brief get Unit Joystick2 x-axis 8bits mapped value
     * @return x-axis 8bits mapped value
     */
    int8_t get_joy_adc_8bits_offset_value_x(void);

    /**
     * @brief get Unit Joystick2 y-axis 8bits mapped value
     * @return y-axis 8bits mapped value
     */
    int8_t get_joy_adc_8bits_offset_value_y(void);

    /**
     * @brief get Unit Joystick2 x-axis and y-axis 16bits ADC value
     * @param adc_x pointer of x-axis ADC value
     * @param adc_y pointer of y-axis ADC value
     */
    void get_joy_adc_16bits_value_xy(uint16_t *adc_x, uint16_t *adc_y);

    /**
     * @brief get Unit Joystick2 x-axis and y-axis 8bits ADC value
     * @param adc_x pointer of x-axis ADC value
     * @param adc_y pointer of y-axis ADC value
     */
    void get_joy_adc_8bits_value_xy(uint8_t *adc_x, uint8_t *adc_y);

private:
    /**
     * @brief I2C write
     * @param addr I2C device address
     * @param reg I2C reg address
     * @param buffer pointer of write buffer
     * @param length write length
     */
    void write_bytes(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t length);

    /**
     * @brief I2C read
     * @param addr I2C device address
     * @param reg I2C reg address
     * @param buffer pointer of read buffer
     * @param length read length
     */
    void read_bytes(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t length);
    uint8_t _addr;
    TwoWire *_wire;
    uint8_t _scl;
    uint8_t _sda;
    uint32_t _speed;
};

#endif
