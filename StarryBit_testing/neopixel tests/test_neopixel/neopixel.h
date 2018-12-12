#pragma once

#include <cstdint>
#include "nrf_delay.h"
//#include "nrf_gpio.h"
//#include "MicroBitPin.h"

// roughly a port of https://github.com/lavallc/nrf51-neopixel

//These defines are timed specific to a series of if statements and will need to be changed
//to compensate for different writing algorithms than the one in neopixel.c
#define NEOPIXEL_SEND_ONE	NRF_GPIO->OUTSET = (1UL << PIN); \
    __ASM ( \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
      ); \
      NRF_GPIO->OUTCLR = (1UL << PIN); \

#define NEOPIXEL_SEND_ZERO NRF_GPIO->OUTSET = (1UL << PIN); \
    __ASM (  \
        " NOP\n\t"  \
      );  \
      NRF_GPIO->OUTCLR = (1UL << PIN);  \
    __ASM ( \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
        " NOP\n\t" \
      );


struct Color {
  union {
    struct {
      uint8_t g, r, b;
    };
    uint8_t grb[3];
  };

  Color();
  Color(uint8_t r, uint8_t g, uint8_t b);
};


/**
  Class for manipulating an array of neopixels.

  Stores an array (buffer) of color values for the leds,
  to make changes to the object visible call show().
*/
class Neopixel {
private:
  uint8_t pin_num;
  uint16_t led_count;
  Color* leds;
public:
  /**
    @brief Initialize Neopixel with output pin name and led count
    @param[in] name of output pin (e.g. MICROBIT_PIN_P9)
  	@param[in] count of leds
  */
  Neopixel(uint16_t pin, uint16_t led_count);

  Neopixel(Neopixel& np);
  Neopixel(Neopixel&& np);
  ~Neopixel();

  // iterator
  Color* begin() { return leds; }
  Color* end() { return leds + led_count; }

  /**
    @brief Turn all LEDs off (set to r=g=b=0 and show)
  */
  void clear();

  /**
    @brief Update with the new data in the class
  */
  void show();

  /**
    @brief Set the color of one LED to RGB values
    @param[in] LED number (starting at 1)
    @param[in] red value
    @param[in] green value
    @param[in] blue value
    @retval 0 Successful write
    @retval 1 LED number is out of bounds
  */
  uint8_t setColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b);

  /**
    @brief Set the color of one LED given Color object
    @param[in] LED number (starting at 1)
    @param[in] Color object
    @retval 0 Successful write
    @retval 1 LED number is out of bounds
  */
  uint8_t setColor(uint16_t index, const Color& color)
  {
    return setColor(index, color.r, color.g, color.b);
  }

  /**
    @brief Set the color of one LED to RGB values and update LED
    @param[in] LED number (starting at 1)
    @param[in] red value
    @param[in] green value
    @param[in] blue value
    @retval 0 Successful write
    @retval 1 LED number is out of bounds
  */
  uint8_t showColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b);

  /**
    @brief Set the color of one LED given Color object and update LED
    @param[in] LED number (starting at 1)
    @param[in] Color object
    @retval 0 Successful write
    @retval 1 LED number is out of bounds
  */
  uint8_t showColor(uint16_t index, const Color& color)
  {
    return showColor(index, color.r, color.g, color.b);
  }

};
