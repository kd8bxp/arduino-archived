/**
 * @file PCA9554.cpp
 * @author David Baka
 * @brief
 * @version 0.3.2
 * @date 2023-10-02
 *  
 * @copyright Copyright (c) 2023 David Baka
 * 
 */

#include "PCA9554.h"

PCA9554::PCA9554(byte _slaveAddress, TwoWire *_wire) {
	slaveAddress = _slaveAddress;
    wire = _wire;
}

pca9554_return_t PCA9554::configSinglePort(bool mode, pca9554_port_num_t port) {
    pca9554_data_t configReg;
    pca9554_return_t ret;

    ret = readRegister(CONFIG_REG, &configReg);
    if(ret != PCA9554_I2C_SUCCESS)
        return ret;

    configReg &= ~(1 << port);     // clear bit
    configReg |= mode << port;     // set bit

    ret = writeRegister(CONFIG_REG, configReg);

    return ret;
}

pca9554_return_t PCA9554::configMultiplePorts(bool mode, pca9554_mask_t mask) {
    pca9554_data_t configReg;
    pca9554_return_t ret;

    ret = readRegister(CONFIG_REG, &configReg);
    if(ret != PCA9554_I2C_SUCCESS)
        return ret;

    pca9554_data_t val = mode?0xFF:0x00;   // fill a byte with the value of output mode
    configReg &= ~mask;            // clear bits covered by the mask
    configReg |= (val & mask);     // set bits covered by the mask

    ret = writeRegister(CONFIG_REG, configReg);

    return ret;
}

pca9554_return_t PCA9554::readConfigRegister(pca9554_data_t *value) {
    return readRegister(CONFIG_REG, value);
}

pca9554_return_t PCA9554::invertSinglePort(bool inverted, pca9554_port_num_t port) {
    pca9554_data_t inversionReg;
    pca9554_return_t ret;

    ret = readRegister(POL_INV_REG, &inversionReg);
    if(ret != PCA9554_I2C_SUCCESS)
        return ret;

    inversionReg &= ~(1 << port);
    inversionReg |= inverted << port;

    ret = writeRegister(POL_INV_REG, inversionReg);

    return ret;
}

pca9554_return_t PCA9554::invertMultiplePorts(bool inverted, pca9554_mask_t mask) {
    pca9554_data_t inversionReg;
    pca9554_return_t ret;

    ret = readRegister(POL_INV_REG, &inversionReg);
    if(ret != PCA9554_I2C_SUCCESS)
        return ret;

    pca9554_data_t val = inverted?0xFF:0x00;
    inversionReg &= ~mask;
    inversionReg |= (val & mask);

    ret = writeRegister(POL_INV_REG, inversionReg);

    return ret;
}

pca9554_return_t PCA9554::readInputRegister(pca9554_data_t* result, pca9554_mask_t mask) {
    pca9554_data_t val;
    pca9554_return_t ret;

    ret = readRegister(INPUT_REG, &val);

    val &= mask;
    *result = val;

    return ret;
}

pca9554_return_t PCA9554::writeSinglePort(bool level, pca9554_port_num_t port) {
    pca9554_data_t outputReg;
    pca9554_return_t ret;

    ret = readRegister(OUTPUT_REG, &outputReg);
    if(ret != PCA9554_I2C_SUCCESS)
        return ret;


    outputReg &= ~(1 << port);     // clear bit
    outputReg |= level << port;

    ret = writeRegister(OUTPUT_REG, outputReg);

    return ret;
}

pca9554_return_t PCA9554::writeMultiplePorts(bool level, pca9554_mask_t mask) {
    pca9554_data_t outputReg;
    pca9554_return_t ret;

    ret = readRegister(OUTPUT_REG, &outputReg);
    if(ret != PCA9554_I2C_SUCCESS)
        return ret;

    pca9554_data_t val = level?0xFF:0x00;
    outputReg &= ~mask;
    outputReg |= (val & mask);

    ret = writeRegister(OUTPUT_REG, outputReg);

    return ret;
}

pca9554_return_t PCA9554::readOutputRegister(pca9554_data_t* result, pca9554_mask_t mask) {
    pca9554_data_t val;
    pca9554_return_t ret;

    ret = readRegister(OUTPUT_REG, &val);

    val &= mask;
    *result = val;

    return ret;
}

pca9554_return_t PCA9554::clearOutputs() {
    return writeRegister(OUTPUT_REG, 0x00);
}

// ================= PRIVATE METHODS =================

pca9554_return_t PCA9554::readRegister(pca9554_reg_t reg, pca9554_data_t* value) {
    uint8_t ret;
    wire->beginTransmission(slaveAddress);
    wire->write(reg);
    ret = wire->endTransmission(false);

    delay(2);

    wire->requestFrom(slaveAddress, 1, (int)true);
    wire->readBytes(value, 1);

    return (pca9554_return_t)ret;
}

pca9554_return_t PCA9554::writeRegister(pca9554_reg_t reg, pca9554_data_t value) {
    uint8_t ret;
    wire->beginTransmission(slaveAddress);
    wire->write(reg);
    wire->write(value);

    ret = wire->endTransmission(true);

    return (pca9554_return_t)ret;
}
