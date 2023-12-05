/**
 * @file PCA9554.h
 * @author David Baka
 * @brief
 * @version 0.3.2
 * @date 2023-10-02
 *  
 * @copyright Copyright (c) 2023 David Baka
 * 
 */

#ifndef PCA9554_h
#define PCA9554_h

#include <Arduino.h>
#include <Wire.h>
#include "PCA9554_defs.h"

class PCA9554 {
	public :

        /**
         * @brief Constructor for PCA9554 lib
         * @param slaveAddress I2C address of the expander
         * @param wire TwoWire instance for I2C bus
        */
		PCA9554(byte _slaveAddress, TwoWire *_wire = &Wire);

        /**
         * @brief configure a single port for input/output
         * @param mode 0 - output 1 - input (macros available)
         * @param port number of the I/O port
         * @return success: 0, other: I2C error
        */
        pca9554_return_t configSinglePort(bool mode, pca9554_port_num_t port);

        /**
         * @brief configure multiple ports for input/output
         * @param mode 0 - output 1 - input (macros available)
         * @param mask (optional) mask representing the ports to be configured (0xFF for all ports - default)
         * @return success: 0, other: I2C error
        */
        pca9554_return_t configMultiplePorts(bool mode, pca9554_mask_t = 0xFF );

        /**
         * @brief read the contents of the config register
         * @param value pointer to variable that stores the result
         * @return success: 0, other: I2C error
        */
        pca9554_return_t readConfigRegister(pca9554_data_t *value);

        /**
         * @brief invert the value of a single input port
         * @note this action only affects the value in the input register
         * @param inverted 0 - normal 1 - inverted (macros available)
         * @param port number of the I/O port
         * @return success: 0, other: I2C error
        */
        pca9554_return_t invertSinglePort(bool inverted, pca9554_port_num_t port);

        /**
         * @brief invert the value of multuple input ports
         * @note this action only affects the value in the input register
         * @param inverted 0 - normal 1 - inverted (macros available)
         * @param mask (optional) mask representing the ports to be configured (0xFF for all ports - default)
         * @return success: 0, other: I2C error
        */
        pca9554_return_t invertMultiplePorts(bool inverted, pca9554_mask_t mask = 0xFF );

        /**
         * @brief read input register
         * @note values show state of port regardless if it is configured as read or write port
         * @param result pointer to variable that stores the result
         * @param mask (optional) mask representing the ports to be read (0xFF for all ports - default)
         * @return success: 0, other: I2C error
        */
        pca9554_return_t readInputRegister(pca9554_data_t* result, pca9554_mask_t mask = 0xFF);

        /**
         * @brief write logic level to single port
         * @note this action has no effect on ports configured as inputs
         * @param level 0 - logic low 1 - logic high (macros available)
         * @param port number of the I/O port
         * @return success: 0, other: I2C error
        */
        pca9554_return_t writeSinglePort(bool level, pca9554_port_num_t port);

        /**
         * @brief write logic level to multiple ports
         * @note this action has no effect on ports configured as inputs
         * @param mode 0 - logic low 1 - logic high (macros available)
         * @param mask (optional) mask representing the ports to be written (0xFF for all ports - default)
         * @return success: 0, other: I2C error
        */
        pca9554_return_t writeMultiplePorts(bool level, pca9554_mask_t mask = 0xFF);

        /**
         * @brief read commanded outputs
         * @note this action has no effect on ports configured as inputs
         * @note while read input register shows actual physical state of I/O, reading output register returns the commanded value on given port
         * @param result pointer to variable that stores the result
         * @param mask (optional) mask representing the ports to be written (0xFF for all ports - default)
         * @return success: 0, other: I2C error
        */
        pca9554_return_t readOutputRegister(pca9554_data_t* result, pca9554_mask_t mask = 0xFF);

        /**
         * @brief sets the contents of the output register to 0
         * @return success: 0, other: I2C error
        */
        pca9554_return_t clearOutputs();
		
	private:
        /**
         * @brief private method to read single register on I2C bus
         * @param reg register to read
         * @param value pointer to variable hold the data
         * @return success: 0, other: I2C error
        */
        pca9554_return_t readRegister(pca9554_reg_t reg, pca9554_data_t* value);

        /**
         * @brief private method to read single register on I2C bus
         * @param reg register to write
         * @param value data to be written
         * @return success: 0, other: I2C error
        */
        pca9554_return_t writeRegister(pca9554_reg_t reg, pca9554_data_t value);
		int slaveAddress;
        TwoWire*  wire;
};

#endif