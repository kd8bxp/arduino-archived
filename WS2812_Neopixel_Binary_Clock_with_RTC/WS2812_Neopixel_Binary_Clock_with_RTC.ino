#include <Adafruit_NeoPixel.h>
#include "Wire.h"
#define PIN 3
#define NUMPIXELS 27
#define DS3231_I2C_ADDRESS 0x68
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ400);

//start the time on 12:30:00
int hour=12, minute=30, second=00;

//initialize the pinout variables
int randomColor=0;
int setMinutes=0;
int setHours=0;
int ledStatus = 0;

//initialize the LED r,g,b color variables
int secRed=100;
int secGreen=10;
int secBlue=10;
int minRed=10;
int minGreen=100;
int minBlue=10;
int hourRed=10;
int hourGreen=10;
int hourBlue=100;

//initialize the time variables
int secondUnit = 0;
int minuteUnit = 0;
int hourUnit = 0;

//LED on and off control variables
int stringPixel = 0;
boolean light = 1;

//decimal number to packed binary-coded conversion
byte decToBcd(byte val)
{
return( (val/10*16) + (val%10) );
}
byte bcdToDec(byte val)
{
return( (val/16*10) + (val%16) );
}

void setup(){
//set outputs and inputs of the header pins
pinMode(6, OUTPUT); //Addressable Pixel LED Signal Output

pinMode(9, INPUT); //ON/OFF Button
pinMode(10, INPUT); //hour button
pinMode(11, INPUT); //minute button
pinMode(12, INPUT); //change colors

digitalWrite(9, HIGH);
digitalWrite(10, HIGH);
digitalWrite(11, HIGH);
digitalWrite(12, HIGH);

//left in for debugging purposes
Wire.begin();
Serial.begin(9600);

//set initial time on RTC
setDS3231time(hour,minute,second);
}

void setDS3231time(byte hour, byte minute, byte second){
Wire.beginTransmission(DS3231_I2C_ADDRESS);
Wire.write(0);
Wire.write(decToBcd(second));
Wire.write(decToBcd(minute));
Wire.write(decToBcd(hour));
Wire.endTransmission();
}

