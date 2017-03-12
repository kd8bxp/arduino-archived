#include <LiquidCrystal.h>
#include <AccelStepper.h>

float maxspeed = 750; //half speed for motor 1500 maxspeed
float inch = 200; //how many steps in a inch 200 steps equals 1 full turn
float inch2 = 12; //this is how many inches to start with 
float steps = inch * inch2;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
int lcd_key2    = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

AccelStepper stepper(AccelStepper::DRIVER, 24, 22);

int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor 

    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result

    if (adc_key_in > 1000) return btnNONE; 

    // For V1.1 us this threshold
    if (adc_key_in < 50)   return btnRIGHT;  
    if (adc_key_in < 250)  return btnUP; 
    if (adc_key_in < 450)  return btnDOWN; 
    if (adc_key_in < 650)  return btnLEFT; 
    if (adc_key_in < 850)  return btnSELECT;  

    return btnNONE;                // when all others fail, return this.
}

void (* resetFunc) (void) = 0;

int runsteppernow() {
	stepper.setMaxSpeed(maxspeed);
    stepper.setAcceleration((maxspeed*.05));
   	stepper.setEnablePin(0x18);
	stepper.runToNewPosition(steps);
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Holding Select");
	lcd.setCursor(0,1);
	lcd.print("to return sled.");


	lcd_key2 = 0;
	adc_key_in = 0;
	while (btnNONE) {
		lcd_key2 = read_LCD_buttons();   // read the buttons

   switch (lcd_key2){               // depending on which button was pushed, we perform an action

       case btnRIGHT:{             
                        break;
       }
       case btnLEFT:{
                         break;
       }    
       case btnUP:{
                         break;
       }
       case btnDOWN:{
       		             break;
       }
       case btnSELECT:{ 
       		 lcd.clear();
             lcd.setCursor(4,0);
             lcd.print("Running");
             stepper.runToNewPosition(0);
             return btnNONE;
       }
       case btnNONE:{
                         break;
       }
   }	
   }	
			}
	

int setlcdscreens() {
   lcd.begin(16, 2);               // start the library
   lcd.setCursor(0,0);             // set the LCD cursor   position 
   lcd.print("U/D Inches: ");   // print a simple message on the LCD
   lcd.setCursor(10,0);
   lcd.print (inch2);
   lcd.setCursor(0,1);
   lcd.print("L/R Speed: ");
   lcd.setCursor(12,1);
   lcd.print(maxspeed);
}

void setup(){
   setlcdscreens();
   pinMode(38, OUTPUT);
	pinMode(40, OUTPUT);
	digitalWrite(38, LOW);
	digitalWrite(40, LOW);
}
 
void loop(){
   
   lcd_key = read_LCD_buttons();   // read the buttons

   switch (lcd_key){               // depending on which button was pushed, we perform an action

       case btnRIGHT:{             
            maxspeed = maxspeed +(float)1;
            if (maxspeed > 1500) {maxspeed = 1500;}
            lcd.setCursor(12,1);
            lcd.print(maxspeed);
            break;
       }
       case btnLEFT:{
             maxspeed = maxspeed -(float)1;
             if (maxspeed < 100) {maxspeed = 100;}
             lcd.setCursor(12,1);
             lcd.print(maxspeed);
             break;
       }    
       case btnUP:{
             inch2 = inch2 + (float)1;
             steps = inch * inch2;
             lcd.setCursor(10,0);
             lcd.print(inch2);
             delay (100);
             break;
       }
       case btnDOWN:{
       		 inch2 = inch2 -(float)1;
             steps = inch / inch2;
             if (inch2 < 0) {inch2 = 0;}
             lcd.setCursor(10,0);
             lcd.print(inch2);
             delay (100);
             break;
       }
       case btnSELECT:{
             lcd.clear();
             lcd.setCursor(4,0);
             lcd.print("Running");
             runsteppernow();
             //resetFunc();
             setlcdscreens();
             
             break;
       }
       case btnNONE:{
             
             break;
       }
   }
}
