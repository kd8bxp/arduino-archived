/*
* LAB: 11
* Name: ESP32 ADC Multi-Sampling & Moving Average Filter
* Author: Khaled Magdy
* For More Info Visit: www.DeepBlueMbedded.com
*/
 
#include "esp_adc_cal.h"
#include "src/M5GFX/M5UnitRCA.h"

#define HORIZONTAL 216

M5UnitRCA gfx_rca;

 double capture[HORIZONTAL]; 
double oldCapture[HORIZONTAL];
double captureFilter[HORIZONTAL];
double oldCaptureFilter[HORIZONTAL];

#define AN_Pot1     34 //35
#define FILTER_LEN  5
 
uint32_t AN_Pot1_Buffer[FILTER_LEN] = {0};
int AN_Pot1_i = 0;
int AN_Pot1_Raw = 0;
int AN_Pot1_Filtered = 0;


 
void setup()
{
  Serial.begin(115200);
  analogReadResolution(10);
  //analogSetAttenuation(ADC_6db);
  gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);
    gfx_rca.startWrite();
    //gfx_rca.setTextColor(TFT_WHITE, TFT_BLACK);
    //gfx_rca.drawCenterString("M5 RCA Module :)", 120, 50);
}
 
void loop()
{
  for (int i=0; i<HORIZONTAL; i++) {
    
  AN_Pot1_Raw = analogRead(AN_Pot1);
  AN_Pot1_Filtered = readADC_Avg(AN_Pot1_Raw);
  Serial.print(AN_Pot1_Raw);        // Print Raw Reading
  Serial.print(',');                // Comma Separator
  Serial.print(AN_Pot1_Filtered); // Print Filtered Output
  Serial.print(',');
  int temp = map(AN_Pot1_Raw, 0, 1024, 0, 80);
  capture[i] = temp; //raw
  captureFilter[i] = map(AN_Pot1_Raw, 0, 1024, 80, 0);
  
  Serial.println(temp);
  //delay(1);
}

for (int i=1; i<HORIZONTAL-1; i++) {
      gfx_rca.drawLine(i, oldCapture[i+1], i, oldCapture[i], BLACK);
      
      gfx_rca.drawLine(i, capture[i+1], i, capture[i], GREEN);
      gfx_rca.drawLine(i, oldCaptureFilter[i+1], i, oldCaptureFilter[i], BLACK);
      gfx_rca.drawLine(i, captureFilter[i+1], i, captureFilter[i], GREEN);
      oldCapture[i] = capture[i];
      oldCaptureFilter[i] = captureFilter[i];
  }
 
}


 
uint32_t readADC_Avg(int ADC_Raw)
{
  int i = 0;
  uint32_t Sum = 0;
  
  AN_Pot1_Buffer[AN_Pot1_i++] = ADC_Raw;
  if(AN_Pot1_i == FILTER_LEN)
  {
    AN_Pot1_i = 0;
  }
  for(i=0; i<FILTER_LEN; i++)
  {
    Sum += AN_Pot1_Buffer[i];
  }
  return (Sum/FILTER_LEN);
}
