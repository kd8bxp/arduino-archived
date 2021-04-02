#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include <stdint.h>
#include <math.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include <string>
#include "epdParallel.h"
//#include "../Adafruit_GFX/Adafruit_GFX.h"
//#include "epdspi.h"
#include "../Epdiy-IDF/epd_driver.h"

#define HAS_16_LEVELS_GRAY 1
#define ED047TC1_WIDTH 960
#define ED047TC1_HEIGHT 540

// COLOR defines (Only 8 but actually this epapers have 16 levels)
#define EPD_WHITE 255
#define EPD_WHITISH 223
#define EPD_SLGRAY 200
#define EPD_LGRAY 150
#define EPD_GRAY 100
#define EPD_DGRAY 50
#define EPD_SDGRAY 25
#define EPD_BLACK 0

// 1 byte of this color in the buffer
// This needs to be refactored since this display uses 4 bit per pixel

class Ed047TC1 : public EpdParallel 
{
  public:
    Ed047TC1();

    uint8_t *framebuffer;
    uint8_t colors_supported = 1;

    void drawPixel(int16_t x, int16_t y, uint16_t color);  // Override GFX own drawPixel method

    void init(bool debug = false);
    void clearScreen();
    void clearArea(Rect_t area);
    void powerOn();
    void powerOff();
    
    void fillScreen(uint16_t color);
    void update(enum DrawMode mode = BLACK_ON_WHITE);
    
    // Partial update of rectangle from buffer to screen, does not power off
    // Pending implementation
    void updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true);

  private:
    bool color = false;
    bool _initial = true;
    bool _debug_buffer = false;
};
