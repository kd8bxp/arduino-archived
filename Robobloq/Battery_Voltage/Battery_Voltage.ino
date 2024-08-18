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

//Computer Volts is a rough guess - It maybe a little low
//It maybe ok - it maybe because of miscellanous componets
//on the board.
//Based on a 9v battery supply
//Voltage looks like it is on pin A7

void setup() {
Serial.begin(9600);
pinMode(A7, INPUT);
}

void loop() {
  Serial.print("Voltage: ");
  Serial.print(" Raw: ");
  float rawVolts = analogRead(A7)*(5.0/1023.0);
  Serial.print(rawVolts);
  float volts = map(rawVolts,0.0,3.0,0.0,9);
  Serial.print(" Computed Volts: ");
  Serial.println(volts);
  delay(1000);

}
