#define LCD_CS A3    
#define LCD_CD A2    
#define LCD_WR A1   
#define LCD_RD A0    
// you can also just connect RESET to the arduino RESET pin
#define LCD_RESET A4
//Duemilanove/Diecimila/UNO/etc ('168 and '328 chips) microcontoller:
// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF

#include &quot;TFTLCD.h&quot;

TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  Serial.begin(9600);
  Serial.println(&quot;8 Bit LCD test!&quot;);
  
  
  tft.reset();
  
  //uint16_t identifier = tft.readRegister(0x0);
//  if (identifier == 0x9325) 
//  {
 //   Serial.println(&quot;Found ILI9325&quot;);
 // } else if (identifier == 0x9328) 
  //{
  //  Serial.println(&quot;Found ILI9328&quot;);
  //} else 
 // {
 //   Serial.print(&quot;Unknown driver chip &quot;);
 //   Serial.println(identifier, HEX);
  //  while (1);
  //}  
 
  tft.initDisplay();
  
  testtext(RED);
  delay(2000);
  testlines(CYAN);
  delay(500);
  testfastlines(RED, BLUE);
  delay(500);
  testdrawrects(GREEN);
  delay(500);
  testfillrects(YELLOW, MAGENTA);
  delay(500);
  tft.fillScreen(BLACK);
  testfillcircles(10, MAGENTA);
  testdrawcircles(10, WHITE);
  delay(500); 
  testtriangles();
  delay(500); 
  testfilltriangles();
  delay(500); 
  testRoundRect();
  delay(500); 
  testFillRoundRect();
}

void loop(void) {
  for (uint8_t rotation=0; rotation&lt;4; rotation++) {
    tft.setRotation(rotation); 
    testtext(RED);
    delay(1000);
  }
}



void testFillRoundRect() {
  tft.fillScreen(BLACK);
  
  for (uint16_t x=tft.width(); x &gt; 20 ; x-=6) {
    tft.fillRoundRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, x/8,  tft.Color565(0, x, 0));
 }
}

void testRoundRect() {
  tft.fillScreen(BLACK);
  
  for (uint16_t x=0; x &lt; tft.width(); x+=6) {
    tft.drawRoundRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, x/8, tft.Color565(x, 0, 0));
 }
}

void testtriangles() {
  tft.fillScreen(BLACK);
  for (uint16_t i=0; i&lt;tft.width()/2; i+=5) {
    tft.drawTriangle(tft.width()/2, tft.height()/2-i,
                     tft.width()/2-i, tft.height()/2+i,
                     tft.width()/2+i, tft.height()/2+i, tft.Color565(0, 0, i));
  }
}

void testfilltriangles() {
  tft.fillScreen(BLACK);
  
  for (uint16_t i=tft.width()/2; i&gt;10; i-=5) {
    tft.fillTriangle(tft.width()/2, tft.height()/2-i,
                     tft.width()/2-i, tft.height()/2+i,
                     tft.width()/2+i, tft.height()/2+i, 
                     tft.Color565(0, i, i));
    tft.drawTriangle(tft.width()/2, tft.height()/2-i,
                     tft.width()/2-i, tft.height()/2+i,
                     tft.width()/2+i, tft.height()/2+i, tft.Color565(i, i, 0));    
  }
}
void testtext(uint16_t color) {
  tft.fillScreen(BLACK);
  tft.setCursor(0, 20);
  tft.setTextColor(color);
  tft.setTextSize(1);
  tft.println(&quot;Hello World!&quot;);
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint16_t x=radius; x &lt; tft.width(); x+=radius*2) {
    for (uint16_t y=radius; y &lt; tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }  
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (uint16_t x=0; x &lt; tft.width()+radius; x+=radius*2) {
    for (uint16_t y=0; y &lt; tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }  
}


void testfillrects(uint16_t color1, uint16_t color2) {
 tft.fillScreen(BLACK);
 for (uint16_t x=tft.width()-1; x &gt; 6; x-=6) {
   //Serial.println(x, DEC);
   tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
   tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
 }
}

void testdrawrects(uint16_t color) {
 tft.fillScreen(BLACK);
 for (uint16_t x=0; x &lt; tft.width(); x+=6) {
   tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
 }
}

void testfastlines(uint16_t color1, uint16_t color2) {
   tft.fillScreen(BLACK);
   for (uint16_t y=0; y &lt; tft.height(); y+=5) {
     tft.drawHorizontalLine(0, y, tft.width(), color1);
   }
   for (uint16_t x=0; x &lt; tft.width(); x+=5) {
     tft.drawVerticalLine(x, 0, tft.height(), color2);
   }
  
}

void testlines(uint16_t color) {
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x &lt; tft.width(); x+=6) {
     tft.drawLine(0, 0, x, tft.height()-1, color);
   }
   for (uint16_t y=0; y &lt; tft.height(); y+=6) {
     tft.drawLine(0, 0, tft.width()-1, y, color);
   }
   
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x &lt; tft.width(); x+=6) {
     tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
   }
   for (uint16_t y=0; y &lt; tft.height(); y+=6) {
     tft.drawLine(tft.width()-1, 0, 0, y, color);
   }
   
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x &lt; tft.width(); x+=6) {
     tft.drawLine(0, tft.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y &lt; tft.height(); y+=6) {
     tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
   }

   tft.fillScreen(BLACK);
   for (uint16_t x=0; x &lt; tft.width(); x+=6) {
     tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y &lt; tft.height(); y+=6) {
     tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
   }
}


void testBars() {
  uint16_t i,j;
  for(i=0; i &lt; tft.height(); i++)
  {
    for(j=0; j &lt; tft.width(); j++)
    {
      if(i&gt;279) tft.writeData(WHITE);
      else if(i&gt;239) tft.writeData(BLUE);
      else if(i&gt;199) tft.writeData(GREEN);
      else if(i&gt;159) tft.writeData(CYAN);
      else if(i&gt;119) tft.writeData(RED);
      else if(i&gt;79) tft.writeData(MAGENTA);
      else if(i&gt;39) tft.writeData(YELLOW);
      else tft.writeData(BLACK);
    }
  }
}
