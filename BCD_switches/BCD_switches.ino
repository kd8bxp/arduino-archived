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
int tens, ones;
int temp1,temp2,temp4,temp8;

void setup() {
  //Ones Spot
pinMode(2, INPUT_PULLUP); //the one spot of BCD (ONES) 
pinMode(3, INPUT_PULLUP); //the two spot of BCD (ONES)
pinMode(4, INPUT_PULLUP); //the four spot of BCD (ONES)
pinMode(5, INPUT_PULLUP); //the eight spot of BCD (ONES)
  //Tens Spot
pinMode(6, INPUT_PULLUP); //the one spot of BCD (TENS) 
pinMode(7, INPUT_PULLUP); //the two spot of BCD (TENS)
pinMode(8, INPUT_PULLUP); //the four spot of BCD (TENS)
pinMode(9, INPUT_PULLUP); //the eight spot of BCD (TENS)
 //+/-
pinMode(10, INPUT_PULLUP); //the one spot of BCD (-) 
pinMode(11, INPUT_PULLUP); //the two spot of BCD (+)

Serial.begin(9600); 
}

void loop() {
//you would probably read the tens digit first...it may not matter..


//Read the ONES digit
if (digitalRead(2) == LOW) { temp1 = 1; } else {temp1 = 0;} //if this is LOW it will set the bit to the number it should be
if (digitalRead(3) == LOW) { temp2 = 2; } else {temp2 = 0;}
if (digitalRead(4) == LOW) { temp4 = 4; } else {temp4 = 0;}
if (digitalRead(5) == LOW) { temp8 = 8; } else {temp8 = 0;}

ones = temp1 + temp2 + temp4 + temp8; 

//Read the TENS digit the same as above with the same temp variables
if (digitalRead(6) == LOW) { temp1 = 1; } else {temp1 = 0;} //if this is LOW it will set the bit to the number it should be
if (digitalRead(7) == LOW) { temp2 = 2; } else {temp2 = 0;}
if (digitalRead(8) == LOW) { temp4 = 4; } else {temp4 = 0;}
if (digitalRead(9) == LOW) { temp8 = 8; } else {temp8 = 0;}


tens = temp1 + temp2 + temp4 + temp8;

//Serial.print(tens);
//Serial.println(ones);

//If you need this as a number not two digits you could do something like this.

int number;
number = (tens*10) + ones;

Serial.println(number); 
delay(500);
}

//That is probably how I read the BCD switches... 
