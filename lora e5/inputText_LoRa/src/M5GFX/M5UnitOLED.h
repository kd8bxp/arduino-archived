#ifndef __M5GFX_M5UNITOLED__
#define __M5GFX_M5UNITOLED__

// If you want to use a set of functions to handle SD/SPIFFS/HTTP,
//  please include <SD.h>,<SPIFFS.h>,<HTTPClient.h> before <M5GFX.h>
// #include <SD.h>
// #include <SPIFFS.h>
// #include <HTTPClient.h>

#if defined (ESP_PLATFORM)
 #include <sdkconfig.h>
#else
 #include "lgfx/v1/platforms/sdl/Panel_sdl.hpp"
#endif

#include "lgfx/v1/panel/Panel_SSD1306.hpp"
#include "M5GFX.h"

#ifndef M5UNITOLED_SDA
 #if defined ( ARDUINO )
  #define M5UNITOLED_SDA SDA
 #elif defined (CONFIG_IDF_TARGET_ESP32S3)
  #define M5UNITOLED_SDA 2
 #elif defined (CONFIG_IDF_TARGET_ESP32C3)
  #define M5UNITOLED_SDA 1
 #else
  #define M5UNITOLED_SDA 21
 #endif
#endif

#ifndef M5UNITOLED_SCL
 #if defined ( ARDUINO )
  #define M5UNITOLED_SCL SCL
 #elif defined (CONFIG_IDF_TARGET_ESP32S3)
  #define M5UNITOLED_SCL 1
 #elif defined (CONFIG_IDF_TARGET_ESP32C3)
  #define M5UNITOLED_SCL 0
 #else
  #define M5UNITOLED_SCL 22
 #endif
#endif

#ifndef M5UNITOLED_ADDR
#define M5UNITOLED_ADDR 0x3C
#endif

#ifndef M5UNITOLED_FREQ
#define M5UNITOLED_FREQ 400000
#endif

class M5UnitOLED : public M5GFX
{
  lgfx::Bus_I2C::config_t _bus_cfg;

public:

  struct config_t
  {
    uint8_t pin_sda = 255;
    uint8_t pin_scl = 255;
    uint8_t i2c_addr = M5UNITOLED_ADDR;
    int8_t i2c_port = -1;
    uint32_t i2c_freq = M5UNITOLED_FREQ;
  };

  config_t config(void) const { return config_t(); }

#if defined (SDL_h_)

  M5UnitOLED(const config_t &cfg)
  {
    setup(cfg.pin_sda, cfg.pin_scl, cfg.i2c_freq, cfg.i2c_port, cfg.i2c_addr);
  }
  M5UnitOLED(uint8_t pin_sda = M5UNITOLED_SDA, uint8_t pin_scl = M5UNITOLED_SCL, uint32_t i2c_freq = M5UNITOLED_FREQ, int8_t i2c_port = -1, uint8_t i2c_addr = M5UNITOLED_ADDR)
  {
    setup(pin_sda, pin_scl, i2c_freq, i2c_port, i2c_addr);
  }

  using lgfx::LGFX_Device::init;
  bool init(uint8_t pin_sda, uint8_t pin_scl, uint32_t i2c_freq = M5UNITOLED_FREQ, int8_t i2c_port = -1, uint8_t i2c_addr = M5UNITOLED_ADDR)
  {
    return init();
  }

  void setup(uint8_t pin_sda = M5UNITOLED_SDA, uint8_t pin_scl = M5UNITOLED_SCL, uint32_t i2c_freq = M5UNITOLED_FREQ, int8_t i2c_port = -1, uint8_t i2c_addr = M5UNITOLED_ADDR)
  {
    _board = lgfx::board_t::board_M5UnitOLED;
  }

