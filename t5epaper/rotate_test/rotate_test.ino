// Run idf.py menuconfig-> Component Config -> E-Paper driver and select:
// Display type: LILIGO 4.7 ED047TC1
// Board: LILIGO T5-4.7 Epaper
// In the same section Component Config -> ESP32 Specifics -> Enable PSRAM
#include "src/CalEPD/ED047TC1.h"
Ed047TC1 display;

// Include a font header
#include "src/Adafruit_GFX/Fonts/ubuntu/Ubuntu_M24pt8b.h"

//extern "C" { void app_main(); }

//void delay(uint32_t millis) { vTaskDelay(millis / portTICK_PERIOD_MS); }

void setup()
{
   display.init();
   //display.setRotation(0); // Working, 1 rotates 90° the display
   // Clear all screen to white
   display.clearScreen();
   delay(1000);

   // Draw some rectangles
   for (int x = 0; x < 200; x++) {

      for (int y = 0; y < 400; y++) {
         display.drawPixel(x, y, 0);
         display.drawPixel(x+200, y, 80);
         display.drawPixel(x+400, y, 160);
         display.drawPixel(x+600, y, 200);
         display.drawPixel(x+760, y, 230);
      }
   }

   // Draw some text using Ubuntu Font
   display.setCursor(30,200);
   display.setFont(&Ubuntu_M24pt8b);
   display.println("Hello world");
}

void loop() {
  
}
