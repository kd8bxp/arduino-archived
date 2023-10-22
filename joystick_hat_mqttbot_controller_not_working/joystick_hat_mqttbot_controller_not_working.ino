#include "src/M5StickC/M5StickC.h"
#include "Wire.h"

#define JOY_ADDR 0x38



int8_t x_data, y_data, button_data;

void setup() {
    // initialize the display
    Serial.begin(115200);
    M5.begin();
    Wire.begin(0, 26, 100000UL);
    // clear the background
    M5.Lcd.setRotation(3);
    M5.Lcd.fillRect(0, 0, 80, 160, BLACK);
    M5.Lcd.setCursor(20,0);
    M5.Lcd.print("MQTTBot Controler");
   
}

void loop() {
    Wire.beginTransmission(JOY_ADDR);
    Wire.write(0x02);
    Wire.endTransmission();
    Wire.requestFrom(JOY_ADDR, 3);
    if (Wire.available()) {
        x_data      = Wire.read();
        y_data      = Wire.read();
        button_data = Wire.read();
    }

  M5.Lcd.setCursor(10,20);
  M5.Lcd.printf("X: %d   ", -x_data); //negitive because of orintation of device
  M5.Lcd.setCursor(60,20);
  M5.Lcd.printf("Y: %d   ", y_data);
  M5.Lcd.setCursor(20,35); 
  M5.Lcd.printf("Button: %d", button_data);

  x_data = map(-x_data, -127, 127, -1, 1);
  y_data = map(y_data, -127, 127, -1, 1);
  Serial.print("X: "); Serial.print(x_data); Serial.print(" Y: "); Serial.println(y_data);

  String tempMsg = "";

  if (x_data == 0 && y_data == 0) {tempMsg = "Stop"; goto screen;}
  if (x_data == 1  || y_data ==0 ) { tempMsg = "Forward"; goto screen;} 
  if (x_data == -1 || y_data == 0 ) { tempMsg = "Reverse"; goto screen;}
  if (y_data == -1 || x_data == 0 ) { tempMsg = "Left"; goto screen; }
  if (y_data >= 1 || x_data == 0 ) {tempMsg = "Right"; goto screen;}

screen:
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.print(tempMsg); M5.Lcd.print("        ");
  
}
