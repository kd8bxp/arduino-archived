/*
 * start rewrite from:
 * https://github.com/arduino/ArduinoCore-mbed/blob/master/libraries/SPI/SPI.cpp
 */
#ifdef ARDUINO_ARCH_NRF52840

#ifndef _ARDUINO_NRFXSPI_H_
#define _ARDUINO_NRFXSPI_H_

#include <SPI.h>

#include "../Arduino_DataBus.h"

#define NRFXSPI_MAX_PIXELS_AT_ONCE 32

class Arduino_NRFXSPI : public Arduino_DataBus
{
public:
  Arduino_NRFXSPI(int8_t dc, int8_t cs = GFX_NOT_DEFINED, int8_t sck = GFX_NOT_DEFINED, int8_t mosi = GFX_NOT_DEFINED, int8_t miso = GFX_NOT_DEFINED); // Constructor

  bool begin(int32_t speed = GFX_NOT_DEFINED, int8_t dataMode = GFX_NOT_DEFINED) override;
  void beginWrite() override;
  void endWrite() override;
  void writeCommand(uint8_t) override;
  void writeCommand16(uint16_t) override;
  void write(uint8_t) override;
  void write16(uint16_t) override;
  void writeRepeat(uint16_t p, uint32_t len) override;
  void writePixels(uint16_t *data, uint32_t len) override;

  void writeC8D8(uint8_t c, uint8_t d) override;
  void writeC8D16(uint8_t c, uint16_t d) override;
  void writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2) override;
  void writeBytes(uint8_t *data, uint32_t len) override;

private:
  INLINE void WRITE(uint8_t d);
  INLINE void WRITE16(uint16_t d);
  INLINE void WRITEBUF(uint8_t *buf, size_t count);
  INLINE void DC_HIGH(void);
  INLINE void DC_LOW(void);
  INLINE void CS_HIGH(void);
  INLINE void CS_LOW(void);

  int8_t _dc, _cs;
  int8_t _sck, _mosi, _miso;

  PORTreg_t _csPortSet; ///< PORT register for chip select SET
  PORTreg_t _csPortClr; ///< PORT register for chip select CLEAR
  PORTreg_t _dcPortSet; ///< PORT register for data/command SET
  PORTreg_t _dcPortClr; ///< PORT register for data/command CLEAR
  ARDUINOGFX_PORT_t _csPinMask; ///< Bitmask for chip select
  ARDUINOGFX_PORT_t _dcPinMask; ///< Bitmask for data/command

  nrfx_spi_config_t _nrfxSpiConfig = NRFX_SPI_DEFAULT_CONFIG;
  nrfx_spi_t _nrfxSpi = NRFX_SPI_INSTANCE(0);

  union
  {
    uint8_t _buffer[NRFXSPI_MAX_PIXELS_AT_ONCE * 2] = {0};
    uint16_t _buffer16[NRFXSPI_MAX_PIXELS_AT_ONCE];
    uint32_t _buffer32[NRFXSPI_MAX_PIXELS_AT_ONCE / 2];
  };
};

#endif // _ARDUINO_NRFXSPI_H_

#endif // #ifdef ARDUINO_ARCH_NRF52840
