#include <Driver_IRPod.h>

/*
 This example uses the Robonii IR Pod driver to control the LED’s on the IR pod, by communicating to it using the TWI interface.
 */


void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
  pinMode(14, OUTPUT);     
 
  
  digitalWrite(13, HIGH);   // set the LED off
  digitalWrite(14, HIGH);   // set the LED off
  
     
  delay(400);
  Robonii_IRPod.InitTWI();
      
  delay(1000);
  Robonii_IRPod.SetLED(IRPOD_LED_BICOLOR_GREEN,IRPOD_LED_ON);
  Robonii_IRPod.SetLED(IRPOD_LED_RED_LEFT,IRPOD_LED_FLASH_FAST);
  
  Robonii_IRPod.SetLED(IRPOD_LED_RED_LEFT,IRPOD_LED_FLASH_FAST);
  delay(5000);
  Robonii_IRPod.SetLED(IRPOD_LED_RED_RIGHT,IRPOD_LED_FLASH_SLOW);
  Robonii_IRPod.SetLED(IRPOD_LED_BICOLOR_RED,IRPOD_LED_FLASH_FAST);
  
}

unsigned char DataCount = 0;
unsigned char DataArr[10];


void loop() {
  digitalWrite(13, HIGH);   // set the LED on
  delay(1000);              // wait for a second
  Robonii_IRPod.IrpodReqVersion();
  digitalWrite(13, LOW);    // set the LED off
  delay(1000);              // wait for a second
  
  
  if (DataCount=Robonii_IRPod.ReadData(DataArr))
  {
    char i = 0;
    Serial.print("Bytes of Data Received: ");
     Serial.println(DataCount);
    for (i=0;i<DataCount;i++)
    { 
      Serial.println(DataArr[i]); 
    }
  } 
  
}