void loop(){
//establish connection to RTC
Wire.beginTransmission(DS3231_I2C_ADDRESS);
Wire.write(0);
Wire.endTransmission();
Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
//read time from RTC
second = bcdToDec(Wire.read() & 0x7f);
minute = bcdToDec(Wire.read());
hour = bcdToDec(Wire.read() & 0x3f);
//store RTC values into variables
secondUnit = second%10;
minuteUnit = minute%10; //sets the variable minuteUnit and hourUnit for the unit digits
hourUnit = hour%10;

//read digital pin for button press to turn LEDs on or off
ledStatus = digitalRead(9);
if (ledStatus == LOW){
light=!light;
delay(250);
}
//if button press detected, turn off LEDs
if(light==LOW){
for(stringPixel=0;stringPixel<=19;stringPixel++){
pixels.setPixelColor(stringPixel,pixels.Color(0,0,0));pixels.show();
}
}else{
//Setting the LEDs on or off to display the time
//Seconds
if(secondUnit == 1 || secondUnit == 3 || secondUnit == 5 || secondUnit == 7 || secondUnit == 9){
pixels.setPixelColor(0,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(0,pixels.Color(0,0,0));
pixels.show();
}

if(secondUnit == 2 || secondUnit == 3 || secondUnit == 6 || secondUnit == 7){
pixels.setPixelColor(1,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(1,pixels.Color(0,0,0));
pixels.show();
}

if(secondUnit == 4 || secondUnit == 5 || secondUnit == 6 || secondUnit == 7){
pixels.setPixelColor(2,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(2,pixels.Color(0,0,0));
pixels.show();
}

if(secondUnit == 8 || secondUnit == 9){
pixels.setPixelColor(3,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(3,pixels.Color(0,0,0));
pixels.show();
}

//seconds*10
if((second >= 10 && second < 20) || (second >= 30 && second < 40) || (second >= 50 && second < 60)) {
pixels.setPixelColor(4,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(4,pixels.Color(0,0,0));
pixels.show();
}

if(second >= 20 && second < 40) {
pixels.setPixelColor(5,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(5,pixels.Color(0,0,0));
pixels.show();
}

if(second >= 40 && second < 60) {
pixels.setPixelColor(6,pixels.Color(secRed,secGreen,secBlue));
pixels.show();
}else{
pixels.setPixelColor(6,pixels.Color(0,0,0));
pixels.show();
}

//minutes
if(minuteUnit == 1 || minuteUnit == 3 || minuteUnit == 5 || minuteUnit == 7 || minuteUnit == 9){
pixels.setPixelColor(7,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(7,pixels.Color(0,0,0));
pixels.show();
}

if(minuteUnit == 2 || minuteUnit == 3 || minuteUnit == 6 || minuteUnit == 7){
pixels.setPixelColor(8,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(8,pixels.Color(0,0,0));
pixels.show();
}

if(minuteUnit == 4 || minuteUnit == 5 || minuteUnit == 6 || minuteUnit == 7){
pixels.setPixelColor(9,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(9,pixels.Color(0,0,0));
pixels.show();
}

if(minuteUnit == 8 || minuteUnit == 9){
pixels.setPixelColor(10,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(10,pixels.Color(0,0,0));
pixels.show();
}

//minutes*10
if((minute >= 10 && minute < 20) || (minute >= 30 && minute < 40) || (minute >= 50 && minute < 60)) {
pixels.setPixelColor(11,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(11,pixels.Color(0,0,0));
pixels.show();
}

if(minute >= 20 && minute < 40) {
pixels.setPixelColor(12,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(12,pixels.Color(0,0,0));
pixels.show();
}

if(minute >= 40 && minute < 60) {
pixels.setPixelColor(13,pixels.Color(minRed,minGreen,minBlue));
pixels.show();
}else{
pixels.setPixelColor(13,pixels.Color(0,0,0));
pixels.show();
}

//hours
if(hourUnit == 1 || hourUnit == 3 || hourUnit == 5 || hourUnit == 7 || hourUnit == 9){
pixels.setPixelColor(14,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(14,pixels.Color(0,0,0));
pixels.show();
}

if(hourUnit == 2 || hourUnit == 3 || hourUnit == 6 || hourUnit == 7){
pixels.setPixelColor(15,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(15,pixels.Color(0,0,0));
pixels.show();
}

if(hourUnit == 4 || hourUnit == 5 || hourUnit == 6 || hourUnit == 7){
pixels.setPixelColor(16,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(16,pixels.Color(0,0,0));
pixels.show();
}

if(hourUnit == 8 || hourUnit == 9){
pixels.setPixelColor(17,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(17,pixels.Color(0,0,0));
pixels.show();
}

//hour*10
if(hour >= 10 && hour < 20){
pixels.setPixelColor(18,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(18,pixels.Color(0,0,0));
pixels.show();
}

if(hour >= 20 && hour < 24){
pixels.setPixelColor(19,pixels.Color(hourRed,hourGreen,hourBlue));
pixels.show();
}else{
pixels.setPixelColor(19,pixels.Color(0,0,0));
pixels.show();
}

//Setting the Time
setMinutes = digitalRead(11); // add one minute when pressed
if(setMinutes==LOW) {
Wire.beginTransmission(DS3231_I2C_ADDRESS);
Wire.write(0);
Wire.endTransmission();
Wire.requestFrom(DS3231_I2C_ADDRESS, 7);

second = bcdToDec(Wire.read() & 0x7f);
minute = bcdToDec(Wire.read());
hour = bcdToDec(Wire.read() & 0x3f);

minute++;
if(minute>=60){
minute=0;
}
second=0;

Wire.beginTransmission(DS3231_I2C_ADDRESS);
Wire.write(0);
Wire.write(decToBcd(second));
Wire.write(decToBcd(minute));
Wire.write(decToBcd(hour));
Wire.endTransmission();
delay(500);
}

setHours = digitalRead(10); // add one minute when pressed
if(setHours==LOW){
Wire.beginTransmission(DS3231_I2C_ADDRESS);
Wire.write(0);
Wire.endTransmission();
Wire.requestFrom(DS3231_I2C_ADDRESS, 7);

second = bcdToDec(Wire.read() & 0x7f);
minute = bcdToDec(Wire.read());
hour = bcdToDec(Wire.read() & 0x3f);

hour++;
if(hour>=24){
hour=0;
}
second=0;

Wire.beginTransmission(DS3231_I2C_ADDRESS);
Wire.write(0);
Wire.write(decToBcd(second));
Wire.write(decToBcd(minute));
Wire.write(decToBcd(hour));
Wire.endTransmission();
delay(500);
}

randomColor = digitalRead(12);
if(randomColor==0){
//Seconds Color
secRed=random(100,255);
secGreen=random(10,50);
secBlue=random(10,50);
//Minute Color
minRed=random(10,50);
minGreen=random(100,255);
minBlue=random(10,50);
//Hour Color
hourRed=random(10,50);
hourGreen=random(10,50);
hourBlue=random(100,255);
delay(500);
}

}

}