//#include "src/M5Stack/M5Stack.h"
#include "src/M5GFX/M5UnitRCA.h"

#define M5STACKFIRE_MICROPHONE_PIN 34 //0
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
    //pinMode(M5STACKFIRE_MICROPHONE_PIN, INPUT_PULLUP);
    analogReadResolution(10);
    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);
    gfx_rca.startWrite();
    //gfx_rca.setTextColor(TFT_WHITE, TFT_BLACK);
    //gfx_rca.drawCenterString("M5 RCA Module :)", 120, 50);
}


void loop() {
    
for (int i=0; i<HORIZONTAL; i++) {
       int /*float*/ readMicValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);
	   adc_voltage  = (readMicValue * ref_voltage) / 1024.0;
       in_voltage = (adc_voltage*(R1+R2)/R2); // * 2.5;
       Serial.print(readMicValue); //in_voltage);
       Serial.print(','); Serial.print(in_voltage); Serial.print(',');
       //int temp = in_voltage;
       //int temp = map(in_voltage , 0.0, 5.0 /*24.0 1024.0*/, 80.0, 10.0); //works sq wave
       //float temp = map(in_voltage , 0.0, 5.0 /*24.0 1024.0*/, 80.0, 10.0); //works sq wave
       //int temp = map(readMicValue, 0, 100, 0, 80); //works wrong position on screen large output
       int temp = map(readMicValue, 0, 200, 0, 160); //works, display is inverted, works with (oldCapture[i+1]/2)+75 etc

       //add some random noise
       if (temp == 0) {capture[i] = random(0,5); } else {
       capture[i] = temp; }
       Serial.println(temp); 
   }
        
for (int i=1; i<HORIZONTAL-1; i++) {
  //if (oldCapture[i+1] == 0 || oldCapture[i] == 0) {
   // gfx_rca.drawLine(i, (oldCapture[i+1]/2)+75, i, (oldCapture[i]/2) + 75, BLACK);
 // } else {
      gfx_rca.drawLine(i, (oldCapture[i+1]/2)+75, i, (oldCapture[i]/2)+75, BLACK);
      gfx_rca.drawLine(i, (capture[i+1]/2)+75, i, (capture[i]/2)+75, GREEN);
  //}
      oldCapture[i] = capture[i];
  }
 
}
