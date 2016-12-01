#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

String message;
char tempmessage;
int val11;
int val2;
float volts;

//Works for strings but not floats, see the work around for the voltage display.
//when calling messageclear or messagedisplay you also supply the rows and col you want the message displayed at
//(ROW, COL) -- So, 0,0 is the upper left corner.

void setup()
{
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextSize(1);
//display.setTextColor(WHITE);
//display.setCursor(0,0);
message = " Bitty-Bot  Robot";
messagedisplay(0,0);
message = "Startup...";
messagedisplay(0,8);
delay(2000);
messageclear(0,8);
message = "Ready....";
messagedisplay(0,8);
delay(2000);
display.clearDisplay();
display.setTextSize(1);
message = " Bitty-Bot  Robot";
messagedisplay(0,0);
Serial.begin(9600);
}

void loop()
{
message = "Volts: ";
messagedisplay(0,56);
display.setTextColor(BLACK);
display.setCursor(45,56);
display.print(volts);
display.display();
display.setTextColor(WHITE);
display.setCursor(45,56);
checkvoltage();
display.print(volts);
display.display();
message = " V";
messagedisplay(70, 56);

delay(20000);
}

void messageclear(int posx, int posy) {
display.setTextColor(BLACK);
display.setCursor(posx, posy);
display.print(message);
display.display();
}

void messagedisplay(int posx, int posy) {
display.setTextColor(WHITE);
display.setCursor(posx, posy);
display.print(message);
display.display();
}

int checkvoltage() {
float temp;

val11=analogRead(1);
temp=(val11/4.092)/10;
val11=(int)temp * 10;//
val2=((val11%100)/10);

Serial.print("Raw Reading: ");
Serial.println(temp);
Serial.println("Corrected Reading: ");
Serial.println(val2);
//delay(1000);

if (temp <= 7.5 && temp >=6.3) {
	Serial.println("Very Low Voltage");
	tone(9, 1000, 100);
}

if (temp <= 6.3) {
	Serial.println("Voltage Crital");
	sos();
}
volts = temp;

}

int sos() {
	
	for (int xx=0; xx<3; xx++) {
		tone(9, 440, 100);
		delay(200);
		noTone(9);
	}
	for (int xx=0; xx<3; xx++){
		tone(9, 440, 300);
		delay(400);
		noTone(9);
	}
	for (int xx=0; xx<3; xx++) {
		tone(9, 440, 100);
		delay(200);
		noTone(9);
	}
	delay(1000);
	sos();
}

