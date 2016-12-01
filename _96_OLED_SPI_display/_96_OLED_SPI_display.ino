// 96 OLED SPI display by steve8939 

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
/*#define OLED_MOSI   9 // SDA
#define OLED_CLK   10 // SCL
#define OLED_DC    11 // D/C
#define OLED_CS    12 // Not used
#define OLED_RESET 13 // RST
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
*/
// Uncomment this block to use hardware SPI
 #define OLED_DC     26
 #define OLED_CS     27
 #define OLED_RESET  24
 Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
 



#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int nFrames = 36;

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
}


void loop() {
  for (int frame=0; frame < nFrames; frame++)
  {
    HariChord(frame);
  }

  for (int frame=(nFrames-1); frame >= 0; frame--)
  {
    HariChord(frame);
  }
}

void HariChord(int frame)
{
  display.clearDisplay();
  int n = 7;
  int r = frame * 64 / nFrames;
  float rot = frame * 2*PI / nFrames;
  for (int i=0; i<(n-1); i++)
  {
    float a = rot + i * 2*PI / n;
    int x1 = 64 + cos(a) * r;
    int y1 = 32 + sin(a) * r;
    for (int j=i+1; j<n; j++)
    {
      a = rot + j * 2*PI / n;
      int x2 = 64 + cos(a) * r;
      int y2 = 32 + sin(a) * r;
      display.drawLine(x1,y1, x2,y2, WHITE);
    }
  }
  display.display();
}