  bool init_impl(bool use_reset, bool use_clear)
  {
    if (_panel_last.get() != nullptr) {
      return true;
    }
    auto p = new lgfx::Panel_sdl;
    {
      auto cfg = p->config();
      cfg.memory_width = 64;
      cfg.panel_width  = 64;
      cfg.memory_height = 128;
      cfg.panel_height  = 128;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;
      p->config(cfg);
      uint_fast8_t scale = 2;
#if defined (M5GFX_SCALE)
 #if M5GFX_SCALE > 2
      scale = M5GFX_SCALE;
 #endif
#endif
      p->setScaling(scale, scale);
      p->setWindowTitle("UnitOLED");
      p->setColorDepth(lgfx::color_depth_t::grayscale_8bit);
    }
    setPanel(p);
    if (lgfx::LGFX_Device::init_impl(use_reset, use_clear)) {
      _panel_last.reset(p);
      return true;
    }
    setPanel(nullptr);
    delete p;
    return false;
  }

#else

  M5UnitOLED(const config_t &cfg)
  {
    uint8_t pin_sda = cfg.pin_sda < GPIO_NUM_MAX ? cfg.pin_sda : M5UNITOLED_SDA;
    uint8_t pin_scl = cfg.pin_scl < GPIO_NUM_MAX ? cfg.pin_scl : M5UNITOLED_SCL;
    setup(pin_sda, pin_scl, cfg.i2c_freq, cfg.i2c_port, cfg.i2c_addr);
  }

  M5UnitOLED(uint8_t pin_sda = M5UNITOLED_SDA, uint8_t pin_scl = M5UNITOLED_SCL, uint32_t i2c_freq = M5UNITOLED_FREQ, int8_t i2c_port = -1, uint8_t i2c_addr = M5UNITOLED_ADDR)
  {
    setup(pin_sda, pin_scl, i2c_freq, i2c_port, i2c_addr);
  }

  using lgfx::LGFX_Device::init;
  bool init(uint8_t pin_sda, uint8_t pin_scl, uint32_t i2c_freq = M5UNITOLED_FREQ, int8_t i2c_port = -1, uint8_t i2c_addr = M5UNITOLED_ADDR)
  {
    setup(pin_sda, pin_scl, i2c_freq, i2c_port, i2c_addr);
    return init();
  }

  void setup(uint8_t pin_sda = M5UNITOLED_SDA, uint8_t pin_scl = M5UNITOLED_SCL, uint32_t i2c_freq = M5UNITOLED_FREQ, int8_t i2c_port = -1, uint8_t i2c_addr = M5UNITOLED_ADDR)
  {
    _board = lgfx::board_t::board_M5UnitOLED;
    if (i2c_port < 0)
    {
      i2c_port = 0;
#ifdef _M5EPD_H_
      if ((pin_sda == 25 && pin_scl == 32)  /// M5Paper
      {
        i2c_port = 1
      }
#endif
    }

    {
      _bus_cfg.freq_write = i2c_freq;
      _bus_cfg.freq_read = i2c_freq;
      _bus_cfg.pin_scl = pin_scl;
      _bus_cfg.pin_sda = pin_sda;
      _bus_cfg.i2c_port = i2c_port;
      _bus_cfg.i2c_addr = i2c_addr;
      _bus_cfg.prefix_cmd = 0x00;
      _bus_cfg.prefix_data = 0x40;
      _bus_cfg.prefix_len = 1;
    }
  }

  bool init_impl(bool use_reset, bool use_clear)
  {
    if (_panel_last.get() != nullptr) {
      return true;
    }
    auto p = new lgfx::Panel_SH110x();
    auto b = new lgfx::Bus_I2C();
    b->config(_bus_cfg);
    {
      p->bus(b);
      auto cfg = p->config();
      cfg.panel_width = 64;
      cfg.offset_x = 32;
      cfg.bus_shared = false;
      p->config(cfg);
    }
    setPanel(p);
    if (lgfx::LGFX_Device::init_impl(use_reset, use_clear)) {
      _panel_last.reset(p);
      _bus_last.reset(b);
      return true;
    }
    setPanel(nullptr);
    delete p;
    delete b;
    return false;
  }
#endif
};

#endif
