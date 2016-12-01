// Display Test
// Test several Display Adafruit_GFX functions
// for 1.8" TFT LCD Color Display (SainSmart).
// Hans Luijten, Tweaking4All.com

// Init pins
#define sclk 13
#define mosi 11
#define cs   10
#define dc   9
#define rst  8  

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

void setup() {
  // Init serial for serial monitor, so we can see debug text in the Serial Monitor Window
  Serial.begin(9600);
  Serial.println("Display Function Tests Started");

  // If your TFT's plastic wrap has a Black Tab, use the following:
 // tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  // If your TFT's plastic wrap has a Red Tab, use the following:
  //tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
  // If your TFT's plastic wrap has a Green Tab, use the following:
  tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab  
  Serial.println("setup() - Init Completed");
}

void loop() {
  // This keeps repeating ...
  Serial.println("loop() - starting (again) ...");

  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

  // Hello
  Serial.println("Hello message");
  tft.setTextSize(1);
  tft.setCursor(0, 50);
  tft.setTextColor(ST7735_WHITE);
  tft.print("   Tweaking");
  tft.setTextColor( tft.Color565( 0xFF, 0xCC, 0x00) );
  tft.print("4");
  tft.setTextColor(ST7735_WHITE);
  tft.println("All.com");
  tft.setTextSize(1);
  tft.println("     Display Demo");
  delay(3000);

  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

  // Draw some pixels
  Serial.println("Draw 4 pixels");
  tft.drawPixel(10,10,ST7735_WHITE);
  tft.drawPixel(100,100,ST7735_GREEN);
  tft.drawPixel(10,50,ST7735_RED);
  tft.drawPixel(100,50,ST7735_YELLOW);
  delay(3000);

  // Draw some lines
  Serial.println("Draw some lines");
  tft.drawLine(10,10,100,100,ST7735_WHITE);
  tft.drawLine(100,100,10,50,ST7735_GREEN);
  tft.drawLine(10,50,100,50,ST7735_RED);
  tft.drawLine(100,50,10,10,ST7735_YELLOW);
  delay(3000);

  // Draw some rectangles
  Serial.println("Draw some rectangles, with delay between open and filled rectangle");
  tft.drawRect(20,20,100,100,ST7735_RED);
  tft.drawRect(40,10,10,100,ST7735_YELLOW);
  delay(2000);
  tft.fillRect(50,10,50,20,ST7735_RED);
  tft.fillRect(80,80,10,10,ST7735_YELLOW);
  delay(3000);

  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

  // Draw some circles
  Serial.println("Draw some circles, with delay between open and filled cicle");
  tft.drawCircle(70,70,10,ST7735_WHITE);
  tft.drawCircle(70,70,50,ST7735_RED);
  delay(2000);
  tft.fillCircle(20,20,50,ST7735_YELLOW);
  tft.fillCircle(20,20,10,ST7735_BLUE);
  delay(3000);

  // Draw some triangles
  Serial.println("Draw some triangles, with delay between open and filled triangles");
  tft.drawTriangle(64,100, 64,150, 100,150, ST7735_YELLOW);
  tft.drawTriangle(64,100, 44,150, 84,150,  ST7735_RED);
  delay(1000);
  tft.fillTriangle(64,100, 64,150, 100,150, ST7735_YELLOW);
  tft.fillTriangle(64,100, 44,150, 84,150,  ST7735_RED);
  delay(3000);

  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

  // Draw rectangles with rounded corners for a dialog
  // Big box
  Serial.println("Display panic message!");
  tft.fillRoundRect( 14,16, 100, 100, 8, tft.Color565( 0xC4, 0xC4, 0xC4) );
  tft.drawRoundRect( 14,16, 100, 100, 8, tft.Color565( 0x99, 0x99, 0x99) );
  // Title
  tft.fillRoundRect( 20,8, 60, 20, 4, tft.Color565( 0xFF, 0, 0) );
  tft.drawRoundRect( 20,8, 60, 20, 4, tft.Color565( 0x99, 0x99, 0x99) );
  // Title text
  tft.setCursor(28, 14);
  tft.setTextColor(ST7735_WHITE);
  tft.print("WARNING");
  // Message
  tft.setTextColor(ST7735_RED);
  tft.setCursor(22, 38);
  tft.print("Something is");
  tft.setCursor(22, 48);
  tft.print("very wrong!");
  tft.setCursor(22, 68);
  tft.print("OMG - PANIC !");
  tft.setCursor(22, 88);
  tft.print("We're out of");
  tft.setCursor(22, 98);
  tft.print("COFFEE!!!");
  delay(3000);

  // Display blinking -- invert or normal
  Serial.println("Display blinking ...");
  for(int i=0; i<10; i++)
  {
    tft.invertDisplay(true);
    delay(500);
    tft.invertDisplay(false);
    delay(500);
  }  
  delay(3000);

  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

  // Write a long text with word wrap
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(0, 0);
  tft.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ");
  delay(2000);

  // Clear screen
  Serial.println("Fill screen with BLACK");
  tft.fillScreen(ST7735_BLACK);

  Serial.println("Test font sizes");
  tft.setTextWrap(true);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(0, 0);
  delay(1000);

  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.println("1 (default)");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.setTextSize(2);
  tft.println("2");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.setTextSize(3);
  tft.println("3");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.setTextSize(4);
  tft.println("4");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.setTextSize(5);
  tft.println("5");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.setTextSize(6);
  tft.println("6");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.setTextSize(7);
  tft.println("7");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.setTextSize(8);
  tft.println("8");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.setTextSize(9);
  tft.println("9");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Fontsize:");
  tft.setCursor(0, 30);
  tft.setTextSize(10);
  tft.println("10");
  delay(3000);
}