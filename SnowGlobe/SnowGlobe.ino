/**************************************************************************
 * M5Stack IMU Snow-Globe
 * 
 * A simple program that turns an M5Stack into a digital snow globe.
 * Works with M5Stack Gray, FIRE and CORE2
 * 
 * Hague Nusseck @ electricidea
 * v1.20 25.December.2020
 * https://github.com/electricidea/M5Stack-Snow-Globe
 * 
 * Changelog:
 * v1.01 = - initial version for the M5Stack-fire
 * v1.20 = - 8 bit Sprite instead of 16 bit sprite
 *         - NO PSRAM needed anymore
 *         - works with M5Stack Gray and FIRE
 *         - pushing the sprite without transparency color
 * v1.21 = - compiler options for the different M5Stack devices
 * 
 * Distributed as-is; no warranty is given.
**************************************************************************/

//Select the device for which the code should be compiled and build:
#define M5STACK_GRAY
//#define M5STACK_FIRE
//#define M5STACK_CORE2

// REMEMBER:
// Also change the "board" option in the platformio.ini:
//
// board = m5stack-grey
// board = m5stack-fire
// board = m5stack-core-esp32

#include <Arduino.h>

/*#if defined (M5STACK_CORE2)
  #include <M5Core2.h>
  // install the library:
  // pio lib install "m5stack/M5Core2"

#else // M5STACK_GRAY or M5STACK_FIRE
*/
  // For the M5Stack FIRE and M5Stack-gray, this definition 
  // must be placed before the #include <M5Stack.h>:
  // #define M5STACK_MPU6886 
  #define M5STACK_MPU9250 
  // #define M5STACK_MPU6050
  // #define M5STACK_200Q

  #include "src/M5Stack/M5Stack.h"
  // install the library:
  // pio lib install "M5Stack"

//#endif

// Free Fonts for nice looking fonts on the screen
#include "Free_Fonts.h"

// logo with 150x150 pixel size in XBM format
// check the file header for more information
#include "electric-idea_logo.h"

// Stuff for the Graphical output
// The M5Stack screen pixel is 320x240, with the top left corner of the screen as the origin (0,0)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// acceleration values
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;
/*  Direction of acceleration:

      . down = +z
  - - - - -
  |       |
  |       | --> -x
  | O O O |
  - - - - - 
      |
      V
     +y 
*/

// bitmap of one snowflake (XBM Format)
#include "snowflake.h"

// background image as uint16_t RGB565 array
#include "background_image.h"

// structure for the position and speed of every snow flake
const int flake_max = 250;
struct flakeObject
{
  int32_t x;
  int32_t y;
  float speed;
};

flakeObject flakeArray[flake_max];

// Sprite object "img" with pointer to "M5.Lcd" object
// the pointer is used by pushSprite() to push it onto the LCD
TFT_eSprite img = TFT_eSprite(&M5.Lcd);  

// color depth of the sprite
// can be: 1, 8 or 16
// NOTE: 16bit fullscreen sprites require more memory. 
// They only work with the M5Stack Fire or Core2 with activated PSRAM.
#define SPRITE_COLOR_DEPTH 8

void setup(void) {
  M5.begin();
  #if defined (M5STACK_FIRE)
    M5.Power.begin();
  #endif
  M5.IMU.Init();
  // int the starting position of all snowflakes
  for(int i=0; i < flake_max; i++){
    // horizontally distributed
    flakeArray[i].x = random(SCREEN_WIDTH);
    // at the bottom of the screen
    flakeArray[i].y = SCREEN_HEIGHT-random(20);
    // individual speed for each snowflake
    flakeArray[i].speed = (random(80)+20)/100.0;
  }
  // electric-idea logo
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawXBitmap((int)(320-logoWidth)/2, (int)(240-logoHeight)/2, logo, logoWidth, logoHeight, TFT_WHITE);
  delay(1500);
  // Create a sprite
  img.setColorDepth(SPRITE_COLOR_DEPTH);
  img.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
  // welcome text
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.fillScreen(TFT_BLACK);
  // configure centered String output
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setFreeFont(FF2);
  M5.Lcd.drawString("IMU Snow Globe", (int)(M5.Lcd.width()/2), (int)(M5.Lcd.height()/2), 1);
  M5.Lcd.setFreeFont(FF1);
  M5.Lcd.drawString("Version 1.21 | 25.12.2020", (int)(M5.Lcd.width()/2), M5.Lcd.height()-20, 1);
  delay(1500);
}

void loop() {
  M5.update();
  // mute speaker
  dacWrite (25,0);
  // push the background image to the sprite
  img.pushImage(0, 0, 320, 240, (uint16_t *)background_image);
  // get the acceleration data
  // values are in g (9.81 m/s2)
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  // Draw the snowflakes
  for (int i=0; i < flake_max; i++)
  {
    // detect shaking
    if(fabs(accX) > 2 || fabs(accY) > 2){
      flakeArray[i].x = random(SCREEN_WIDTH);
      flakeArray[i].y = random(SCREEN_HEIGHT);
    }
    else {
      // use gravity vector for movement
      float dx = (accX*-10.0) + (round(accX)*random(5)) + (round(accY)*(random(10)-5));
      float dy = (accY*10.0) +  (round(accX)*random(5)) + (round(accY)*(random(10)-5));
      flakeArray[i].x = flakeArray[i].x + round(dx*flakeArray[i].speed);
      flakeArray[i].y = flakeArray[i].y + round(dy*flakeArray[i].speed);
    }
    // keep the snowflakes on the screen
    if(flakeArray[i].x < 0)
      flakeArray[i].x = 0;
    if(flakeArray[i].x > SCREEN_WIDTH)
      flakeArray[i].x = SCREEN_WIDTH;
    if(flakeArray[i].y < 0)
      flakeArray[i].y = 0;
    if(flakeArray[i].y > SCREEN_HEIGHT)
      flakeArray[i].y = SCREEN_HEIGHT;
    // push the snowflake to the sprite on top of the background image
    img.drawXBitmap((int)(flakeArray[i].x-flakeWidth), 
                       (int)(flakeArray[i].y-flakeHeight), 
                       snowflake, flakeWidth, flakeHeight, TFT_WHITE);
  }
  // After all snowflakes are drawn, pus the sprite
  // to TFT screen CGRAM at coordinate x,y (top left corner)
  img.pushSprite(0, 0);
  delay(5);
}
