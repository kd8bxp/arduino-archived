#include <Arduino.h>
#include <TVout.h>
#include <avr/pgmspace.h>

#define PATTERN_WHITE  0
#define PATTERN_BLACK  1

#define SCREEN_WIDTH  96
#define SCREEN_HEIGHT  64

// Inputs
#define DPAD_UP			B10000000
#define DPAD_LEFT		B01000000
#define DPAD_RIGHT		B00100000
#define DPAD_DOWN		B00010000
#define DPAD_UP_LEFT            DPAD_UP | DPAD_LEFT
#define DPAD_UP_RIGHT           DPAD_UP | DPAD_RIGHT
#define DPAD_DOWN_LEFT          DPAD_DOWN | DPAD_LEFT
#define DPAD_DOWN_RIGHT         DPAD_DOWN | DPAD_RIGHT
#define DPAD_ANY		B11110000
#define BUTTON_1		B00001000
#define BUTTON_2		B00000100
#define BUTTON_3		B00000010
#define BUTTON_4		B00000001
#define BUTTON_ANY		B00001111  

class GambyEmulation {
 public:
  void init();
  void update();
  void clearScreen();
  void setPos(byte, byte);
  void drawChar(char);
  void print(char *);
  void print(unsigned int);
  void print_P(const char *);
  
  void rect(char x1, char y1, char x2, char y2);
  void setPixel(char x, char y);
  void setPixel(char x, char y, char p);
  void drawSprite(byte x, byte y, const prog_uchar *sprite);
  void drawSprite(byte x, byte y, const prog_uchar *sprite, byte frame);
  void drawSprite(byte x, byte y, const prog_uchar *sprite, const prog_uchar *mask);
  void drawSprite(byte x, byte y, const prog_uchar *sprite, byte frame, const prog_uchar *mask, byte maskFrame);
  void readInputs();
  
  static byte inputs;            /**< The D-Pad and button states. Set by readInputs(). */
  static byte current_x;
  static byte current_y;
  static unsigned int drawPattern;
  
  const prog_int32_t* font; /**< The font to be used for drawing text, read from PROGMEM. */

 private:
  void drawByte(byte data);
};
