#include "SPI.h"
#include "src/Adafruit_GFX/Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"

// Define pins for display interface. You'll probably need to edit this for
// your own needs:


#define TFT_DC  2
#define TFT_CS 10
#define TFT_BL 3
#define TFT_CLK 6
#define TFT_MOSI 7
#define TFT_MISO -1
#define TFT_RST -1

// Display constructor for primary hardware SPI connection -- the specific
// pins used for writing to the display are unique to each board and are not
// negotiable. "Soft" SPI (using any pins) is an option but performance is
// GC9A01A_REDuced; it's rarely used, see header file for syntax if needed.
Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);


extern const unsigned char eyecenter[];

extern const unsigned char eyeright1[];
extern const unsigned char eyeright2[];

extern const unsigned char eyeleft1[];
extern const unsigned char eyeleft2[];

extern const unsigned char eyeblink1[];
extern const unsigned char eyeblink2[];
extern const unsigned char eyeblink3[];
extern const unsigned char eyeblink4[];
extern const unsigned char eyeblink5[];
byte eye_x, eye_y;

void setup() {
  Serial.begin(115200);
  Serial.println("GC9A01A Test!");

  tft.begin();

#if defined(TFT_BL)
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Backlight on
#endif // end TFT_BL

  
}

void loop(void) {
  eye_x = 0; //75; 
  eye_y = 0; //90;  
  
  
  tft.drawBitmap(eye_x, eye_y, eyecenter,240,240,GC9A01A_RED);
  delay(100);
  tft.fillScreen( GC9A01A_BLACK );
  tft.drawBitmap(eye_x, eye_y, eyeright1,240,240,GC9A01A_RED);
  delay(50);
  tft.fillScreen( GC9A01A_BLACK );
  tft.drawBitmap(eye_x, eye_y, eyeright2,240,240,GC9A01A_RED);
  delay(100);
   
  tft.fillScreen( GC9A01A_BLACK );
  tft.drawBitmap(eye_x, eye_y, eyeright1,240,240,GC9A01A_RED);
  delay(50);
  
  tft.fillScreen( GC9A01A_BLACK );
  tft.drawBitmap(eye_x, eye_y, eyecenter,240,240,GC9A01A_RED);
  delay(50);
  
  tft.fillScreen( GC9A01A_BLACK );
  tft.drawBitmap(eye_x, eye_y, eyeleft1,240,240,GC9A01A_RED);
  delay(50);
  
  tft.fillScreen( GC9A01A_BLACK );
  tft.drawBitmap(eye_x, eye_y, eyeleft2,240,240,GC9A01A_RED);
  delay(100);
  
  tft.fillScreen( GC9A01A_BLACK );
  tft.drawBitmap(eye_x, eye_y, eyeleft1,240,240,GC9A01A_RED);
  delay(50);


  
  tft.fillScreen( GC9A01A_BLACK );
  tft.drawBitmap(eye_x, eye_y, eyeblink1,240,240,GC9A01A_RED);
  delay(10);
  
  tft.fillScreen( GC9A01A_BLACK );
  
  tft.drawBitmap(eye_x, eye_y, eyeblink2,240,240,GC9A01A_RED);
  delay(10);
  tft.fillScreen( GC9A01A_BLACK );
  
  tft.drawBitmap(eye_x, eye_y, eyeblink3,240,240,GC9A01A_RED);
  delay(10);
  tft.fillScreen( GC9A01A_BLACK );
  
  tft.drawBitmap(eye_x, eye_y, eyeblink4,240,240,GC9A01A_RED);
  delay(10);
  tft.fillScreen( GC9A01A_BLACK );
  
  tft.drawBitmap(eye_x, eye_y, eyeblink5,240,240,GC9A01A_RED);
  delay(10);
  tft.fillScreen( GC9A01A_BLACK );
  
  tft.drawBitmap(eye_x, eye_y, eyeblink4,240,240,GC9A01A_RED);
  delay(10);
  tft.fillScreen( GC9A01A_BLACK );
  
  tft.drawBitmap(eye_x, eye_y, eyeblink3,240,240,GC9A01A_RED);
  delay(10);
  tft.fillScreen( GC9A01A_BLACK );
  
  tft.drawBitmap(eye_x, eye_y, eyeblink2,240,240,GC9A01A_RED);
  delay(10);
  tft.fillScreen( GC9A01A_BLACK );
  
  tft.drawBitmap(eye_x, eye_y, eyeblink1,240,240,GC9A01A_RED);
  delay(10);
  tft.fillScreen( GC9A01A_BLACK );
  
}
