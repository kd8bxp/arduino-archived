#include "src/M5Stack/M5Stack.h"
#include "src/M5GFX/M5UnitRCA.h"

#define M5STACKFIRE_MICROPHONE_PIN 0
#define HORIZONTAL 216

M5UnitRCA gfx_rca;

int micValue = 0;
double capture[HORIZONTAL]; 
double oldCapture[HORIZONTAL];

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 3.3; //5.0;
 
// Integer for ADC value
int adc_value = 0;
 


void setup() {
    Serial.begin(115200);
    M5.begin();
    M5.Power.begin();
    
    //analogReadResolution(10);
   
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
	   adc_voltage  = (readMicValue * ref_voltage) / 1024.0;
       in_voltage = (adc_voltage*(R1+R2)/R2) * 2.0;
       int temp = in_voltage;
       //int temp = map(readMicValue , 0, 1024, 0,80); //175
       Serial.println(temp); 
       capture[i] = temp; 
   }
        
for (int i=1; i<HORIZONTAL-1; i++) {
      gfx_rca.drawLine(i, 42+oldCapture[i+1], i, 42+oldCapture[i], BLACK);
      gfx_rca.drawLine(i, 42+capture[i+1], i, 42+capture[i], GREEN);
      oldCapture[i] = capture[i];
  }
 
}
