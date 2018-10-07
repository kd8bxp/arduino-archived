//------------------------------------------------------------------------------

enum gunFiremode
{
	FULLAUTO,
	SEMIAUTO,
};

//------------------------------------------------------------------------------

gunFiremode 	firemode 	= FULLAUTO;

const String 	fmodeStr[]	= { " Auto", " Semi" };

byte		 	ammoMag		= 0;				// cartridges in magazine - now
unsigned int 	ammoBag		= 0;				// cartridges in backpack - now

//------//
// OLED //
//------//

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const byte SID 	= 11;
const byte SCLK	= 13;
const byte DC	= 9;
const byte RST	= 2;
const byte CS	= 1;	// no such pin

/*const byte SID 	= 15;	// A0
const byte SCLK	= 14;	// A1
const byte DC	= 17;	// A2
const byte RST	= 16;	// A3
const byte CS	= 10;	// no such pin
*/

Adafruit_SSD1306 display(SID, SCLK, DC, RST, CS);

//------------------------------------------------------------------------------


//==============================================================================

void setup()
{
	Serial.begin(9600);
	
	scrInit();
	delay(2000);
}

//==============================================================================

void loop()
{
	scrRedraw();		delay(1000); display.clearDisplay(); display.display(); delay(1000);
	scrReload();		delay(1000); display.clearDisplay(); display.display(); delay(1000);
	scrNoAmmo();		delay(1000); display.clearDisplay(); display.display(); delay(1000);
	scrFiremode();		delay(1000); display.clearDisplay(); display.display(); delay(1000);
	scrGunDisabled();	delay(1000); display.clearDisplay(); display.display(); delay(1000);
}

//==============================================================================


//------------------------------------------------------------------------------
// OLED
//------------------------------------------------------------------------------

void scrInit()
{
	display.begin(SSD1306_SWITCHCAPVCC, 0x6C);
	
	scrRedraw();
}

void scrRedraw(void)
{	
	display.clearDisplay();
	
	// firemode
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);
	display.print("Mode:");
	display.setTextColor(BLACK, WHITE); 	// 'inverted' text
	display.println(fmodeStr[firemode]);
	
	// ammo
	display.setTextSize(4);
	display.setTextColor(WHITE);
	display.print((ammoMag >= 10 ? "" : " ") + (String)ammoMag);

	// text above ammoBag
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.print(" *Ammo");
	display.print("     ");
	display.setTextColor(BLACK, WHITE);

	display.print((ammoBag >= 100) 	? " " : (ammoBag >= 10 ? " 0" : " 00") + (String)ammoBag + " ");
	
	display.display();
}

void scrReload()
{
	display.clearDisplay();
	
	display.setTextSize(3);
	display.setTextColor(WHITE);
	display.setCursor(10, 20);
	display.println("Reload");

	display.display();
}

void scrNoAmmo()
{
	display.clearDisplay();
	
	display.setTextSize(3);
	display.setTextColor(WHITE);
	display.setCursor(0, 20);
	display.print("No Ammo");
	
	display.display();
}

void scrGunDisabled()
{
	display.clearDisplay();
}

void scrFiremode(void)
{
	firemode = (firemode == FULLAUTO) ? SEMIAUTO : FULLAUTO;
	
	scrRedraw();
}
//------------------------------------------------------------------------------
