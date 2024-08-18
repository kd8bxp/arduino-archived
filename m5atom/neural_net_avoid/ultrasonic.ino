//  the original code by Ted Meyers
//  posted here: https://groups.google.com/d/msg/diyrovers/lc7NUZYuJOg/ICPrYNJGBgAJ

//  if your tof have some problem, please see https://docs.m5stack.com/#/en/unit/tof 
// June 10, 2022 slightly modified to use two sensors on Wire & Wire1 buses (LeRoy Miller, KD8BXP)
// May 27, 2024 modified to work with M5Stack I2C Ultrasonic sensors with the PaHub(I2C hub)

void readRight() {
//Read Sensor A 
  byte val = 0;
  //int cnt = 0;
  //while (cnt < 100) { // 1 second waiting time max
   // delay(10);
   Serial.print("Sensor Right (2): ");
    portselect(int(5));
    delay(1);
    dist = grabDistance();
   // cnt++;
  }  
//}

void readLeft() {
//Read Sensor B
 
  byte val = 0;
  //int cnt = 0;
  //while (cnt < 100) { // 1 second waiting time max
   // delay(10);
   Serial.print("Sensor Left (1): ");
    portselect(int(0));
    delay(1);
    dist = grabDistance();
   // cnt++;
  }
//}

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

int grabDistance() {
   static float Distance = 0;
    Distance              = sensor.getDistance();
    if ((Distance < 4000) && (Distance > 20)) {
        Serial.printf("Distance: %.2fmm\r\n", Distance);
        return Distance;
    }
    //delay(100);
    return 0;
}
