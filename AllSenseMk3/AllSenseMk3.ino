#include <dht.h>  //Here we include all the libraries that we downloaded...
#include <BMP085.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define dht_apin A2  //saying what pin the DHT module should read the info.
#define NUMREADINGS 15 // here we are defining NUMREADINGS as 15 data values; raise this number to increase data smoothing. This is for the EMF.

int senseLimit = 15;
int probePin = A0; //setting probePin as analog 0.
int val = 0; // reading from probePin

dht DHT;

BMP085 bmp;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //starting the lcd with the pins we assigned.

const int inUse = 8; //this will be the pin where the tilt switch is connected to.
int i = 0;
const int screen = 6;

int readings[NUMREADINGS];                // the readings from the analog input
int index = 0;                            // the index of the current reading
int total = 0;                            // the running total
int average = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  bmp.begin();
  // Print a message to the LCD.

  pinMode(inUse, INPUT); 
  pinMode(screen, OUTPUT);
  for (int i = 0; i < NUMREADINGS; i++)
    readings[i] = 0;
}

void loop() {
  i = 0;
 
  if (digitalRead(inUse) == HIGH) {
    lcd.clear();
    digitalWrite(screen, LOW);
  }
  else {
    while (i < 30) {
      digitalWrite(screen, HIGH);
      DHT.read11(dht_apin);
      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      //Print a message to second line of LCD
      lcd.clear();
      lcd.setCursor(8, 0);
      lcd.print((bmp.readPressure() + 2000) / 100);
      Serial.println(bmp.readPressure());
      lcd.print("hPa");
      lcd.setCursor(1, 1);
      lcd.print(DHT.humidity);
      Serial.println(DHT.humidity);
      lcd.print("%");
      lcd.print("  ");
      lcd.print(DHT.temperature);
      lcd.print("C");

      val = analogRead(probePin);  // take a reading from the probe
      lcd.setCursor(0, 0);
      lcd.print("IE");
      if (val >= 1) {              // if the reading isn't zero, proceed

        val = constrain(val, 1, senseLimit);  // turn any reading higher than the senseLimit value into the senseLimit value
        val = map(val, 1, senseLimit, 1, 1023);  // remap the constrained value within a 1 to 1023 range

        total -= readings[index];               // subtract the last reading
        readings[index] = val; // read from the sensor
        total += readings[index];               // add the reading to the total
        index = (index + 1);                    // advance to the next index

        if (index >= NUMREADINGS) {               // if we're at the end of the array...
          index = 0;                            // ...wrap around to the beginning
        }
        average = total / NUMREADINGS; 
        Serial.println(average);
        if (average > 150) { //checking the value of the average and printing a ">" if it's significant.
          lcd.print(">");
        }


        if (average > 350) { //checking again, but this time for a higher value.
          lcd.print(">");
        }


        if (average > 550) { // and so on...
          lcd.print(">");
        }


        if (average > 750) {
          lcd.print(">");
        }


        if (average > 950) {
          lcd.print(">");
        }

      }

      i += 1;
      delay(1000);
    }

  }
}
