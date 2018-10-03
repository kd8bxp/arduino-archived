#include <NewPing.h>
 
#define TRIGGER_PIN 1
#define ECHO_PIN 2
#define MAX_DISTANCE 500
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
 
void setup() {
   Serial.begin(9600);
}
 
void loop() {
   delay(50);
   unsigned int uS = sonar.ping_in();
   Serial.print(uS);
   Serial.println(" in");
   delay(500);
}
