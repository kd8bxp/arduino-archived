/**
 * @file PCA9554_defs.h
 * @author David Baka
 * @brief
 * @version 0.3.1
 * @date 2023-10-02
 *  
 * @copyright Copyright (c) 2023 David Baka
 * 
 */

#ifndef __PCA9554_DEFS__
#define __PCA9554_DEFS__

typedef enum : uint8_t{
    PCA9554_I2C_SUCCESS = 0,
    PCA9554_I2C_DATA_LONG,
    PCA9554_I2C_ADDR_NACK,
    PCA9554_I2C_DATA_NACK,
    PCA9554_I2C_OTHER_ERR
} pca9554_return_t;

typedef enum : uint8_t{
    PORT0 = 0,
    PORT1,
    PORT2,
    PORT3,
    PORT4,
    PORT5,
    PORT6,
    PORT7,
} pca9554_port_num_t;

typedef uint8_t pca9554_reg_t;
typedef uint8_t pca9554_data_t;
typedef uint8_t pca9554_mask_t;

#define PORTS_ALL 0xFF

//PCA9554 Command Byte
#define INPUT_REG	0x00
#define OUTPUT_REG	0x01
#define POL_INV_REG	0x02
#define CONFIG_REG	0x03

#define PCA9554_OUTPUT      0x00
#define PCA9554_INPUT       0x01

#define PCA9554_LOW         0x00
#define PCA9554_HIGH        0x01

#define PCA9554_NO_INVERT   0x00
#define PCA9554_INVERT      0x01

#endif