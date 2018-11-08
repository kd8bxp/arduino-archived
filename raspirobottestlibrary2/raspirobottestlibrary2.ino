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

#include "Robot.h"

Robot *robot;

int fastspd = 70;
int mediumspd = 30;
int slowspd = 40;
int speed = 50; //percent of speed from 0 to 100 (0 stop, 100 full speed).

void setup() {
robot = new Robot();
Serial.begin(9600);
robot->begin();

}

void loop() {
robot->update();
if (robot->ping() > 15) {
  speed = fastspd;
  robot->speed(speed,speed);
  robot->forward(150000);
} else {
  speed = slowspd;
  robot->speed(speed,speed);
  robot->turnRight(150000);
}
}
