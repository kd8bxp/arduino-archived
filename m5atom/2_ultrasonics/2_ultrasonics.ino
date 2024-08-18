#include "src/M5Atom/M5Atom.h"
#include "src/Unit_Sonic/Unit_Sonic.h"
#include "Wire.h"

#define PAHUB_ADDR 0X70

SONIC_I2C sensor;

void portselectall(uint8_t ports) {  
  Wire.beginTransmission(PAHUB_ADDR);
  Wire.write(ports&0x3f);
  Wire.endTransmission(); 
}


//Hub range is 0 to 5
void portselect(uint8_t i) {
  if (i > 7) return;
  
  Wire.beginTransmission(PAHUB_ADDR);
  Wire.write(1 << i);
  Wire.endTransmission(); 
}

void grabDistance() {
   static float Distance = 0;
    Distance              = sensor.getDistance();
    if ((Distance < 4000) && (Distance > 20)) {
        Serial.printf("Distance: %.2fmm\r\n", Distance);
    }
    delay(100);
}

void setup() {
  M5.begin(true, true, false);
  //M5.begin();
  sensor.begin(); 
  Serial.begin(115200);
  //Wire.begin();

}

void loop() {
  Serial.print("Sensor 0: ");
  portselect(int(0));
  delay(1);
  grabDistance();

  Serial.print("Sensor 1: ");
  portselect(int(5));
  delay(1);
  grabDistance();

  delay(1000);
}
