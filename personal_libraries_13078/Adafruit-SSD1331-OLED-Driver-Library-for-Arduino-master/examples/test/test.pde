/*************************************************** 
  This is a example sketch demonstrating the graphics
  capabilities of the SSD1331 library  for the 0.96&quot; 
  16-bit Color OLED with SSD1331 driver chip

  Pick one up today in the adafruit shop!
  ------&gt; http://www.adafruit.com/products/684

  These displays use SPI to communicate, 4 or 5 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/


// You can use any (4 or) 5 pins 
#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8


// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include &lt;Adafruit_GFX.h&gt;
#include &lt;Adafruit_SSD1331.h&gt;
#include &lt;SPI.h&gt;

// Option 1: use any pins but a little slower
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
//Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.print(&quot;hello!&quot;);
  display.begin();

  Serial.println(&quot;init&quot;);
  uint16_t time = millis();
  display.fillScreen(BLACK);
  time = millis() - time;
  
  Serial.println(time, DEC);
  delay(500);
   
  lcdTestPattern();
  delay(1000);
  
  display.fillScreen(BLACK);
  display.setCursor(0,0);
  display.print(&quot;Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa&quot;);
  delay(1000);

  // tft print function!
  tftPrintTest();
  delay(2000);
  
  //a single pixel
  display.drawPixel(display.width()/2, display.height()/2, GREEN);
  delay(500);
  
  // line draw test
  testlines(YELLOW);
  delay(500);    
  
  // optimized lines
  testfastlines(RED, BLUE);
  delay(500);    
 
  testdrawrects(GREEN);
  delay(1000);

  testfillrects(YELLOW, MAGENTA);
  delay(1000);

  display.fillScreen(BLACK);
  testfillcircles(10, BLUE);
  testdrawcircles(10, WHITE);
  delay(1000);

  testroundrects();
  delay(500);
  
  testtriangles();
  delay(500);
  
  Serial.println(&quot;done&quot;);
  delay(1000);
}

void loop() {
}

void testlines(uint16_t color) {
   display.fillScreen(BLACK);
   for (int16_t x=0; x &lt; display.width()-1; x+=6) {
     display.drawLine(0, 0, x, display.height()-1, color);
   }
   for (int16_t y=0; y &lt; display.height()-1; y+=6) {
     display.drawLine(0, 0, display.width()-1, y, color);
   }
   
   display.fillScreen(BLACK);
   for (int16_t x=0; x &lt; display.width()-1; x+=6) {
     display.drawLine(display.width()-1, 0, x, display.height()-1, color);
   }
   for (int16_t y=0; y &lt; display.height()-1; y+=6) {
     display.drawLine(display.width()-1, 0, 0, y, color);
   }
   
   display.fillScreen(BLACK);
   for (int16_t x=0; x &lt; display.width()-1; x+=6) {
     display.drawLine(0, display.height()-1, x, 0, color);
   }
   for (int16_t y=0; y &lt; display.height()-1; y+=6) {
     display.drawLine(0, display.height()-1, display.width()-1, y, color);
   }

   display.fillScreen(BLACK);
   for (int16_t x=0; x &lt; display.width()-1; x+=6) {
     display.drawLine(display.width()-1, display.height()-1, x, 0, color);
   }
   for (int16_t y=0; y &lt; display.height()-1; y+=6) {
     display.drawLine(display.width()-1, display.height()-1, 0, y, color);
   }
   
}

void testdrawtext(char *text, uint16_t color) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i &lt; 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i &gt; 0) &amp;&amp; (i % 21 == 0))
      display.println();
  }    
}

void testfastlines(uint16_t color1, uint16_t color2) {
   display.fillScreen(BLACK);
   for (int16_t y=0; y &lt; display.height()-1; y+=5) {
     display.drawFastHLine(0, y, display.width()-1, color1);
   }
   for (int16_t x=0; x &lt; display.width()-1; x+=5) {
     display.drawFastVLine(x, 0, display.height()-1, color2);
   }
}

void testdrawrects(uint16_t color) {
 display.fillScreen(BLACK);
 for (int16_t x=0; x &lt; display.height()-1; x+=6) {
   display.drawRect((display.width()-1)/2 -x/2, (display.height()-1)/2 -x/2 , x, x, color);
 }
}

void testfillrects(uint16_t color1, uint16_t color2) {
 display.fillScreen(BLACK);
 for (int16_t x=display.height()-1; x &gt; 6; x-=6) {
   display.fillRect((display.width()-1)/2 -x/2, (display.height()-1)/2 -x/2 , x, x, color1);
   display.drawRect((display.width()-1)/2 -x/2, (display.height()-1)/2 -x/2 , x, x, color2);
 }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x &lt; display.width()-1; x+=radius*2) {
    for (uint8_t y=radius; y &lt; display.height()-1; y+=radius*2) {
      display.fillCircle(x, y, radius, color);
    }
  }  
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x &lt; display.width()-1+radius; x+=radius*2) {
    for (int16_t y=0; y &lt; display.height()-1+radius; y+=radius*2) {
      display.drawCircle(x, y, radius, color);
    }
  }  
}

void testtriangles() {
  display.fillScreen(BLACK);
  int color = 0xF800;
  int t;
  int w = display.width()/2;
  int x = display.height();
  int y = 0;
  int z = display.width();
  for(t = 0 ; t &lt;= 15; t+=1) {
    display.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  display.fillScreen(BLACK);
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t &lt;= 4; t+=1) {
  int x = 0;
  int y = 0;
  int w = display.width();
  int h = display.height();
    for(i = 0 ; i &lt;= 24; i+=1) {
    display.drawRoundRect(x, y, w, h, 5, color);
    x+=2;
    y+=3;
    w-=4;
    h-=6;
    color+=1100;
  }
  color+=100;
  }
}

void tftPrintTest() {
  display.fillScreen(BLACK);
  display.setCursor(0, 5);
  display.setTextColor(RED);  
  display.setTextSize(1);
  display.println(&quot;Hello World!&quot;);
  display.setTextColor(YELLOW, GREEN);
  display.setTextSize(2);
  display.print(&quot;Hello Wo&quot;);
  display.setTextColor(BLUE);
  display.setTextSize(3);
  display.print(1234.567);
  delay(1500);
  display.setCursor(0, 5);
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(0);
  display.println(&quot;Hello World!&quot;);
  display.setTextSize(1);
  display.setTextColor(GREEN);
  display.print(p, 5);
  display.println(&quot; Want pi?&quot;);
  display.print(8675309, HEX); // print 8,675,309 out in HEX!
  display.print(&quot; Print HEX&quot;);
  display.setTextColor(WHITE);
  display.println(&quot;Sketch has been&quot;);
  display.println(&quot;running for: &quot;);
  display.setTextColor(MAGENTA);
  display.print(millis() / 1000);
  display.setTextColor(WHITE);
  display.print(&quot; seconds.&quot;);
}

void mediabuttons() {
 // play
  display.fillScreen(BLACK);
  display.fillRoundRect(25, 10, 78, 60, 8, WHITE);
  display.fillTriangle(42, 20, 42, 60, 90, 40, RED);
  delay(500);
  // pause
  display.fillRoundRect(25, 90, 78, 60, 8, WHITE);
  display.fillRoundRect(39, 98, 20, 45, 5, GREEN);
  display.fillRoundRect(69, 98, 20, 45, 5, GREEN);
  delay(500);
  // play color
  display.fillTriangle(42, 20, 42, 60, 90, 40, BLUE);
  delay(50);
  // pause color
  display.fillRoundRect(39, 98, 20, 45, 5, RED);
  display.fillRoundRect(69, 98, 20, 45, 5, RED);
  // play color
  display.fillTriangle(42, 20, 42, 60, 90, 40, GREEN);
}

/**************************************************************************/
/*! 
    @brief  Renders a simple test pattern on the LCD
*/
/**************************************************************************/
void lcdTestPattern(void)
{
  uint32_t i,j;
  display.goTo(0, 0);
  
  for(i=0;i&lt;64;i++)
  {
    for(j=0;j&lt;96;j++)
    {
      if(i&gt;55){display.writeData(WHITE&gt;&gt;8);display.writeData(WHITE);}
      else if(i&gt;47){display.writeData(BLUE&gt;&gt;8);display.writeData(BLUE);}
      else if(i&gt;39){display.writeData(GREEN&gt;&gt;8);display.writeData(GREEN);}
      else if(i&gt;31){display.writeData(CYAN&gt;&gt;8);display.writeData(CYAN);}
      else if(i&gt;23){display.writeData(RED&gt;&gt;8);display.writeData(RED);}
      else if(i&gt;15){display.writeData(MAGENTA&gt;&gt;8);display.writeData(MAGENTA);}
      else if(i&gt;7){display.writeData(YELLOW&gt;&gt;8);display.writeData(YELLOW);}
      else {display.writeData(BLACK&gt;&gt;8);display.writeData(BLACK);}
    }
  }
}
