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

#define IR 16
#define LED 12
int readon = 0;
int readoff = 0;

void setup() {
pinMode(IR, INPUT);
pinMode(LED, OUTPUT);  
Serial.begin(9600);
}

void loop() {
  int n = read_SAMSUNG();
    if (n == -522182433) {
        // Key 1
        digitalWrite(LED, HIGH);
    } else if (n == -522149793) {
        // Key 2
        digitalWrite(LED, LOW);
    } 
Serial.println(n);
}


void wait_for_signal() {
  //wait for signal
  while(analogRead(IR) > 500) { }
}

int read_SAMSUNG() {
    wait_for_signal();
    int c1 = 0;
    int c2 = 0;
    long res = 0;
    while (c1 < 25 && c2 < 25) {
        c1 = read_on();
        c2 = read_off();
    }
    for (int i = 0; i < 32; i++) {
        c1 = read_on();
        c2 = read_off();
        res = res + res;
        if (c2 > 10) {
            res = res + 1;
        }
    }
    return res;
}


int read_on() {
  long c = 0;
  while (analogRead(IR) < 500 && c < 500) {
    c++;
  }
  return c;
}

int read_off() {
  long c = 0;
  while(analogRead(IR) > 500 && c< 500) {
    c++;
  }
  return c;
}

