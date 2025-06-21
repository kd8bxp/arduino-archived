/* Deep Sleep - Blink
 *
 * Blinks the onboard LED, sleeps for 10 seconds and repeats
 *
 * Connections:
 * D0 -- RST
 *
 * If you cant reprogram as the ESP is sleeping, disconnect D0 - RST and try again
 */

// sleep for this many seconds
const int sleepSeconds = 5;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nWake up");

  pinMode(BUILTIN_LED, OUTPUT);

  // Connect D0 to RST to wake up
  pinMode(D0, WAKEUP_PULLUP);

  // LED: LOW = on, HIGH = off
  Serial.println("Start blinking");
  for (int i = 0; i < 20; i++)
  {
    digitalWrite(BUILTIN_LED, LOW);
    delay(1000);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
  }
  Serial.println("Stop blinking");

  Serial.printf("Sleep for %d seconds\n\n", sleepSeconds);

  // convert to microseconds
  ESP.deepSleep(sleepSeconds * 1000000);
}

void loop() {
}
