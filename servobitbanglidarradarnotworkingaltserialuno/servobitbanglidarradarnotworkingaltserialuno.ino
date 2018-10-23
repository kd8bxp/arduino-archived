/*
Copyright (c) 2018 LeRoy Miller

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  

https://github.com/kd8bxp
https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  
*/
//#include <SoftwareSerial.h>
#include <AltSoftSerial.h> //https://github.com/PaulStoffregen/AltSoftSerial
AltSoftSerial Serial1;//(8,9); //10, 11); // RX, TX

#define panPin 7
#define tiltPin 6

// Variables for the duration and the distance

unsigned int distance;

void setup() {
   Serial.begin( 9600 );

  // Serial connected to LIDAR sensor
  Serial1.begin( 115200 );
  delay(5000); //just wait....
pinMode(panPin,OUTPUT);
pinMode(tiltPin,OUTPUT);

//servoPosition(tiltPin, 90); 
delay(100);
servoPosition(panPin, 45); //1350);

}

void servoPosition(int servopin, int pulsemicros)
{
  for(int i=0; i<(pulsemicros*15); i++) { //gets about 90 degrees movement, call twice or change i<16 to i<32 if 180 needed
    digitalWrite(servopin, HIGH);
    delayMicroseconds(pulsemicros*15);
    digitalWrite(servopin, LOW);
    delay(25);
  }
  
}
void loop() {
  
 // rotates the servo motor from 15 to 165 degrees
  for(int i=45;i<=120;i++){  
  servoPosition(panPin, i);
  
  //distance = readLIDAR( 2000 );
  distance = map(readLIDAR(2000),30,1200,1,39);
  //distance = random(1,39);
  delay(300);
  Serial.print(i); // Sends the current degree into the Serial Port
  Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  Serial.print(distance); // Sends the distance value into the Serial Port
  Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  }
    for(int i=120;i>45;i--){  
  servoPosition(panPin, i);
  
  //distance = readLIDAR( 2000 );
  distance = map(readLIDAR(2000),30,1200,0,39);
  //distance = random(1,39);
  delay(300);
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

