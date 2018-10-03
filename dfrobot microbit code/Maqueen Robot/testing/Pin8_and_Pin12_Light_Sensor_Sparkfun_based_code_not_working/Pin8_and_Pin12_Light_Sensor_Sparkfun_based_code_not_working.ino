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
//const unsigned int THRESHOLD = 1; //800;

// Stop counting after this value (we can assume it is total darkness)
const unsigned int MAX_T = 20000; //20000;

// Pin definitions
//const int P_JNCT_PIN = -1;// P junction of sensing LED
//const int N_JNCT_PIN = 8;// N junction of sensing LED

// Global variables
unsigned int sen_time;// Time it takes to discharge LED

void setup() {

  // Start serial communications
  Serial.begin(9600);
microbit.begin();
bot.begin();
  // Set P junction pin to output low (GND)
 // pinMode(P_JNCT_PIN, OUTPUT);
 // digitalWrite(P_JNCT_PIN, LOW);

  
}

void loop() {

  // Read the amount of light falling on the LED
  readLED1();
  long int number1 = sen_time/150000;
  Serial.print("Number 1: "); Serial.print(number1);
  readLED2();
  long int number2 = sen_time/100000;
  Serial.print(" Number 2: "); Serial.println(number2);
if (number1 <= 1 && number2 <= 1) {
    bot.stop();
  } else  if (number1 >= 1 && number2 >= 1) {
    bot.setSpeed(30);
    bot.forward();
  } else if ( number2 >= 1 ) {
    bot.setSpeed(30);
    bot.left();
  } else if ( number1 >= 1) {
    bot.setSpeed(30);
    bot.right();
  } else {
  bot.stop(); }
}

void readLED1() {

  unsigned int t;
unsigned int avg;
  // Reset global LED discharge time
  sen_time = 0;
avg = 0;
for (int i=0; i<50; i++) {
  // Apply reverse voltage to charge the sensing LED's capacitance
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);
  
  
  // Isolate N junction and turn off pull-up resistor
  pinMode(LED1, INPUT);
  digitalWrite(LED1, LOW);
  
  // Count how long it takes for the LED to discharge
  for ( t = 0; t < MAX_T; t++ ) {
    if ( digitalRead(LED1) == 0 ) {
      break;
    }
avg = avg + t;
  }
}

sen_time = avg / 50;

}

void readLED2() {

  unsigned int t;
unsigned int avg;
  // Reset global LED discharge time
  sen_time = 0;
avg = 0;
for (int i=0; i<50; i++) {
  // Apply reverse voltage to charge the sensing LED's capacitance
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, HIGH);
  
  // Isolate N junction and turn off pull-up resistor
  pinMode(LED2, INPUT);
  digitalWrite(LED2, LOW);
  // Count how long it takes for the LED to discharge
  for ( t = 0; t < MAX_T; t++ ) {
    if ( digitalRead(LED2) == 0 ) {
      break;
    }
avg = avg + t;
}
}
sen_time = avg / 50;

}
