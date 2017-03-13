#include <Servo.h>
#include <Wire.h>

#define TILT_SERVO 7
#define PAN_SERVO 8

#define H_SIZE 40 
#define V_SIZE 30
#define Xcenter 150
#define Ycenter 50
#define SETTIME 10

Servo servo_tilt;
Servo servo_pan;

void goto_origin(){
  servo_tilt.write( Xcenter ); // Tilt servo at top.
  servo_pan.write( Ycenter );  // Pan servo to left.
  delay(SETTIME);                            // time to reach origin.
}

void setup() {
  servo_tilt.attach(TILT_SERVO); 
  servo_pan.attach(PAN_SERVO);
  Serial.begin(9600);
  
}

void loop() {
       goto_origin();                            // Set (0,0) position.

  for (int y=V_SIZE-1 ; y>=0 ; y--) { 
    
    //servo_tilt.write(Ycenter + V_SIZE/2 -y );
    delay(SETTIME);                              // time to reach position.   
    for ( int x=0 ; x<H_SIZE ; x++) {            // Scan from Left to Right.
      servo_pan.write( Xcenter + x);
      servo_tilt.write(Ycenter + x);
    delay(SETTIME);                            // time to reach position.
           
    }
    y--;                                         // Next line.
    //servo_tilt.write(Ycenter + V_SIZE/2 - y);
    delay(SETTIME);                              // time to reach position.    
    for ( int x=H_SIZE-1 ; x>=0 ; x--) {         // Scan from Right to Left.
      servo_pan.write(H_SIZE/2 + x);
      servo_tilt.write(V_SIZE/2 + x);     
     delay(SETTIME);                            // time to reach position.
    
    } 
  }

}
