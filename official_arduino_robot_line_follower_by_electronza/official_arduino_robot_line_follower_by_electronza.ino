
/*
 * Teodor Costachioiu Aug 7 2015
 * https://electronza.com/arduino-robot-a-line-follower-code/
 * 
 */
 
#include <ArduinoRobot.h>
#include <Wire.h>
#include <SPI.h>

int maxspeed;
int pos;
int ontheline;

void setup() {
  Robot.begin();
}

void loop() {
  int maxspd = Robot.knobRead() ; // the value is in the range [0:1023]
  // map the value into an useful range
  maxspd = map(maxspd, 0, 1023, 50, 255); // minimum speed is 10

  //this function needs about 10 ms to complete
  Robot.updateIR();
  
  pos = 5; // no line at all
  ontheline = 1024;
  for (int i = 0; i < 5; i++) {
    // White background, black line 
    // the lowest value read is from the sensor over the line
    if (Robot.IRarray[i] < ontheline){
       ontheline = Robot.IRarray[i];
       pos = i;
    }
  }

  //one last check: have we lost the line
  if (Robot.IRarray[pos] > 500){
      pos = 5;
  }

  switch (pos) {
    case 0:    // Sharp turn right
      Robot.motorsStop();
      Robot.motorsWrite(maxspd, -maxspd);
      break;
    case 1:    // Turn right
      Robot.motorsStop();
      Robot.motorsWrite(maxspd, 0 );
      break;
    case 2:    // Go forward
      Robot.motorsStop();
      Robot.motorsWrite(maxspd, maxspd);
      break;
    case 3:    // Turn left
      Robot.motorsStop();
      Robot.motorsWrite(0, maxspd);
      break;
    case 4:    // Sharp turn right
      Robot.motorsStop();
      Robot.motorsWrite(-maxspd, maxspd);
      break;
    case 5:    // No line. Go slowly left 
      Robot.motorsStop();
      Robot.motorsWrite(50, 150);
      break;
  }
}
