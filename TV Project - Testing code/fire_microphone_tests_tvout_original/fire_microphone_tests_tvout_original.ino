#include "src/M5Stack/M5Stack.h"
#include "src/M5GFX/M5UnitRCA.h"

#define M5STACKFIRE_MICROPHONE_PIN 34
#define HORIZONTAL 216
#define SPREAD 1 //1 works good, 0 produces interesting effects

M5UnitRCA gfx_rca;

int micValue = 0;
double capture[HORIZONTAL]; 
double oldCapture[HORIZONTAL];


void setup() {
    Serial.begin(115200);
    M5.begin();
    M5.Power.begin();
    analogReadResolution(10);
   
    M5.Lcd.begin();
    M5.Lcd.fillScreen(BLACK);
    M5.Speaker.begin();
    M5.Speaker.setVolume(128);
    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);
    gfx_rca.startWrite();
}


void loop() {
    
for (int i=0; i<HORIZONTAL; i++) {
       int readMicValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);
       int temp = map(readMicValue , 0, 1024, 0,80); //175
       Serial.println(temp); //readMicValue);
       capture[i] = temp; //abs(temp); //readMicValue;
   }
        
for (int i=1; i<HORIZONTAL-SPREAD; i++) {

      // This doesn't really work, it sort of works but needs to be at the bottom of the screen      
      //gfx_rca.drawFastVLine(i,100,oldCapture[i]/2, GREEN);
      //gfx_rca.drawFastVLine(i,100,capture[i]/2, BLACK);

      // Makes a better circle then the circle below
      //gfx_rca.drawEllipse(108,65,oldCapture[i]/2,oldCapture[i]/2, BLACK);
      //gfx_rca.drawEllipse(108,65,capture[i]/2,capture[i]/2, GREEN);
      
      // Interesting effect, but it's slow
      //gfx_rca.drawArc(108, 65, 0/*+oldCapture[i+SPREAD]/5*/, 25+oldCapture[i+SPREAD]/5, ((360*i) * oldCapture[i+SPREAD])/0.75, ((360 -i) *oldCapture[i+SPREAD])/0.5, BLACK);
      //gfx_rca.drawArc(108, 65, 0/*+capture[i+SPREAD]/5*/, 25+capture[i+SPREAD]/5, ((360*i) * capture[i+SPREAD])/0.75, ((360-i) * capture[i+SPREAD])/0.5, GREEN);

      // Makes a circle
      //gfx_rca.drawCircle(108, 65, oldCapture[i+SPREAD]/5, BLACK);
      //gfx_rca.drawCircle(108, 65, capture[i+SPREAD]/5, GREEN);
      
      // Draws the voice line
      gfx_rca.drawLine(i, 42+oldCapture[i+SPREAD], i, 42+oldCapture[i], BLACK);
      gfx_rca.drawLine(i, 42+capture[i+SPREAD], i, 42+capture[i], GREEN);
      oldCapture[i] = capture[i];
  }
 
}
