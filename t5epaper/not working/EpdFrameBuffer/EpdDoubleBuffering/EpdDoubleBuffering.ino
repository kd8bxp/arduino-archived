/* Simple firmware for a ESP32 displaying a static image on an EPaper Screen.
   Image is firs drawn on a framebuffer in memory, then put on screen all at once
*/

#include <Arduino.h>
#include "src/epd_driver.h"

uint8_t *framebuffer;

void setup()
{
  Serial.begin(115200);
  epd_init();
  framebuffer = (uint8_t *)heap_caps_malloc(EPD_WIDTH * EPD_HEIGHT / 2, MALLOC_CAP_SPIRAM);
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
}

void loop()
{
#define STEPS 30

  int stepX = EPD_WIDTH / STEPS;
  int stepY = EPD_HEIGHT / STEPS;
  for (int i = 1 ; i < STEPS ; i++) {
    epd_draw_line(i * stepX, 0, EPD_WIDTH, i * stepY, 0, framebuffer);
    epd_draw_line(EPD_WIDTH, i * stepY, EPD_WIDTH - i * stepX, EPD_HEIGHT, 0, framebuffer);
    epd_draw_line(EPD_WIDTH - i * stepX, EPD_HEIGHT, 0, EPD_HEIGHT - i * stepY, 0, framebuffer);
    epd_draw_line(0, EPD_HEIGHT - i * stepY, i * stepX, 0, 0, framebuffer);

    epd_draw_circle(50 + i * stepX, i * i / 2, i * 5, i*192/STEPS, framebuffer);
    epd_draw_circle(EPD_WIDTH - 50 - i * stepX, EPD_HEIGHT - i * i / 2, i * 5, i*192/STEPS, framebuffer);
  }

  epd_poweron();
  epd_clear();
  epd_draw_grayscale_image(epd_full_screen(), framebuffer);
  epd_poweroff();
  delay(1000);
  epd_poweroff_all();
  delay(60000);
}
