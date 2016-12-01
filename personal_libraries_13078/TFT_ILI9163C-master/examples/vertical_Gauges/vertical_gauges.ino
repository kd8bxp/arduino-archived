//example adapted from somewhere but cannot remember!
//If the author recognize it drop me a note!

#include &lt;SPI.h&gt;
#include &lt;Adafruit_GFX.h&gt;
#include &lt;TFT_ILI9163C.h&gt;

// Color definitions
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

#define  NBINS                 8
const uint8_t  bar_Width =     7;
uint32_t       avrg_TmrF =     0;
uint16_t       t_b[NBINS];

/*
Teensy3.x and Arduino's
You are using 4 wire SPI here, so:
 MOSI:  11//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 MISO:  12//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 SCK:   13//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 the rest of pin below:
 */
#define __CS 10
#define __DC 9
/*
Teensy 3.x can use: 2,6,9,10,15,20,21,22,23
Arduino's 8 bit: any
DUE: check arduino site
If you do not use reset, tie it to +3V3
*/


TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC);


void setup(void) {
  Serial.begin(38400);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.setTextWrap(true);
  tft.setTextColor(WHITE,BLACK);
  tft.setCursor(0,0);
  Draw_Table();
}


void loop(){
  for (int i=0;i&lt;NBINS;i++){
    t_b[i] = random(0,4096);
  }
  Print_Data();
  delay(100);
}

void  Draw_Table(void) 
{  
  tft.drawFastVLine( 22, 0, 128, WHITE);    // Draw a Scale
  tft.drawFastHLine( 20, 0,           4, WHITE);  
  for ( int i = 10; i &lt; 128; i += 10 ) {
    tft.drawFastHLine( 20, i, 4, WHITE); 
    tft.setCursor( 0, i - 3);               
    //if ( i &lt; 60 )
    if ( i &lt; 120 )
      tft.print( i * 1.2,0);          
    else  
      tft.print(&quot; dB&quot;);
  }
  tft.setCursor( 96,  1);                      // Digital display on right side
  tft.print(&quot;T H D&quot;);

  tft.setCursor( 96, 23);
  tft.print(&quot;F R Q&quot;);

  tft.setCursor( 96, 45);
  tft.print(&quot;R M S&quot;);
}


void Print_Data(void)
{
  float frequency =  0.0;
  float total_thd =  0.0;
  float voltag_ac =  0.0;
  
  avrg_TmrF &gt;&gt;= 4;
  if (avrg_TmrF != 0) frequency = (8.0 * 16000000.0) / avrg_TmrF;
  avrg_TmrF = 0;
  //--------------------- FREQ ---------------
  tft.setCursor(96,33);
  if (frequency &lt; 99) {
    tft.print(frequency,2);
    }
  else{
    tft.print(&quot;...&quot;);
    }

/*THD: Total Harmonic Distortion. The harmonic distortion characterises the ratio of the sum of the
harmonics to the fundamental signal. Normally there are the first 6 harmonics used for the
characterisation.
THD = 20 * log (SQRT (SUM (SQR ([Harmonics]))) / [Fundamental])*/
// ---------------- Vertical VU's ------------------------------------------
  uint32_t total1 = 0;            // ALL
  uint32_t total2 = 0;            // All, Except Fundamental (1).
  uint16_t fnd = 0;            // Fundamental

  for (int i = 1; i &lt; NBINS; i++) {
    int st1 = (i * 10) + 15;  // k = 70 / (NBINS -1)
    tft.drawRect((st1-1),0,bar_Width,128,WHITE); // Volume
    uint32_t vremn1 = t_b[i] &gt;&gt; 4;                           // V(i) / updt_Rate                 
    uint32_t vremn2 = vremn1 * vremn1;                       // V(i) ^ 2.
    total1 +=  vremn2;                                   // Total1 = V1^2 + V2^2 + V3^2 + V4^2 + V5^2
    if (i != 1) 
       total2 +=  vremn2;                                // Total2 =        V2^2 + V3^2 + V4^2 + V5^2
    else 
       fnd = vremn1;                                     // Fundamental = V1
    vremn2 =  20 * log10(vremn1+1);                     // !!! +1 MUST,
    int st2 = map(vremn2,0,73,(128-2),0);                             // 73 dB  
    tft.fillRect(st1,1,(bar_Width-2),st2,BLACK);                            // Empty  
    tft.fillRect(st1,(st2 + 2),(bar_Width-2),(128-2-st2),GREEN); // Fill Up  
    t_b[i] = 0;      
}

  voltag_ac = sqrt(total1) / 20.27;               // Hardware Calibration Coefficient  /0.39752907
  //-------------RMS--------------------
  tft.setCursor(96,55);
  if (voltag_ac &lt; 999) {
    tft.print(voltag_ac,1);
    }
  else{
    tft.print( &quot;...&quot;);
    }
    
  total_thd = 100.0 * sqrt(total2) / fnd;
 //-------------THD ------------------------ 
  tft.setCursor(96,11);
  if (total_thd &lt; 9) {
    tft.print(total_thd,3);
    }
  else{
    tft.print( &quot;...&quot;);
    }
}