/*
Copyright (c) 2019 LeRoy Miller

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
//Presumably each port also has I2C, but this is unverified
//voltages on each port are also unknown (presumably it's 5v, *WARNING* but Auriga boards could be as high as 12v)
//something on pin 31 (?)
//pin A4 maybe battery voltage (?)
//RUN LED is pin 13
//Neopixels on pin A3
//No sound sensor, no light sensors
//MPU6050 on I2C 0x69

 //Line Follow Port (Auriga port 10)
/*
#define pin1 A11 //LED2 side
#define pin2 A6 //LED1 side
*/

// Purple Port (Auriga port 9)
/*
#define pin1 A12 //(When using Line Follower this is LED2)
#define pin2 A7
*/

//Blue Port (Auriga port 8)
/*
#define pin1 A13 //(When using Line Follower this is LED2)
#define pin2 A8
*/

//light blue port (Auriga port 7)
/*
#define pin1 A14 //(When using Line Follower this is LED2)
#define pin2 A9
*/

//Ultrasonic Port (Auriga Port 1)
/*
#define pin1 5 //(When using newping.h This is ECHO)
#define pin2 4 //(When using newping.h This is TRIGGER)
//A little flaky with newping - maybe a timing thing
*/

//Orange Port (Auriga Port 2)
/*
#define pin1 3 //(When using Line Follower this is LED1)
#define pin2 2
*/

//Yellow Port (Auriga Port 3)
/*
#define pin1 7 //(When using Line Follower this is LED1)
#define pin2 6
*/

//Green Port (Auriga Port 5)
#define pin1 16 //TX2 
#define pin2 17 //Rx2

void setup() {
pinMode(pin1, INPUT);
pinMode(pin2, INPUT);  
Serial.begin(9600);
}

void loop() {
Serial.print("Pin1: "); Serial.print(digitalRead(pin1));
Serial.print(" Pin2: "); Serial.println(digitalRead(pin2));
delay(100);


}
