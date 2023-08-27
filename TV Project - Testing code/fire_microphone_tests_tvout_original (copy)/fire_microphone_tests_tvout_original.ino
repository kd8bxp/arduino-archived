#include "src/M5Stack/M5Stack.h"
#include "src/M5GFX/M5UnitRCA.h"

#define M5STACKFIRE_MICROPHONE_PIN 34
#define HORIZONTAL 216

M5UnitRCA gfx_rca;

int micValue = 0;
double capture[HORIZONTAL]; 
double oldCapture[HORIZONTAL];


void setup() {
    Serial.begin(115200);
    M5.begin();
    M5.Power.begin();
    analogReadResolution(10);
    // WiFi.mode(WIFI_OFF);
    // btStop();
    //dacWrite(M5STACKFIRE_SPEAKER_PIN,0);  // make sure that the speaker is quite
   // dac_output_disable(DAC_CHANNEL_1);
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
     
      micValue = readMicValue; //temp;
      if (micValue > 1950 && micValue < 2250) { readMicValue = 1950; }
      //if (micValue < 1800 ) { readMicValue = 1950; }
      //if (micValue < 1700 ) {readMicValue = 1700; }
      //if (micValue > 2250 ) {readMicValue = 2250; }
      //int temp = map(readMicValue, 1700, 4000, 60, 240);
        //int temp = map(readMicValue, 1700, 4000, 60, 125);
        //capture[i] = abs(temp); //readMicValue;
        int temp = map(readMicValue , 0, 1024, 0,175);
      Serial.println(readMicValue); capture[i] = temp; //abs(temp); //readMicValue;
   }
    
    
for (int i=1; i<HORIZONTAL-1; i++) {

  gfx_rca.drawLine(i, oldCapture[i+1], i, oldCapture[i], BLACK);
  gfx_rca.drawLine(i, capture[i+1], i, capture[i], GREEN);

 
    oldCapture[i] = capture[i];
}
 
    //delay(1);
}
