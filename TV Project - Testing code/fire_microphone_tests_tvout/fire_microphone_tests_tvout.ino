#include "src/M5Stack/M5Stack.h"
#include "src/M5GFX/M5UnitRCA.h"
#include "M5_ADS1100.h"

#define M5STACKFIRE_MICROPHONE_PIN 34
#define HORIZONTAL 216

M5UnitRCA gfx_rca;
ADS1100 ads;

int micValue = 0;
double capture[HORIZONTAL]; 
double oldCapture[HORIZONTAL];
int16_t result;

void setup() {
    Serial.begin(115200);
    M5.begin();
    M5.Power.begin();
    pinMode(M5STACKFIRE_MICROPHONE_PIN, INPUT);
    analogReadResolution(10);
    
    ads.getAddr_ADS1100(ADS1100_DEFAULT_ADDRESS);  // 0x48, 1001 000 (ADDR = GND)
// The ADC gain (PGA).  ADC增益(PGA)
    //ads.setGain(GAIN_ONE);  // 1x gain(default)
    // ads.setGain(GAIN_TWO);       // 2x gain
     ads.setGain(GAIN_FOUR);      // 4x gain
    // ads.setGain(GAIN_EIGHT);     // 8x gain
ads.setMode(MODE_SINGLE); //MODE_CONTIN);  // Continuous conversion mode (default)
ads.setRate(RATE_32);  
 ads.setOSMode(OSMODE_SINGLE);  
 ads.begin();
     
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
     xTaskCreatePinnedToCore(readLoop, "readLoop", 8192, NULL, 1, NULL, 1);
     xTaskCreatePinnedToCore(displayLoop, "displayLoop", 8192, NULL, 1, NULL, 1);
}

int readADC() {
  byte error;
    int8_t address;

    address = ads.ads_i2cAddress;
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)  // If the device is connected.  如果连接上设备
    {
        
        result = ads.Measure_Differential();
        M5.Lcd.fillScreen(BLACK);
        //char data[20] = {0};
        //sprintf(data, "%d", result);
        //M5.Lcd.drawCentreString(data, 160, 100, 4);
    } else {
        M5.Lcd.drawString("No Found ADC sensor.", 20, 100, 2);
    }
    return (result);
}

void readLoop(void*) {
 while(true) {   
    for (int i=0; i<HORIZONTAL; i++) {
     //int readMicValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);
     int readMicValue = readADC();
     //Serial.println(readMicValue );
      

      int temp = map(readMicValue , 0, 100, 0,175);
       //int temp = map(readMicValue , 0, 1024, 0,175);
      Serial.println(readMicValue);
      capture[i] = temp; //abs(temp); //readMicValue;
   }
   vTaskDelay(1);
 }
}    

void displayLoop(void*) {
  while(true) {    
for (int i=1; i<HORIZONTAL-1; i++) {

  gfx_rca.drawLine(i, oldCapture[i+1], i, oldCapture[i], BLACK);
  gfx_rca.drawLine(i, capture[i+1], i, capture[i], GREEN);

 
    oldCapture[i] = capture[i];
   }
 
    vTaskDelay(1);
   }
}

void loop() {
  

}
