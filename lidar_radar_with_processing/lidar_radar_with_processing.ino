/*
 * TFMini LIDAR Sensor test skecth for ESP32
 * 
 * Juan Jose Luna Espinosa 2018
 * https://github.com/yomboprime/TFMiniArduinoTest
 * 
 * Released under public domain
 * 
 * 
 * Connections:
 * Green cable to RX in the ESP32 (UART 2, pin 16)
 * Red cable to 5V
 * Black cable to GND
 * Green cable is not needed.
 * 
 * 9 byte frame of the sensor:
 * Byte 0: Always is 0x59 
 * Byte 1: Always is 0x59 
 * Byte 2: Distance in cm, low byte
 * Byte 3: Distance in cm, high byte
 * Byte 4: Signal strength, low byte
 * Byte 5: Signal strength, high byte
 * Byte 6: Reserved byte
 * Byte 7: Raw signal quality
 * Byte 8: Checksum of the 8 previous bytes.
 */

#include <SoftwareSerial.h>
#include <Servo.h>

#define panPin 7
#define tiltPin 6

SoftwareSerial Serial1(8,9); //10, 11); // RX, TX

Servo pan; //pan servo
Servo tilt; //tilt no used except for setup

// Variables for the duration and the distance

unsigned int distance;

void setup() {

  // Debug serial
  Serial.begin( 9600 );

  // Serial connected to LIDAR sensor
  Serial1.begin( 115200 );
pan.attach(panPin);
tilt.attach(tiltPin);
  delay( 5000 );
 // Serial.println( "Starting..." );
tilt.write(90);
}

void loop() {

   // rotates the servo motor from 15 to 165 degrees
  for(int i=15;i<=160;i++){  
  pan.write(i);
  
  //distance = readLIDAR( 2000 );
  distance = map(readLIDAR(2000),30,1200,1,39);
  //distance = random(1,39);
  delayMicroseconds(30);
  Serial.print(i); // Sends the current degree into the Serial Port
  Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  Serial.print(distance); // Sends the distance value into the Serial Port
  Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  }
    for(int i=160;i>15;i--){  
  pan.write(i);
  
  //distance = readLIDAR( 2000 );
  distance = map(readLIDAR(2000),30,1200,0,39);
  //distance = random(1,39);
  delayMicroseconds(30);
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

