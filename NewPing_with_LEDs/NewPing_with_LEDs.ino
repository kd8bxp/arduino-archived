// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define REDLED 8
#define GREENLED 9
#define BLUELED 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
pinMode(REDLED, OUTPUT);
pinMode(GREENLED, OUTPUT);
pinMode(BLUELED, OUTPUT);
digitalWrite(REDLED, HIGH);
digitalWrite(GREENLED, HIGH);
digitalWrite(BLUELED, HIGH);

}

void loop() {
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  if (uS / US_ROUNDTRIP_CM >= 40) {digitalWrite(REDLED, HIGH); digitalWrite(BLUELED, HIGH); digitalWrite(GREENLED, LOW);}
  if (uS / US_ROUNDTRIP_CM <= 39 && uS / US_ROUNDTRIP_CM >= 30) {digitalWrite(REDLED, HIGH); digitalWrite(BLUELED, LOW); digitalWrite(GREENLED, HIGH);}
  if (uS / US_ROUNDTRIP_CM >= 0 && uS / US_ROUNDTRIP_CM <= 29) {digitalWrite(REDLED, LOW); digitalWrite(BLUELED, HIGH); digitalWrite(GREENLED, HIGH);}
  delay(100);
 // digitalWrite(REDLED, HIGH);
//digitalWrite(GREENLED, HIGH);
//digitalWrite(BLUELED, HIGH);
}
