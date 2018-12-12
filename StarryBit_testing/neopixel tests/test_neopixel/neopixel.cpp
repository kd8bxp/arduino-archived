#include <algorithm>  // for std::copy
#include "neopixel.h"


Color::Color(): g(0), r(0), b(0) { }
Color::Color(uint8_t rr, uint8_t gg, uint8_t bb) : g(gg), r(rr), b(bb) { }

Neopixel::Neopixel(uint16_t pin, uint16_t count)
  : pin_num(pin), led_count{count}
{
  const uint8_t PIN = pin_num;
              // nrf_gpio_cfg_output(PIN);
  NRF_GPIO->OUTCLR = (1UL << PIN);
  
  leds = new Color[led_count];
}

Neopixel::Neopixel(Neopixel& np)
{
  pin_num = np.pin_num;
  led_count = np.led_count;
  leds = new Color[led_count];
  std::copy(np.begin(), np.end(), leds);
}

Neopixel::Neopixel(Neopixel&& np)
{
  pin_num = np.pin_num;
  led_count = np.led_count;
  leds = np.leds;
  np.leds = nullptr;
}

Neopixel::~Neopixel()
{
  delete[] leds;
  led_count = 0;
  pin_num = 0;
}

void Neopixel::clear() {
  for (Color& led: *this) {
    led.g = 0;
    led.r = 0;
    led.b = 0;
  }
  show();
}

void Neopixel::show() {
  const uint8_t PIN =  pin_num;
  NRF_GPIO->OUTCLR = (1UL << PIN);
  nrf_delay_us(50);
  uint32_t irq_state = __get_PRIMASK();
  __disable_irq();

  for (int i = 0; i < led_count; i++) {
    for (int j = 0; j < 3; j++) {
      if ((leds[i].grb[j] & 128) > 0)	{NEOPIXEL_SEND_ONE}
      else	{NEOPIXEL_SEND_ZERO}

      if ((leds[i].grb[j] & 64) > 0)	{NEOPIXEL_SEND_ONE}
      else	{NEOPIXEL_SEND_ZERO}

      if ((leds[i].grb[j] & 32) > 0)	{NEOPIXEL_SEND_ONE}
      else	{NEOPIXEL_SEND_ZERO}

      if ((leds[i].grb[j] & 16) > 0)	{NEOPIXEL_SEND_ONE}
      else	{NEOPIXEL_SEND_ZERO}

      if ((leds[i].grb[j] & 8) > 0)	{NEOPIXEL_SEND_ONE}
      else	{NEOPIXEL_SEND_ZERO}

      if ((leds[i].grb[j] & 4) > 0)	{NEOPIXEL_SEND_ONE}
      else	{NEOPIXEL_SEND_ZERO}

      if ((leds[i].grb[j] & 2) > 0)	{NEOPIXEL_SEND_ONE}
      else	{NEOPIXEL_SEND_ZERO}

      if ((leds[i].grb[j] & 1) > 0)	{NEOPIXEL_SEND_ONE}
      else	{NEOPIXEL_SEND_ZERO}
    }
  }
  __set_PRIMASK(irq_state);
}


uint8_t Neopixel::setColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
  if (index < led_count) {
    leds[index].r = r;
    leds[index].g = g;
    leds[index].b = b;
    return 0;
  }
  return 1;
}

uint8_t Neopixel::showColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
  if (setColor(index, r, g, b) == 0) {
    show();
    return 0;
  }
  return 1;
}
