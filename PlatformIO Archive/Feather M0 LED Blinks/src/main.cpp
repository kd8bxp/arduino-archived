#include <Arduino.h>

//#define PIN_LED2 0
//#define PIN_LED3 20

void setup() {
pinMode(LED_BUILTIN, OUTPUT);
pinMode(PIN_LED2, OUTPUT);
pinMode(PIN_LED3,OUTPUT);
}

void loop() {
 digitalWrite(LED_BUILTIN, HIGH);
 digitalWrite(PIN_LED2, LOW);
 digitalWrite(PIN_LED3, LOW);
 delay(500);
 digitalWrite(LED_BUILTIN, LOW);
 digitalWrite(PIN_LED2, HIGH);
 digitalWrite(PIN_LED3, LOW);
 delay(500);
 digitalWrite(LED_BUILTIN, LOW);
 digitalWrite(PIN_LED2, LOW);
 digitalWrite(PIN_LED3, HIGH);
 delay(500);
}

