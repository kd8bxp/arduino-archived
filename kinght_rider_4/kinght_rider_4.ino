/*
 * Knight Rider 4 based on 
 *  (cleft) 2005 K3, Malmo University
 * @author: David Cuartielles
 * @hardware: David Cuartielles, Aaron Hallborg
 * sketch and a question on reddit - that I just couldn't
 * resist to try.
 * 
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

int pinArray[] = {8, 9, 10, 11, 12, 13};
int count = -1;
int interval = 3;
long previousMillis = 0;        // will store last time LED was updated
long previousMillis2 = 0;
long previousMillis3 = 0;
bool ledState[] = {0,0,0,0,0,0};
bool flag = 0;
unsigned long currentMillis = millis();

void setup() {
  Serial.begin(9600);
   for (count=0;count<6;count++) {
    pinMode(pinArray[count], OUTPUT);
  }

}

void loop() {
  currentMillis = millis();
  if (flag == 0) { increase(); }
  if (flag == 1) { decrease(); }
  Serial.print("Flag: ");
  Serial.print(flag);
  Serial.print("count: ");
  Serial.println(count);
}

void increase() {
  count++;
  if (count >= 6) {flag = 1; return;}
  
  if (currentMillis - (previousMillis) >= interval ) {
  digitalWrite(pinArray[count], HIGH);
  previousMillis = currentMillis;
  }
  
  if (currentMillis - (previousMillis2+interval) >= interval) {
  digitalWrite(pinArray[count+1], HIGH);
  previousMillis2 = currentMillis;
  }
  if (currentMillis - (previousMillis3+interval) >= interval) {
    previousMillis3 = currentMillis;
    digitalWrite(pinArray[count], LOW);
}
}

void decrease() {
  count--;
  if (count < 0) {flag = 0; return;}
 if (currentMillis - (previousMillis) >= interval ) {
  digitalWrite(pinArray[count], HIGH);
  previousMillis = currentMillis;
  }
  
  if (currentMillis - (previousMillis2+interval) >= interval) {
  digitalWrite(pinArray[count-1], HIGH);
  previousMillis2 = currentMillis;
  }
  if (currentMillis - (previousMillis3+interval) >= interval) {
    previousMillis3 = currentMillis;
    digitalWrite(pinArray[count], LOW);
}
}
