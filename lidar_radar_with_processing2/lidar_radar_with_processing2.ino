/*LiDar Radar Project - by LeRoy Miller 2018
 * Based on The Arduino Radar Project found:
 * https://howtomechatronics.com/projects/arduino-radar-project/
 * updated to use a TFMini LiDar Sensor.
 * 
 */

#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library

#define pan 0
#define tilt 1
#define SERVOMIN  125 
#define SERVOMAX  575

SoftwareSerial Serial1(8,9); // RX, TX
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Variables for the duration and the distance

unsigned int distance;

void setup() {

  // Debug serial
  Serial.begin( 9600 );

  // Serial connected to LIDAR sensor
  Serial1.begin( 115200 );
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  delay( 5000 );
 // Serial.println( "Starting..." );

pwm.setPWM(tilt,0,angleToPulse(95));
}

void loop() {

   // rotates the servo motor from 15 to 165 degrees
  for(int i=15;i<=160;i++){  
  pwm.setPWM(pan,0,angleToPulse(i));
  
  //distance = readLIDAR( 2000 );
  distance = map(readLIDAR(2000),30,1200,1,39);
  //distance = random(1,39);
  delay(30);
  Serial.print(i); // Sends the current degree into the Serial Port
  Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  Serial.print(distance); // Sends the distance value into the Serial Port
  Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  }
    for(int i=160;i>15;i--){  
  pwm.setPWM(pan,0,angleToPulse(i));
  
  //distance = readLIDAR( 2000 );
  distance = map(readLIDAR(2000),30,1200,0,39);
  //distance = random(1,39);
  delay(30);
  Serial.print(i);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
  }
  
}


/*
 * This function reads the Serial1 until a valid packet is found or timeout passed.
 * Param timeout: Timeout in milliseconds.
 * Returns distance in cm or 0 if timeout happened.
 */
unsigned int readLIDAR( long timeout ) {

  unsigned char readBuffer[ 9 ];

  long t0 = millis();

  while ( Serial1.available() < 9 ) {

    if ( millis() - t0 > timeout ) {
      // Timeout
      return 0;
    }

    delay( 10 );
  }

  for ( int i = 0; i < 9; i++ ) {
    readBuffer[ i ] = Serial1.read();
  }

  while ( ! detectFrame( readBuffer ) ) {

    if ( millis() - t0 > timeout ) {
      // Timeout
      return 0;
    }

    while ( Serial1.available() == 0 ) {
      delay( 10 );
    }

    for ( int i = 0; i < 8; i++ ) {
      readBuffer[ i ] = readBuffer[ i + 1 ];
    }

    readBuffer[ 8 ] = Serial1.read();

  }

  // Distance is in bytes 2 and 3 of the 9 byte frame.
  unsigned int distance = ( (unsigned int)( readBuffer[ 2 ] ) ) |
                          ( ( (unsigned int)( readBuffer[ 3 ] ) ) << 8 );

  return distance;

}

bool detectFrame( unsigned char *readBuffer ) {

  return  readBuffer[ 0 ] == 0x59 &&
          readBuffer[ 1 ] == 0x59 &&
          (unsigned char)(
            0x59 +
            0x59 +
            readBuffer[ 2 ] + 
            readBuffer[ 3 ] + 
            readBuffer[ 4 ] +
            readBuffer[ 5 ] + 
            readBuffer[ 6 ] + 
            readBuffer[ 7 ]
          ) == readBuffer[ 8 ];
}

int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
   //Serial.print("Angle: ");Serial.print(ang);
   //Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}
