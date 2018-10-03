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
#include <Wire.h>
#include <NewPing.h>
#include <Adafruit_Microbit.h>
 
#define TRIGGER_PIN 1
#define ECHO_PIN 2
#define MAX_DISTANCE 500

int speed = 80; 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Adafruit_Microbit_Matrix microbit;

 const uint8_t
  smile_bmp[] =
  { B00000,
    B01010,
    B00000,
    B10001,
    B01110, };

void setup() {
  Serial.begin(9600);
   microbit.begin();
  Wire.begin(); // join i2c bus (address optional for master)
  delay(100);
  stop();
  //delay(500);
  microbit.clear();
  microbit.show(smile_bmp);
}

void loop() {
  while (sonar.ping_in() >= 5) {
forward(speed);
}

stop();

spinleft();
delay(100);
stop();


}
