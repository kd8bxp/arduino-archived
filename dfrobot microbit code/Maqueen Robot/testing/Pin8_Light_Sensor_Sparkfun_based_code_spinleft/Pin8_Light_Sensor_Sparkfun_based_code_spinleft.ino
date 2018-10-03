/**
 * SparkFun Inventor's Kit Project
 * LED as Light Sensor
 * Date: July 8, 2016
 * 
 * Description:
 *  Demonstration of how to use an LED as a light sensors. Cover 
 *  up the LED plugged into pin 2/3 to light up the output LED
 *  plugged into pin 9. Note that you might have to change the
 *  THRESHOLD parameter. Connect a Serial terminal to get raw
 *  readings of the light value.
 *  
 * Hardware Connections:
 *  Pin | LED In 1 | LED 1 
 *  ----|----------|------
 *    2 |     +    |      
 *    3 |     -    |       
 *    4 |          |   +
 *  
 *  You will also need to connect the - sides of the output
 *  LED through a 330 Ohm resistor to ground.
 *  
 * License:
 *  Public Domain
 */
#include <Maqueen.h>

Maqueen bot;
Adafruit_Microbit_Matrix microbit;
// Any reading over this value will turn on the output LED
const unsigned int THRESHOLD = 900;

// Stop counting after this value (we can assume it is total darkness)
const unsigned int MAX_T = 20000;

// Pin definitions
const int P_JNCT_PIN = -1;// P junction of sensing LED
const int N_JNCT_PIN = 8;// N junction of sensing LED

// Global variables
unsigned int sen_time;// Time it takes to discharge LED

void setup() {

  // Start serial communications
  Serial.begin(9600);
microbit.begin();
bot.begin();
  // Set P junction pin to output low (GND)
  pinMode(P_JNCT_PIN, OUTPUT);
  digitalWrite(P_JNCT_PIN, LOW);

  
}

void loop() {

  // Read the amount of light falling on the LED
  readLED();

  // Print out the raw discharge time
  Serial.println(sen_time);
  int number = sen_time/50;
microbit.print(number);
  // If the light is below a certain level (discharge time is over the
  // threshold), turn on the output LED
  if ( sen_time > THRESHOLD ) {
    bot.setSpeed(30);
    bot.spinLeft();
  } else {
    bot.stop();
  }
  
}

void readLED() {

  unsigned int t;

  // Reset global LED discharge time
  sen_time = 0;

  // Apply reverse voltage to charge the sensing LED's capacitance
  pinMode(N_JNCT_PIN, OUTPUT);
  digitalWrite(N_JNCT_PIN, HIGH);

  // Isolate N junction and turn off pull-up resistor
  pinMode(N_JNCT_PIN, INPUT);
  digitalWrite(N_JNCT_PIN, LOW);

  // Count how long it takes for the LED to discharge
  for ( t = 0; t < MAX_T; t++ ) {
    if ( digitalRead(N_JNCT_PIN) == 0 ) {
      break;
    }

}

sen_time = t;

}
