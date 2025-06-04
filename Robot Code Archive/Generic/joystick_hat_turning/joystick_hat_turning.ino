#include "src/M5StickC/M5StickC.h"
#include "Wire.h"

#define JOY_ADDR 0x38

#define VERTOFFSET 60
#define HORZOFFSET 0 //35
#define LOWSPEED 25
#define MIDSPEED 35
#define HIGHSPEED 100 //125

int turn, forward, joyX, joyY;

int8_t x_data, y_data, button_data;

void setup() {
    // initialize the display
    M5.begin();
    Serial.begin(115200);
    Wire.begin(0, 26, 100000UL);
    // clear the background
    M5.Lcd.setRotation(3);
    M5.Lcd.fillRect(0, 0, 80, 160, BLACK);

   
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

if (!button_data) {
    Serial.println("Reverse.");
    turn = 999; forward = 999; joyX = 999; joyY = 999;
  } else {
  Serial.print("X: ");
  Serial.print(x_data); Serial.print(" ");
  
  int x = -x_data;
  int xx;
  if (x == HORZOFFSET) {joyX = map(x, 0,1023,0,0); Serial.print(" "); Serial.print(joyX); xx=0;}
  if (x > HORZOFFSET) {joyX = map(x, HORZOFFSET, 1023, LOWSPEED, MIDSPEED); Serial.print(" "); Serial.print(joyX); xx=1;}
  if (x < HORZOFFSET) {joyX = map(x, 0, HORZOFFSET, MIDSPEED, LOWSPEED); Serial.print(" "); Serial.print(joyX); xx=-1;}
  //int xx = map(x,0,1000,-1,1);
  if (xx == 0) {Serial.print(" Stopped.");}
  if (xx == -1) {Serial.print(" Right."); }
  if (xx == 1) {Serial.print(" Left."); }
  turn = xx;

  Serial.print(" Y: ");
  int y = y_data;
  //int yy = map(y, 0, 1023, -1, 1);
  int yy;
  if (y == VERTOFFSET) { joyY = map(y,0,1023,0,0); } else {
  joyY = map(y,VERTOFFSET,1023, LOWSPEED, HIGHSPEED); }
  Serial.print(y);
  if (y <= VERTOFFSET) {yy=0;} else {yy=1;}
  if (yy <= 0) {Serial.print(" Stopped."); }
  if (yy >0) {Serial.print(" Forward. "); Serial.print(y); }
  Serial.println("");
  forward = yy;
  }


  M5.Lcd.setCursor(10,20);
  M5.Lcd.printf("X: %d   ", -x_data); //negitive because of orintation of device
  M5.Lcd.setCursor(60,20);
  M5.Lcd.printf("Y: %d   ", y_data);
  M5.Lcd.setCursor(0,30); 
  M5.Lcd.printf("Button: %d", button_data);
  
}
