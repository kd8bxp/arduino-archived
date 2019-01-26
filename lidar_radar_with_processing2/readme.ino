/*
 * Copyright (c) 2018 LeRoy Miller

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

 * * Other portions of code based on: *
 * TFMini LIDAR Sensor test skecth for ESP32
 * 
 * Juan Jose Luna Espinosa 2018
 * https://github.com/yomboprime/TFMiniArduinoTest
 * 
 * Released under public domain
 * 
 * 
 * Connections:
 * Green cable to RX (software serial pin 8)
 * Red cable to 5V
 * Black cable to GND
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
 * 
 * Also PAC6985 Servo code: 
 * Original sourse: https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
 * This is the Arduino code PAC6985 16 channel servo controller
 * watch the video for details and demo http://youtu.be/y8X9X10Tn1k
 *  * 
 * Written by Ahmad Nejrabi for Robojax Video channel www.Robojax.com
 * Date: Dec 16, 2017, in Ajax, Ontario, Canada
 * Permission granted to share this code given that this
 * note is kept with the code.
 * Disclaimer: this code is "AS IS" and for educational purpose only.
 * this code has been downloaded from http://robojax.com/learn/arduino/
 */
