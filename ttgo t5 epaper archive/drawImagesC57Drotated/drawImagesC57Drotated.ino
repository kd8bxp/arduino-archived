/* Simple firmware for a ESP32 displaying a static image on an EPaper Screen.
 *
 * Write an image into a header file using a 3...2...1...0 format per pixel,
 * for 4 bits color (16 colors - well, greys.) MSB first.  At 80 MHz, screen
 * clears execute in 1.075 seconds and images are drawn in 1.531 seconds.
 */

#include <Arduino.h>
#include "src/epd_driver.h"
#include "c57drotated.h"

uint8_t *framebuffer;

void setup()
{
    Serial.begin(115200);

    epd_init();

    framebuffer = (uint8_t *)heap_caps_malloc(EPD_WIDTH * EPD_HEIGHT / 2, MALLOC_CAP_SPIRAM);
    memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

}

void update(uint32_t delay_ms)
{
    epd_poweron();
    epd_clear();
    volatile uint32_t t1 = millis();
    epd_draw_grayscale_image(epd_full_screen(), framebuffer);
    volatile uint32_t t2 = millis();
    Serial.printf("EPD draw took %dms.\n", t2 - t1);
    epd_poweroff();
    delay(delay_ms);
}

void loop()
{
    
    epd_poweron();
    epd_clear();
    
    epd_poweroff();
    delay(3000);

    Rect_t area1 = {
        .x = 0,
        .y = 0,
        .width = c57d_width,
        .height =  c57d_height
    };
    epd_copy_to_framebuffer(area1, (uint8_t *) c57d_data, framebuffer);

    update(3000);

 
}
