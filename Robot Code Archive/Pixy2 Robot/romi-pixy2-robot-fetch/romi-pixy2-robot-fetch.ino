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

/* based on project and code found at:
 *  https://www.hackster.io/gatoninja236/making-a-robot-play-fetch-657ebc
 */
 
#include <Pixy2.h>

Pixy2 pixy;

#define ldir 4 
#define lpwm 6 //10 
#define rdir 7 //5 
#define rpwm 5 //9
//Use L9110 motor driver

#define armPinA 2 
#define armPinB 3 

#define CENTER 200 //170
#define DEADZONE 10
int state = 1;
//int pos;

void setup() {
  Serial.begin(9600);
  Serial.print("Starting...\n");
  
  pixy.init();
  pixy.setLamp(1,0);
pinMode(ldir, OUTPUT);
pinMode(rdir, OUTPUT);  
pinMode(lpwm, OUTPUT);
pinMode(rpwm, OUTPUT);
pinMode(armPinA, OUTPUT);
pinMode(armPinB, OUTPUT);
stop();
open(); 
armStop();
    delay(1000); 
}

void loop() {
  int i; 
  // grab blocks!
  pixy.ccc.getBlocks();
  
  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks)
  {
    //Serial.print("Detected ");
    //Serial.println(pixy.ccc.numBlocks);
    for (i=0; i<pixy.ccc.numBlocks; i++)
    {
      /*Serial.print(pixy.ccc.blocks[i].m_width);
      Serial.print(" ");
      Serial.println(pixy.ccc.blocks[i].m_height);
      /*Serial.print("  block ");*/
      //Serial.print(i);
      //Serial.print(": ");
      //pixy.ccc.blocks[i].print();
      if(pixy.ccc.blocks[i].m_width > 220){
        set_gripper(2);
        move(0);
        state = 0;
        move(4);
        delay(1500);
        move(0);
        set_gripper(1);
        while(1);
      }
    Serial.print(pixy.ccc.blocks[i].m_x);
    Serial.print(",");
    Serial.println(pixy.ccc.blocks[i].m_width);
    }
    if(state==1){
    if(pixy.ccc.blocks[i].m_x >= CENTER-DEADZONE || pixy.ccc.blocks[i].m_x <= CENTER+DEADZONE){
      move(3);
      delay(5);
      move(0);
    }
    else if(pixy.ccc.blocks[i].m_x > CENTER){
      //while(pixy.ccc.blocks[i].m_x > CENTER){
        move(1);
        delay(2);
        move(0);
      //}
    }
    
    else if(pixy.ccc.blocks[i].m_x < CENTER){
      //while(pixy.ccc.blocks[i].m_x < CENTER){
        move(2);
        delay(2);
        move(0);
      //}
    }
  }  
  }
 

}


void armStop() {
  //analogWrite(armPinB, 0);
  digitalWrite(armPinB, 0);
  digitalWrite(armPinA, 0);
  
}

void close() {
  //analogWrite(lpwm, 50);
  digitalWrite(armPinB, HIGH);
  digitalWrite(armPinA, LOW);
  delay(2500);
}

void open() {
  //analogWrite(lpwm, 25);
  digitalWrite(armPinB, LOW);
  digitalWrite(armPinA, HIGH);
  delay(2500);
}

void stop() {
  analogWrite(lpwm, 0);
  analogWrite(rpwm, 0);
  digitalWrite(ldir, 0);
  digitalWrite(rdir, 0);
}

void back(int lspd, int rspd) {
  //because our robot switch the front for the back
  //we need to switch the direction of the motors in software
  analogWrite(lpwm, lspd);
  digitalWrite(ldir, HIGH);
  analogWrite(rpwm, rspd);
  digitalWrite(rdir, HIGH);
}

void forward(int lspd, int rspd) {
  analogWrite(lpwm, lspd);
  digitalWrite(ldir, LOW);
  analogWrite(rpwm, rspd);
  digitalWrite(rdir, LOW);
}

void move(int dir){
  switch(dir){
    case 0: //stop
      stop();
      break;
    case 1: //left
      forward(100,0);
      //delay(10);
      break;
    case 2: //right
      forward(0,100);
      //delay(10);
      break;
    case 3: //forward
      forward(50,50);
      //delay(10);
      break;
    case 4: //back
      back(50,50);
      //delay(10);
      break;
  }
}

void set_gripper(int state){
  switch(state){
    case 0: 
      close();
      armStop();
      break;
    case 1:
      open();
      armStop();
      break;
    case 2:
      close();
      armStop();
      break;
  }
}
